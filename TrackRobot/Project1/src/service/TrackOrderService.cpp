#include "TrackOrderService.h"

#include "dto/ExchangeOrderDto.h"

#include <memory>

TrackOrderService::TrackOrderService() {
}

TrackOrderService::~TrackOrderService() {
	executor.waitTasksFinished(std::chrono::minutes(2));
	executor.stop();
	executor.join();
}

void TrackOrderService::addOrder(SignalIdType signal_id, const oatpp::Object<ExchangeOrderDto>& order) {
	std::lock_guard<mutex> lk(mx);

	OATPP_LOGD("TrackOrderService", "[addOrder] for signal=%d, order=%d", signal_id, *order->order_id);
	orders.push_back(order_types::TrackingOrder(signal_id, order));
}

void TrackOrderService::updateOrders() {
	//std::lock_guard<mutex> lk(mx);
	updateOrder();
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
