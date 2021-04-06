#pragma once

#include <list>
#include <mutex>
#include <thread>

#include "oatpp/core/async/Executor.hpp"

#include "structure/Order.h"

using std::mutex;

class TrackOrderService {
	typedef int SignalIdType;
	typedef order_types::TrackingOrder TrackingOrder;

	mutex mx;
	std::list<TrackingOrder> orders;

	oatpp::async::Executor executor;

	std::atomic<bool> checkUpdates;
	std::thread updateThread;
public:
	TrackOrderService() = delete;
	TrackOrderService(std::chrono::milliseconds &&updateInterval);
	~TrackOrderService();
	void addOrder(SignalIdType signal_id, const oatpp::Object<ExchangeOrderDto>& order);
	void updateOrders(std::chrono::milliseconds&& updateInterval);

	void updateOrder();
};