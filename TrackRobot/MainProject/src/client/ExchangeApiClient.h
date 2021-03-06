#pragma once

#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include "oatpp/core/macro/component.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

class ExchangeOrderDto;

#include OATPP_CODEGEN_BEGIN(ApiClient) ///< Begin code-gen section
class ExchangeApiClient : public oatpp::web::client::ApiClient {
public:
	ExchangeApiClient(std::shared_ptr<oatpp::data::mapping::ObjectMapper> objectMapper,
		std::shared_ptr<oatpp::web::client::RequestExecutor> requestExecutor)
		: oatpp::web::client::ApiClient(requestExecutor, objectMapper) {
	}

public:
	API_CLIENT_INIT(ExchangeApiClient)

	API_CALL("POST", "order", postOrder, BODY_DTO(Object<ExchangeOrderDto>, order))
	API_CALL("GET", "order", getOrder, QUERY(UInt32, order_id))
};

#include OATPP_CODEGEN_END(ApiClient) ///< End code-gen section