#pragma once

#include <queue>
#include <mutex>
#include <thread>

#include "oatpp/core/async/Executor.hpp"
#include "oatpp/core/Types.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

using std::mutex;

class ExchangeApiClient;
class ActiveOrderService;
class TrackingOrderDto;
class ExchangeOrderDto;
class ExchangeExecutionDto;

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
	std::shared_ptr<oatpp::data::mapping::ObjectMapper> objectMapper;
public:
	TrackOrderService() = delete;
	TrackOrderService(std::chrono::milliseconds &&updateInterval,
						int concurrentUpdatesMax,
						std::shared_ptr<ExchangeApiClient> exchangeApiClient,
						std::shared_ptr<oatpp::data::mapping::ObjectMapper> objectMapper);
	~TrackOrderService();


	void addOrder(SignalIdType signal_id, const oatpp::Object<ExchangeOrderDto>& order);

	oatpp::Object<TrackingOrderType> getNextOrder();
	void updateOrder(
		oatpp::Object<TrackingOrderType> &&trackingOrder,
		oatpp::List<oatpp::Object<ExchangeExecutionDto>> &&executions);


	void addOrder(oatpp::Object<TrackingOrderType>&& trackingOrder);
	void updateOrders();

	void setActiveService(std::shared_ptr<ActiveOrderService> activeOrderService);
};