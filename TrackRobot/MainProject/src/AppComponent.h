#pragma once

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"

#include "oatpp/core/macro/component.hpp"
#include "oatpp/network/Address.hpp"

class ExchangeApiClient;
class ActiveOrderService;
class TrackOrderService;

/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent {
private:
    std::shared_ptr<ExchangeApiClient> createExchangeClient();
    std::shared_ptr<ActiveOrderService> createActiveOrderService();
    std::shared_ptr<TrackOrderService> createTrackOrderService();
public:
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
        auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
        objectMapper->getDeserializer()->getConfig()->allowUnknownFields = false;
        return objectMapper;
    }());

    oatpp::base::Environment::Component<std::shared_ptr<ExchangeApiClient>> exchangeApiClient;

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
        return oatpp::network::tcp::server::ConnectionProvider::createShared({ "localhost", 8000, oatpp::network::Address::IP_4 });
    }());

    /**
     *  Create Router component
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
        return oatpp::web::server::HttpRouter::createShared();
    }());

    /**
     *  Create ConnectionHandler component which uses Router component to route requests
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
        return oatpp::web::server::HttpConnectionHandler::createShared(router);
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<ActiveOrderService>, activeOrderService)(createActiveOrderService());
    OATPP_CREATE_COMPONENT(std::shared_ptr<TrackOrderService>, trackOrderService)(createTrackOrderService());
    //oatpp::base::Environment::Component<std::shared_ptr<ActiveOrderService>> activeOrderService;
    //oatpp::base::Environment::Component<std::shared_ptr<TrackOrderService>> trackOrderService;

    AppComponent();
};