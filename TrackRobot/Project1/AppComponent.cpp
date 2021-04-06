#include "AppComponent.h"

#include "client/ExchangeApiClient.h"

std::shared_ptr<ExchangeApiClient> AppComponent::createExchangeClient() {
    OATPP_LOGD("AppComponent", "Using Oat++ native HttpRequestExecutor.");

    auto connectionProvider = oatpp::network::tcp::client::ConnectionProvider::createShared({ "order.free.beeceptor.com", 443 });
    auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(connectionProvider);
    OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);

    return ExchangeApiClient::createShared(requestExecutor, objectMapper);
}
