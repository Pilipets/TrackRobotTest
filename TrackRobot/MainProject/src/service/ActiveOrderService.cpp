#include "ActiveOrderService.h"

#include "oatpp/core/Types.hpp"

#include "dto/ExchangeOrderDto.h"
#include "dto/ExchangeExecutionDto.h"
#include "dto/ExecutedOrderDto.h"
#include "dto/ClientOrderDto.h"
#include "dto/ActiveSignalDto.h"
#include "dto/TrackingOrderDto.h"

#include "client/ExchangeApiClient.h"
#include "service/TrackOrderService.h"

#include "coroutine/AddOrderCoroutine.h"
#include "coroutine/UpdateSignalCoroutine.h"

#include <memory>

void ActiveOrderService::addSignal(SignalIdType signal_id) {
	std::lock_guard<mutex> lk(mx);
	auto it = signals.find(signal_id);
	if (it == signals.end()) {
		signals.emplace(signal_id, ActiveSignalType::createShared(signal_id));
		OATPP_LOGD("ActiveOrderService", "[addSignal] for new signal=%d", signal_id);
	}
	else {
		OATPP_LOGD("ActiveOrderService", "[addSignal] on existing signal=%d", signal_id);
	}
}

void ActiveOrderService::updateSignal(const oatpp::Object<TrackingOrderType> &trackingOrder,
	int quantity, bool executed) {

	std::lock_guard<mutex> lk(mx);
	auto signal = signals[trackingOrder->signal_id];

	OATPP_LOGD("ActiveOrderService", "[updateSignal] updating signal=%d with quantity=%d", *signal->signal_id, quantity);
	signal->active_quantity = signal->active_quantity + quantity;

	if (executed) {
		OATPP_LOGD("ActiveOrderService", "[updateSignal] saving executed order order_id=%d with %d executions", *trackingOrder->order_id, trackingOrder->executions->size());
		signal->addFinished(trackingOrder.staticCast<oatpp::Object<ExecutedOrderDto>>());
	}

	if (signal->active_quantity == 0) {
		OATPP_LOGD("ActiveOrderService", "[updateSignal] removing from track signal signal=%d with %d orders", *signal->signal_id, signal->finished_orders->size());
		signals.erase(trackingOrder->signal_id);
	}
}

void ActiveOrderService::updateSignalAsync(const oatpp::Object<TrackingOrderType>& trackingOrder, int quantity, bool executed) {
	executor.execute<UpdateSignalCoroutine>(shared_from_this(), trackingOrder, quantity, executed);
}

Status ActiveOrderService::acceptOrder(const oatpp::Object<ClientOrderDto>& order) {
	// Create exchange order
	auto exchangeOrder = ExchangeOrderDto::createShared(
		order->side, order->price, order->quantity);

	// Send execution order
	OATPP_LOGD("ActiveOrderService", "[postOrder] body='%s'", objectMapper->writeToString(exchangeOrder)->c_str());
	auto response = exchangeApi->postOrder(exchangeOrder);

	OATPP_LOGD("ActiveOrderService", "[postOrder] code=%d, msg='%s'",
		response->getStatusCode(), response->getStatusDescription()->c_str());
	if (response->getStatusCode() != Status::CODE_200.code)
		return Status(response->getStatusCode(), response->getStatusDescription()->c_str());

	executor.execute<AddOrderCoroutine>(order, exchangeOrder, shared_from_this(), trackOrderService);
	return { 200, "Order accepted" };
}

ActiveOrderService::ActiveOrderService(std::shared_ptr<ExchangeApiClient> exchangeApiClient, std::shared_ptr<oatpp::data::mapping::ObjectMapper> objectMapper):
	exchangeApi(exchangeApiClient), objectMapper(objectMapper) {

}

void ActiveOrderService::setTrackingService(std::shared_ptr<TrackOrderService> trackOrderService)
{
	this->trackOrderService = trackOrderService;
}

ActiveOrderService::~ActiveOrderService() {
	executor.waitTasksFinished();
	executor.stop();
	executor.join();
}
