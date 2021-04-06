#pragma once

#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/core/macro/codegen.hpp"

/* Begin DTO code-generation */
#include OATPP_CODEGEN_BEGIN(DTO)

class ExchangeExecutionDto : public oatpp::DTO {
	DTO_INIT(ExchangeExecutionDto, DTO /* Extends */)

	DTO_FIELD(Int32, execution_id);
	DTO_FIELD(Int32, order_id);
	DTO_FIELD(Int8, side);
	DTO_FIELD(Float32, execution_price);
	DTO_FIELD(Int32, quantity);
};

/* End DTO code-generation */
#include OATPP_CODEGEN_END(DTO)