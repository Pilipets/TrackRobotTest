#pragma once

#include "oatpp/core/async/Coroutine.hpp"
#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include <memory>

class ExchangeApiClient;
class TrackOrderService;

class UpdateOrderCoroutine : public oatpp::async::Coroutine<UpdateOrderCoroutine> {
	typedef oatpp::web::protocol::http::Status Status;
	std::shared_ptr<ExchangeApiClient> exchangeApi;
	std::shared_ptr<TrackOrderService> trackService;
	std::shared_ptr<oatpp::data::mapping::ObjectMapper> objectMapper;
public:
	UpdateOrderCoroutine(
		std::shared_ptr<ExchangeApiClient> exchangeApi,
		std::shared_ptr<TrackOrderService> trackService,
		std::shared_ptr<oatpp::data::mapping::ObjectMapper> objectMapper);

	Action act() override;
};