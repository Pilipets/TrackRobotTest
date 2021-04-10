#include "AddOrderCoroutine.h"

#include "dto/ClientOrderDto.h"
#include "dto/ExchangeOrderDto.h"

#include "service/ActiveOrderService.h"
#include "service/TrackOrderService.h"

AddOrderCoroutine::AddOrderCoroutine(
	oatpp::Object<ClientOrderDto> clientOrder,
	oatpp::Object<ExchangeOrderDto> exchangeOrder,
	std::shared_ptr<ActiveOrderService> activeService,
	std::shared_ptr<TrackOrderService> trackService) :
		clientOrder(clientOrder), exchangeOrder(exchangeOrder),
		activeService(activeService), trackService(trackService) {

}

oatpp::async::Action AddOrderCoroutine::act() {
	OATPP_LOGD("AddOrderCoroutine", "[act] order=%d, signal=%d", *exchangeOrder->order_id, *clientOrder->signal_id);

	activeService->addSignal(clientOrder->signal_id);
	trackService->addOrderAsync(clientOrder->signal_id, exchangeOrder);
	return finish();
}
