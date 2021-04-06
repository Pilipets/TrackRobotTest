#pragma once

#include <list>
#include <mutex>

#include "oatpp/core/async/Executor.hpp"

#include "structure/Order.h"

using std::mutex;

class TrackOrderService {
	typedef int SignalIdType;
	typedef order_types::TrackingOrder TrackingOrder;

	mutex mx;
	std::list<TrackingOrder> orders;

	oatpp::async::Executor executor;
public:
	TrackOrderService();
	~TrackOrderService();
	void addOrder(SignalIdType signal_id, const oatpp::Object<ExchangeOrderDto>& order);
	void updateOrders();

	void updateOrder();
};