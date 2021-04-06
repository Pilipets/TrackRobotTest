#pragma once

#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include "BasicOrderDto.h"

/* Begin DTO code-generation */
#include OATPP_CODEGEN_BEGIN(DTO)
class ExchangeExecution : public BasicOrderDto {
	DTO_INIT(ExchangeExecution, BasicOrderDto /* Extends */)

	DTO_FIELD(Int32, execution_id);
};

/* End DTO code-generation */
#include OATPP_CODEGEN_END(DTO)