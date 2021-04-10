#include "TrackOrderService.h"

#include "dto/ExchangeOrderDto.h"
#include "dto/TrackingOrderDto.h"
#include "dto/ExchangeExecutionDto.h"

#include "client/ExchangeApiClient.h"
#include "service/ActiveOrderService.h"

#include "coroutine/UpdateOrderCoroutine.h"

#include <thread>
#include <cmath>
#include <memory>
#include <numeric>

TrackOrderService::TrackOrderService(
		uint8_t concurrentUpdatesMax,
		std::shared_ptr<ExchangeApiClient> exchangeApiClient,
		std::shared_ptr<oatpp::data::mapping::ObjectMapper> objectMapper):
	executor(std::min(std::thread::hardware_concurrency(), static_cast<uint32_t>(concurrentUpdatesMax))),
	exchangeApi(exchangeApiClient), objectMapper(objectMapper) {

}

TrackOrderService::~TrackOrderService() {
	executor.waitTasksFinished();
	executor.stop();
	executor.join();
}

void TrackOrderService::addOrderAsync(SignalIdType signal_id, const oatpp::Object<ExchangeOrderDto> &order) {
	OATPP_LOGD("TrackOrderService", "[addOrderAsync] for signal=%d, order=%d", signal_id, *order->order_id);
	executor.execute<UpdateOrderCoroutine>(
		TrackingOrderType::createShared(signal_id, order),
		exchangeApi, shared_from_this(), objectMapper);
}

void TrackOrderService::updateOrderAsync(oatpp::Object<TrackingOrderType> &&trackingOrder) {
	executor.execute<UpdateOrderCoroutine>(
		std::move(trackingOrder),
		exchangeApi, shared_from_this(), objectMapper);
}

void TrackOrderService::updateOrderExecution(oatpp::Object<TrackingOrderType> &&trackingOrder,
	oatpp::List<oatpp::Object<ExchangeExecutionDto>> &&executions) {

	if (executions->size() == trackingOrder->executions->size() || executions->empty()) {
		// Add order back to the queue since nothing changed
		this->updateOrderAsync(std::move(trackingOrder));
	} else {
		int old_quantity = std::accumulate(trackingOrder->executions->begin(), trackingOrder->executions->end(), 0,
			[](int quantity, const oatpp::Object<ExchangeExecutionDto>& execution) {
				return quantity + execution->quantity;
		});

		int new_quantity = std::accumulate(executions->begin(), executions->end(), 0,
			[](int quantity, const oatpp::Object<ExchangeExecutionDto>& execution) {
				return quantity + execution->quantity;
		});

		// long is positive, short is negative
		bool executed = trackingOrder->quantity == new_quantity;
		int quantity_diff = (trackingOrder->side == 1 ? 1 : -1) * (new_quantity - old_quantity);

		OATPP_LOGD("TrackOrderService", "[updateOrder] new quantity(%d) executed for signal=%d, order=%d",
			quantity_diff, *trackingOrder->signal_id, *trackingOrder->order_id);

		trackingOrder->executions = std::move(executions);
		activeOrderService->updateSignalAsync(trackingOrder, quantity_diff, executed);

		if (!executed) this->updateOrderAsync(std::move(trackingOrder));
	}
}

void TrackOrderService::setActiveService(std::shared_ptr<ActiveOrderService> activeOrderService)
{
	this->activeOrderService = activeOrderService;
}
