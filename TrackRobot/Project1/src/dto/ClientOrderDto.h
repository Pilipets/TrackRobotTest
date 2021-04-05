#pragma once

#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/core/macro/codegen.hpp"

/* Begin DTO code-generation */
#include OATPP_CODEGEN_BEGIN(DTO)

class ClientOrderDto : public oatpp::DTO {
	DTO_INIT(ClientOrderDto, DTO /* Extends */)

	DTO_FIELD(Int8, side);		// long or short
	DTO_FIELD(Float32, price);		// limit order price
	DTO_FIELD(Int32, quantity);		//
	DTO_FIELD(Int32, signal_id);	// track id
};

/* End DTO code-generation */
#include OATPP_CODEGEN_END(DTO)