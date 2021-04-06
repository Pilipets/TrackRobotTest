#pragma once

#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/core/macro/codegen.hpp"

/* Begin DTO code-generation */
#include OATPP_CODEGEN_BEGIN(DTO)

class BasicOrderDto : public oatpp::DTO {
	DTO_INIT(BasicOrderDto, DTO /* Extends */)

	DTO_FIELD(Int8, side);			// long or short
	DTO_FIELD(Float32, price);		// limit order price
	DTO_FIELD(Int32, quantity);		//
public:
	BasicOrderDto(bool side, float price, int quantity) :
		side(side), price(price), quantity(quantity) {
	}
};

/* End DTO code-generation */
#include OATPP_CODEGEN_END(DTO)