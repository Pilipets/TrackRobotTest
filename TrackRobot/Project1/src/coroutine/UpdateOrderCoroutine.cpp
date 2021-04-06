#include "UpdateOrderCoroutine.h"

#include "dto/ClientOrderDto.h"
#include "dto/ExchangeOrderDto.h"

#include "service/ActiveOrderService.h"
#include "service/TrackOrderService.h"

UpdateOrderCoroutine::UpdateOrderCoroutine(oatpp::Object<ClientOrderDto> clientOrder,
	oatpp::Object<ExchangeOrderDto> exchangeOrder,
	ActiveOrderService* activeService,
	TrackOrderService* trackService) :
	clientOrder(clientOrder), exchangeOrder(exchangeOrder),
	activeService(activeService), trackService(trackService) {

}

oatpp::async::Action UpdateOrderCoroutine::act() {
	OATPP_LOGD("UpdateOrderCoroutine", "[act] order=%d, signal=%d", *exchangeOrder->order_id, *clientOrder->signal_id);

	activeService->addSignal(clientOrder);
	trackService->addOrder(clientOrder->signal_id, exchangeOrder);
	return finish();
}
