#include "UpdateOrderCoroutine.h"

#include "client/ExchangeApiClient.h"
#include "service/ActiveOrderService.h"
#include "service/TrackOrderService.h"

#include "dto/TrackingOrderDto.h"
#include "dto/ExchangeExecutionDto.h"

UpdateOrderCoroutine::UpdateOrderCoroutine(
	std::shared_ptr<ExchangeApiClient> exchangeApi,
	std::shared_ptr<TrackOrderService> trackService,
	std::shared_ptr<oatpp::data::mapping::ObjectMapper> objectMapper) :
		exchangeApi(exchangeApi), trackService(trackService), objectMapper(objectMapper) {
}

UpdateOrderCoroutine::Action UpdateOrderCoroutine::act() {

	OATPP_LOGD("UpdateOrderCoroutine", "[act]");
	std::this_thread::sleep_for(std::chrono::seconds(5));
	// Get next order
	auto trackingOrder = trackService->getNextOrder();

	auto response = exchangeApi->getOrder(trackingOrder->order_id);
	OATPP_LOGD("UpdateOrderCoroutine", "[act] code=%d, msg='%s'",
		response->getStatusCode(), response->getStatusDescription()->c_str());
	
	auto body = response->readBodyToString();
	if (response->getStatusCode() != Status::CODE_200.code) {
		// Add order back to the queue
		trackService->updateOrder(std::move(trackingOrder), oatpp::List<oatpp::Object<ExchangeExecutionDto>>::createShared());
	}
	else {
		OATPP_LOGD("UpdateOrderCoroutine", "[act] response='%s'", body->c_str());

		auto executions = objectMapper->readFromString<oatpp::List<oatpp::Object<ExchangeExecutionDto>>>(body);
		// Check if executed fully in updateOrder
		trackService->updateOrder(std::move(trackingOrder), std::move(executions));
	}

	return finish();
}
