#pragma once

#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include "BasicOrderDto.h"

/* Begin DTO code-generation */
#include OATPP_CODEGEN_BEGIN(DTO)

class ExchangeOrderDto : public BasicOrderDto {
	DTO_INIT(ExchangeOrderDto, BasicOrderDto /* Extends */)

	DTO_FIELD(Int32, order_id);

	static int cnt;
public:
	ExchangeOrderDto(bool side, float price, int quantity);
};

/* End DTO code-generation */
#include OATPP_CODEGEN_END(DTO)