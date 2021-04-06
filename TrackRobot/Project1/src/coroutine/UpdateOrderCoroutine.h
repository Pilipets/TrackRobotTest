#pragma once

#include "oatpp/core/async/Coroutine.hpp"

class ClientOrderDto;
class ExchangeOrderDto;
class ActiveOrderService;
class TrackOrderService;

class UpdateOrderCoroutine : public oatpp::async::Coroutine<UpdateOrderCoroutine> {
	oatpp::Object<ClientOrderDto> clientOrder;
	oatpp::Object<ExchangeOrderDto> exchangeOrder;
	ActiveOrderService* activeService;
	TrackOrderService* trackService;
public:
	UpdateOrderCoroutine(oatpp::Object<ClientOrderDto> clientOrder,
		oatpp::Object<ExchangeOrderDto> exchangeOrder,
		ActiveOrderService* activeService,
		TrackOrderService* trackService);

	Action act() override;
};