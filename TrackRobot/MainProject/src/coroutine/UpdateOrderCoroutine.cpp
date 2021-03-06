#include "UpdateOrderCoroutine.h"

#include "client/ExchangeApiClient.h"
#include "service/TrackOrderService.h"

#include "dto/TrackingOrderDto.h"
#include "dto/ExchangeExecutionDto.h"

UpdateOrderCoroutine::UpdateOrderCoroutine(
	oatpp::Object<TrackingOrderDto> trackingOrder,
	std::shared_ptr<ExchangeApiClient> exchangeApi,
	std::shared_ptr<TrackOrderService> trackService,
	std::shared_ptr<oatpp::data::mapping::ObjectMapper> objectMapper) :
		trackingOrder(trackingOrder), exchangeApi(exchangeApi),
		trackService(trackService), objectMapper(objectMapper) {
}

UpdateOrderCoroutine::Action UpdateOrderCoroutine::act() {
	OATPP_LOGD("TrackOrderService", "[act] repeative signal=%d, order=%d", *trackingOrder->signal_id, *trackingOrder->order_id);

	auto response = exchangeApi->getOrder(trackingOrder->order_id);
	OATPP_LOGD("UpdateOrderCoroutine", "[act] code=%d, msg='%s'",
		response->getStatusCode(), response->getStatusDescription()->c_str());
	
	auto body = response->readBodyToString();
	auto executions = oatpp::List<oatpp::Object<ExchangeExecutionDto>>::createShared();
	if (response->getStatusCode() != Status::CODE_200.code) {
		// Add order back to the queue
		trackService->updateOrderExecution(std::move(trackingOrder), std::move(executions));
	}
	else {
		OATPP_LOGD("UpdateOrderCoroutine", "[act] response='%s'", body->c_str());

		try {
			executions = objectMapper->readFromString<oatpp::List<oatpp::Object<ExchangeExecutionDto>>>(body);
		} catch (std::exception& ex) {
			OATPP_LOGD("UpdateOrderCoroutine", "can't parse the response='%s'", ex.what());
		}
		// Check if executed fully in updateOrder
		trackService->updateOrderExecution(std::move(trackingOrder), std::move(executions));
	}
	return finish();
}
