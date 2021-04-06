#pragma once

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include "ExecutedOrderDto.h"
class ExchangeOrderDto;

#include OATPP_CODEGEN_BEGIN(DTO)
class TrackingOrderDto : public ExecutedOrderDto {
	DTO_INIT(TrackingOrderDto, ExecutedOrderDto /* extends */)

	DTO_FIELD(Int32, signal_id);
public:
	TrackingOrderDto(int signal_id, const oatpp::Object<ExchangeOrderDto>& exchangeOrder);
};
#include OATPP_CODEGEN_END(DTO)