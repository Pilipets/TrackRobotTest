#include "ActiveOrderService.h"

#include "oatpp/core/Types.hpp"

#include "dto/ExchangeOrderDto.h"
#include "dto/ExchangeExecutionDto.h"
#include "dto/ClientOrderDto.h"

#include "client/ExchangeApiClient.h"
#include "service/TrackOrderService.h"

#include "coroutine/UpdateOrderCoroutine.h"

#include <memory>

void ActiveOrderService::addSignal(const oatpp::Object<ClientOrderDto> &order) {
	// from C++17 we can use shared_mutex as read-write lock here
	std::lock_guard<mutex> lk(mx);
	auto it = signals.find(*order->signal_id);
	if (it == signals.end()) {
		signals[*order->signal_id];
		OATPP_LOGD("ActiveOrderService", "[addSignal] for new signal=%d", *order->signal_id);
	}
	else {
		OATPP_LOGD("ActiveOrderService", "[addSignal] on existing signal=%d", *order->signal_id);
	}
}

void ActiveOrderService::updateSignal(const TrackingOrder& order) {
	std::lock_guard<mutex> lk(mx);
}

Status ActiveOrderService::acceptOrder(const oatpp::Object<ClientOrderDto>& order) {
	// Create exchange order
	auto exchangeOrder = ExchangeOrderDto::createShared(
		order->side, order->price, order->quantity);

	// Send execution order
	OATPP_LOGD("ActiveOrderService", "[setOrder] body='%s'", objectMapper->writeToString(exchangeOrder)->c_str());
	auto response = exchangeApi->postOrder(exchangeOrder);

	OATPP_LOGD("OrderExecutorService", "[setOrder] code=%d, msg='%s'",
		response->getStatusCode(), response->getStatusDescription()->c_str());
	if (response->getStatusCode() != Status::CODE_200.code)
		return Status(response->getStatusCode(), response->getStatusDescription()->c_str());

	executor.execute<UpdateOrderCoroutine>(order, exchangeOrder, this, trackService.get());
	return { 200, "Order accepted" };
}

ActiveOrderService::ActiveOrderService(std::shared_ptr<ExchangeApiClient> exchangeApiClient, std::shared_ptr<TrackOrderService> trackOrderService, std::shared_ptr<oatpp::data::mapping::ObjectMapper> objectMapper):
	exchangeApi(exchangeApiClient), trackService(trackOrderService), objectMapper(objectMapper) {
}

ActiveOrderService::~ActiveOrderService() {
	executor.waitTasksFinished(std::chrono::minutes(2));
	executor.stop();
	executor.join();
}
