#pragma once

#include "ClientOrderDto.h"

#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/core/macro/codegen.hpp"

/* Begin DTO code-generation */
#include OATPP_CODEGEN_BEGIN(DTO)

class ExchangeOrderDto : public ClientOrderDto {
	DTO_INIT(ExchangeOrderDto, ClientOrderDto /* Extends */)

	DTO_FIELD(Int32, order_id);		// order-id
};

/* End DTO code-generation */
#include OATPP_CODEGEN_END(DTO)