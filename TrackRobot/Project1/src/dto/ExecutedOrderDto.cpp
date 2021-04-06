#include "ExecutedOrderDto.h"

ExecutedOrderDto::ExecutedOrderDto(const oatpp::Object<ExchangeOrderDto>& order):
	ExchangeOrderDto(order->side, order->price, order->quantity) {

}
