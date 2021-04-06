#pragma once

#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/core/async/Executor.hpp"

#include <unordered_map>
#include <list>
#include <mutex>
#include <memory>

#include "structure/Order.h"

using std::mutex;
using std::unordered_map;
using std::list;
using oatpp::web::protocol::http::Status;

class ExchangeApiClient;
class TrackOrderService;
class ClientOrderDto;


class ActiveOrderService : std::enable_shared_from_this<ActiveOrderService> {
	typedef int SignalIdType;
	typedef order_types::ActiveSignal SignalType;
	typedef order_types::TrackingOrder TrackingOrder;

	unordered_map<SignalIdType, SignalType> signals;
	mutex mx;

	oatpp::async::Executor executor;


	std::shared_ptr<ExchangeApiClient> exchangeApi;
	std::shared_ptr<TrackOrderService> trackService;
	std::shared_ptr<oatpp::data::mapping::ObjectMapper> objectMapper;
public:
	void addSignal(const oatpp::Object<ClientOrderDto> &order);
	void updateSignal(const TrackingOrder &order);
	Status acceptOrder(const oatpp::Object<ClientOrderDto>& order);

	ActiveOrderService(std::shared_ptr<ExchangeApiClient> exchangeApiClient,
		std::shared_ptr<TrackOrderService> trackOrderService,
		std::shared_ptr<oatpp::data::mapping::ObjectMapper> objectMapper);

	~ActiveOrderService();
};