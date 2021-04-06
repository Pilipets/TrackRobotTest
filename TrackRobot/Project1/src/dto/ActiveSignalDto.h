#pragma once

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include "ExecutedOrderDto.h"

#include OATPP_CODEGEN_BEGIN(DTO)
class ActiveSignalDto : public oatpp::DTO {
	DTO_INIT(ActiveSignalDto, DTO /* extends */)

	DTO_FIELD(Int32, signal_id);
	DTO_FIELD(Int32, active_quantity);

	DTO_FIELD(List<Object<ExecutedOrderDto>>, finished_orders);
public:
	ActiveSignalDto(int signal_id);

	void addFinished(Object<ExecutedOrderDto>&& order);
};

#include OATPP_CODEGEN_END(DTO)