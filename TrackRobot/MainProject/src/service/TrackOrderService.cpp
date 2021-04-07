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

void TrackOrderService::addOrder(oatpp::Object<TrackingOrderType> &&trackingOrder) {
	std::lock_guard<mutex> lk(mx);

	OATPP_LOGD("TrackOrderService", "[addOrder] repeative signal=%d, order=%d", *trackingOrder->signal_id, *trackingOrder->order_id);
	orders.push(std::move(trackingOrder));
}

TrackOrderService::TrackOrderService(
		std::chrono::milliseconds &&updateInterval,
		int concurrentUpdatesMax,
		std::shared_ptr<ExchangeApiClient> exchangeApiClient,
		std::shared_ptr<oatpp::data::mapping::ObjectMapper> objectMapper):
	checkUpdates(true), updateInterval(std::move(updateInterval)),
	concurrentUpdatesMax(concurrentUpdatesMax), exchangeApi(exchangeApiClient),
	objectMapper(objectMapper) {

	updateThread = std::thread(&TrackOrderService::updateOrders, this);
}

TrackOrderService::~TrackOrderService() {
	checkUpdates.store(false, std::memory_order_release);

	executor.waitTasksFinished(std::chrono::minutes(2));
	executor.stop();
	executor.join();

	if (updateThread.joinable())
		updateThread.join();
}

void TrackOrderService::addOrder(SignalIdType signal_id, const oatpp::Object<ExchangeOrderDto> &order) {
	std::lock_guard<mutex> lk(mx);

	OATPP_LOGD("TrackOrderService", "[addOrder] for signal=%d, order=%d", signal_id, *order->order_id);
	orders.push(TrackingOrderType::createShared(signal_id, order));
}

void TrackOrderService::updateOrders() {
	while (checkUpdates.load(std::memory_order_acquire)) {
		auto sleep_time = std::chrono::steady_clock::now() + updateInterval;

		// Get unfinished order numbers
		std::unique_lock<mutex> lk(mx);
		int lsize = orders.size();
		lk.unlock();
		OATPP_LOGD("TrackOrderService", "[updateOrders] on %d orders", lsize);

		// Invoke update for each on the portions of concurrentUpdatesMax
		for (int i = 0; i < lsize; i += concurrentUpdatesMax) {
			for (int j = std::min(lsize - i, concurrentUpdatesMax); j; --j) {
				executor.execute<UpdateOrderCoroutine>(
					exchangeApi, shared_from_this(), objectMapper);
			}

			executor.waitTasksFinished();
			if (executor.getTasksCount()) {
				OATPP_LOGD("TrackOrderService", "[updateOrders] %d updates left unfinished", executor.getTasksCount());
				executor.stop();
			}
		}
		std::this_thread::sleep_until(sleep_time);
	}
}

oatpp::Object<TrackOrderService::TrackingOrderType> TrackOrderService::getNextOrder() {
	std::lock_guard<mutex> lk(mx);
	auto trackingOrder = std::move(orders.front());
	orders.pop();

	return trackingOrder;
}

void TrackOrderService::updateOrder(oatpp::Object<TrackingOrderType> &&trackingOrder,
	oatpp::List<oatpp::Object<ExchangeExecutionDto>> &&executions) {

	if (executions->size() == trackingOrder->executions->size() || executions->empty()) {
		// Add order back to the queue since nothing changed
		this->addOrder(std::move(trackingOrder));
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

		if (!executed) this->addOrder(std::move(trackingOrder));
	}
}

void TrackOrderService::setActiveService(std::shared_ptr<ActiveOrderService> activeOrderService)
{
	this->activeOrderService = activeOrderService;
}
