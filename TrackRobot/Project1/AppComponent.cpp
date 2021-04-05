#pragma once

#include "AppComponent.h"

std::shared_ptr<ExchangeApiClient> AppComponent::createExchangeClient(const oatpp::network::Address& address) {
    OATPP_LOGD("App", "Using Oat++ native HttpRequestExecutor.");

    auto connectionProvider = oatpp::network::tcp::client::ConnectionProvider::createShared(address);
    auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(connectionProvider);
    OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);

    return ExchangeApiClient::createShared(requestExecutor, objectMapper);
}