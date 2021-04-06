#include "ExecutedOrderDto.h"

ExecutedOrderDto::ExecutedOrderDto(
	const oatpp::Object<ExchangeOrderDto>& order,
	List<Object<ExchangeExecutionDto>> executions):
	ExchangeOrderDto(order->side, order->price, order->quantity), executions(std::move(executions)) {

}
