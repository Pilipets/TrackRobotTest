#pragma once

#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include "BasicOrderDto.h"

/* Begin DTO code-generation */
#include OATPP_CODEGEN_BEGIN(DTO)
class ClientOrderDto : public BasicOrderDto {
	DTO_INIT(ClientOrderDto, BasicOrderDto /* Extends */)

	DTO_FIELD(Int32, signal_id);	// track id
public:
	ClientOrderDto(bool side, float price, int quantity, int signal_id):
		BasicOrderDto(side, price, quantity), signal_id(signal_id) {
	}
};

/* End DTO code-generation */
#include OATPP_CODEGEN_END(DTO)