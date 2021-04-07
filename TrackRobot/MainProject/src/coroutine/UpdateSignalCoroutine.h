#pragma once

#include "oatpp/core/async/Coroutine.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include <memory>

class ActiveOrderService;
class TrackingOrderDto;

class UpdateSignalCoroutine : public oatpp::async::Coroutine<UpdateSignalCoroutine> {
	std::shared_ptr<ActiveOrderService> activeService;
	oatpp::Object<TrackingOrderDto> trackingOrder;
	int quantity;
	bool executed;
public:
	UpdateSignalCoroutine(
		std::shared_ptr<ActiveOrderService> activeService,
		const oatpp::Object<TrackingOrderDto> &trackingOrder,
		int quantity, bool executed);

	Action act() override;
};