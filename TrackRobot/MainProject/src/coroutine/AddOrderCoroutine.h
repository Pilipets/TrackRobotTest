#pragma once

#include "oatpp/core/async/Coroutine.hpp"

class ClientOrderDto;
class ExchangeOrderDto;
class ActiveOrderService;
class TrackOrderService;

class AddOrderCoroutine : public oatpp::async::Coroutine<AddOrderCoroutine> {
	oatpp::Object<ClientOrderDto> clientOrder;
	oatpp::Object<ExchangeOrderDto> exchangeOrder;
	std::shared_ptr<ActiveOrderService> activeService;
	std::shared_ptr<TrackOrderService> trackService;
public:
	AddOrderCoroutine(oatpp::Object<ClientOrderDto> clientOrder,
		oatpp::Object<ExchangeOrderDto> exchangeOrder,
		std::shared_ptr<ActiveOrderService> activeService,
		std::shared_ptr<TrackOrderService> trackService);

	Action act() override;
};