#include "UpdateSignalCoroutine.h"

#include "service/ActiveOrderService.h"

#include "dto/TrackingOrderDto.h"

UpdateSignalCoroutine::UpdateSignalCoroutine(std::shared_ptr<ActiveOrderService> activeService,
	const oatpp::Object<TrackingOrderDto> &trackingOrder, int quantity, bool executed):
		activeService(activeService), trackingOrder(trackingOrder), quantity(quantity), executed(executed) {
}

UpdateSignalCoroutine::Action UpdateSignalCoroutine::act()
{
	activeService->updateSignal(trackingOrder, quantity, executed);
	return finish();
}
