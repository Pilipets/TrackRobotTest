#pragma once

#include "oatpp/core/Types.hpp"

#include "ExchangeOrderDto.h"
#include "ExchangeExecutionDto.h"

#include OATPP_CODEGEN_BEGIN(DTO)
class ExecutedOrderDto : public ExchangeOrderDto {
	DTO_INIT(ExecutedOrderDto, ExchangeOrderDto /* Extends */)

	DTO_FIELD(List<Object<ExchangeExecutionDto>>, executions);
public:
	ExecutedOrderDto(const oatpp::Object<ExchangeOrderDto>& order,
		List<Object<ExchangeExecutionDto>> executions = List<Object<ExchangeExecutionDto>>::createShared());
};
#include OATPP_CODEGEN_END(DTO)