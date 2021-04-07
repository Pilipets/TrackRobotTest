#pragma once

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"

#include "service/ActiveOrderService.h"
#include "dto/ClientOrderDto.h"

#include OATPP_CODEGEN_BEGIN(ApiController) ///< Begin Codegen
class ClientController : public oatpp::web::server::api::ApiController {
    std::shared_ptr<ActiveOrderService> activeOrderService;
    public:
        ClientController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper),
            OATPP_COMPONENT(std::shared_ptr<ActiveOrderService>, activeOrderService))
            : oatpp::web::server::api::ApiController(objectMapper), activeOrderService(activeOrderService) {

        }

    public:

        ENDPOINT("POST", "client/order", setNewOrder, BODY_DTO(Object<ClientOrderDto>, orderDto)) {
            OATPP_LOGD("ClientController", "Order request: '%s'", getDefaultObjectMapper()->writeToString(orderDto)->c_str());
            Status code = activeOrderService->acceptOrder(orderDto);
            return createResponse(code, code.description);
        }
};
#include OATPP_CODEGEN_END(ApiController) ///< End Codegen