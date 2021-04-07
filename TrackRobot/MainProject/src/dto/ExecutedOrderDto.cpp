#include "ExecutedOrderDto.h"

ExecutedOrderDto::ExecutedOrderDto(
	const oatpp::Object<ExchangeOrderDto>& order,
	List<Object<ExchangeExecutionDto>> executions):
	ExchangeOrderDto(*order.get()), executions(std::move(executions)) {

}
