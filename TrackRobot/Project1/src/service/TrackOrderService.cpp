#include "TrackOrderService.h"

#include "dto/ExchangeOrderDto.h"

#include <thread>
#include <memory>

TrackOrderService::TrackOrderService(std::chrono::milliseconds &&updateInterval):
	checkUpdates(true) {

	updateThread = std::thread(&TrackOrderService::updateOrders, this, std::move(updateInterval));
}

TrackOrderService::~TrackOrderService() {
	checkUpdates.store(false, std::memory_order_release);

	executor.waitTasksFinished(std::chrono::minutes(2));
	executor.stop();
	executor.join();

	if (updateThread.joinable())
		updateThread.join();
}

void TrackOrderService::addOrder(SignalIdType signal_id, const oatpp::Object<ExchangeOrderDto>& order) {
	std::lock_guard<mutex> lk(mx);

	OATPP_LOGD("TrackOrderService", "[addOrder] for signal=%d, order=%d", signal_id, *order->order_id);
	orders.push_back(order_types::TrackingOrder(signal_id, order));
}

void TrackOrderService::updateOrders(std::chrono::milliseconds&& updateInterval) {
	while (checkUpdates.load(std::memory_order_acquire)) {
		auto sleep_time = std::chrono::steady_clock::now() + updateInterval;
		std::unique_lock<mutex> lk(mx);
		auto lsize = orders.size();
		lk.unlock();
		OATPP_LOGD("TrackOrderService", "[updateOrders] on %d orders", lsize);

		std::this_thread::sleep_until(sleep_time);
	}
}

void TrackOrderService::updateOrder() {
	/*
	// Log receivedAdd tracking for created order
	auto body = response->readBodyToString();
	OATPP_LOGD("ActiveOrderService", "[setOrder] response='%s'", body->c_str());

	auto executions = objectMapper->readFromString<oatpp::List<oatpp::Object<ExchangeExecution>>>(body);
	return { 200, "Order accepted" };
	*/
}
