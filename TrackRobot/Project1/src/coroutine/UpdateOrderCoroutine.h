#pragma once

#include "oatpp/core/async/Coroutine.hpp"
#include "oatpp/web/protocol/http/Http.hpp"

#include <memory>

class ExchangeApiClient;
class TrackOrderService;
class ActiveOrderService;

class UpdateOrderCoroutine : public oatpp::async::Coroutine<UpdateOrderCoroutine> {
	typedef oatpp::web::protocol::http::Status Status;
	std::shared_ptr<ExchangeApiClient> exchangeApi;
	std::shared_ptr<TrackOrderService> trackService;
	std::shared_ptr<ActiveOrderService> activeService;
public:
	UpdateOrderCoroutine(
		std::shared_ptr<ExchangeApiClient> exchangeApi,
		std::shared_ptr<ActiveOrderService> activeService,
		std::shared_ptr<TrackOrderService> trackService);

	Action act() override;
};