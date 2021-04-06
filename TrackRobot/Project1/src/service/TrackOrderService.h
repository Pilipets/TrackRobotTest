#pragma once

#include <queue>
#include <mutex>
#include <thread>

#include "oatpp/core/async/Executor.hpp"
#include "oatpp/core/Types.hpp"

using std::mutex;

class ExchangeApiClient;
class ActiveOrderService;
class TrackingOrderDto;
class ExchangeOrderDto;

class TrackOrderService : public std::enable_shared_from_this<TrackOrderService> {
	typedef int SignalIdType;
	typedef TrackingOrderDto TrackingOrderType;

	mutex mx;
	std::queue< oatpp::Object<TrackingOrderType>> orders;

	std::chrono::milliseconds updateInterval;
	int concurrentUpdatesMax;

	oatpp::async::Executor executor;
	std::atomic<bool> checkUpdates;
	std::thread updateThread;

	std::shared_ptr<ExchangeApiClient> exchangeApi;
	std::shared_ptr<ActiveOrderService> activeOrderService;
public:
	TrackOrderService() = delete;
	TrackOrderService(std::chrono::milliseconds &&updateInterval,
						int concurrentUpdatesMax,
						std::shared_ptr<ExchangeApiClient> exchangeApiClient);
	~TrackOrderService();


	void addOrder(SignalIdType signal_id, const oatpp::Object<ExchangeOrderDto>& order);
	void updateOrders();

	oatpp::Object<TrackingOrderType> getNextOrder();
	void updateOrder();

	void setActiveService(std::shared_ptr<ActiveOrderService> activeOrderService) {
		this->activeOrderService = activeOrderService;
	}
};