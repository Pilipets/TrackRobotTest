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

	oatpp::async::Executor executor;

	std::shared_ptr<ExchangeApiClient> exchangeApi;
	std::shared_ptr<ActiveOrderService> activeOrderService;
	std::shared_ptr<oatpp::data::mapping::ObjectMapper> objectMapper;
public:
	TrackOrderService() = delete;
	TrackOrderService(uint8_t concurrentUpdatesMax,
		std::shared_ptr<ExchangeApiClient> exchangeApiClient,
		std::shared_ptr<oatpp::data::mapping::ObjectMapper> objectMapper);
	~TrackOrderService();


	void addOrderAsync(SignalIdType signal_id, const oatpp::Object<ExchangeOrderDto>& order);

	void updateOrderExecution(
		oatpp::Object<TrackingOrderType> &&trackingOrder,
		oatpp::List<oatpp::Object<ExchangeExecutionDto>> &&executions);


	void updateOrderAsync(oatpp::Object<TrackingOrderType> &&trackingOrder);

	void setActiveService(std::shared_ptr<ActiveOrderService> activeOrderService);
};