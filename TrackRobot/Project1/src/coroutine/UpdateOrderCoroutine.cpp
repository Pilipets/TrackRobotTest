#include "UpdateOrderCoroutine.h"

#include "client/ExchangeApiClient.h"
#include "service/ActiveOrderService.h"
#include "service/TrackOrderService.h"

#include "dto/TrackingOrderDto.h"

UpdateOrderCoroutine::UpdateOrderCoroutine(
	std::shared_ptr<ExchangeApiClient> exchangeApi,
	std::shared_ptr<ActiveOrderService> activeService,
	std::shared_ptr<TrackOrderService> trackService) :
		exchangeApi(exchangeApi), activeService(activeService), trackService(trackService) {
}

UpdateOrderCoroutine::Action UpdateOrderCoroutine::act() {

	OATPP_LOGD("UpdateOrderCoroutine", "[act]");
	std::this_thread::sleep_for(std::chrono::seconds(5));
	// Get next order
	auto trackingOrder = trackService->getNextOrder();

	auto response = exchangeApi->getOrder(trackingOrder->order_id);
	/*
	OATPP_LOGD("UpdateOrderCoroutine", "[act] code=%d, msg='%s'",
		response->getStatusCode(), response->getStatusDescription()->c_str());
	if (response->getStatusCode() != Status::CODE_200.code) {
		// Add order back to the queue
	}
	else {
		// Check if executed fully in updateOrder
		// Update signal with new executions
		//activeService->updateSignal(order);
	}
	*/
	return finish();
}
