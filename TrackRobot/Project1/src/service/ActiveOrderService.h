#pragma once

#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/core/async/Executor.hpp"
#include "oatpp/core/Types.hpp"

#include <unordered_map>
#include <list>
#include <mutex>
#include <memory>

using std::mutex;
using std::unordered_map;
using std::list;
using oatpp::web::protocol::http::Status;

class ExchangeApiClient;
class TrackOrderService;
class ClientOrderDto;
class ActiveSignalDto;
class TrackingOrderDto;


class ActiveOrderService : public std::enable_shared_from_this<ActiveOrderService> {
	typedef int SignalIdType;
	typedef ActiveSignalDto ActiveSignalType;
	typedef TrackingOrderDto TrackingOrderType;

	unordered_map<SignalIdType, oatpp::Object<ActiveSignalType>> signals;
	mutex mx;

	oatpp::async::Executor executor;


	std::shared_ptr<ExchangeApiClient> exchangeApi;
	std::shared_ptr<TrackOrderService> trackOrderService;
	std::shared_ptr<oatpp::data::mapping::ObjectMapper> objectMapper;
public:
	void addSignal(SignalIdType signal_id);
	void updateSignal(const oatpp::Object<TrackingOrderType> &order, int quantity_diff, bool executed);

	Status acceptOrder(const oatpp::Object<ClientOrderDto>& order);

	ActiveOrderService(std::shared_ptr<ExchangeApiClient> exchangeApiClient,
		std::shared_ptr<oatpp::data::mapping::ObjectMapper> objectMapper);

	void setTrackingService(std::shared_ptr<TrackOrderService> trackOrderService) {
		this->trackOrderService = trackOrderService;
	}

	~ActiveOrderService();
};