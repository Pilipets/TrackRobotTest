#include "TrackOrderService.h"

#include "dto/ExchangeOrderDto.h"
#include "dto/TrackingOrderDto.h"

#include "client/ExchangeApiClient.h"
#include "service/ActiveOrderService.h"

#include "coroutine/UpdateOrderCoroutine.h"

#include <thread>
#include <cmath>
#include <memory>

TrackOrderService::TrackOrderService(std::chrono::milliseconds &&updateInterval,
	int concurrentUpdatesMax,
	std::shared_ptr<ExchangeApiClient> exchangeApiClient):
		checkUpdates(true), updateInterval(std::move(updateInterval)),
		concurrentUpdatesMax(concurrentUpdatesMax), exchangeApi(exchangeApiClient) {

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
					exchangeApi, activeOrderService, shared_from_this());
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

void TrackOrderService::updateOrder() {
	/*
	// Log receivedAdd tracking for created order
	auto body = response->readBodyToString();
	OATPP_LOGD("ActiveOrderService", "[setOrder] response='%s'", body->c_str());

	auto executions = objectMapper->readFromString<oatpp::List<oatpp::Object<ExchangeExecutionDto>>>(body);
	return { 200, "Order accepted" };
	*/
}
