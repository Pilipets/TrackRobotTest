#pragma once

#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include <unordered_map>
#include <mutex>

using std::mutex;
using std::unordered_map;
using oatpp::web::protocol::http::Status;

class ClientOrderDto;
class ExchangeApiClient;

class OrderExecuteService {
	mutex mx;
	std::unordered_map<int, int> signals;

	OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);
	OATPP_COMPONENT(std::shared_ptr<ExchangeApiClient>, exchangeApiClient);
public:
	Status acceptOrder(const oatpp::Object<ClientOrderDto>& order);
};