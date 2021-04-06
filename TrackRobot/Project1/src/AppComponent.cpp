#include "AppComponent.h"

#include "client/ExchangeApiClient.h"
#include "service/ActiveOrderService.h"
#include "service/TrackOrderService.h"

std::shared_ptr<ExchangeApiClient> AppComponent::createExchangeClient() {
    OATPP_LOGD("AppComponent", "Using Oat++ native HttpRequestExecutor.");

    auto connectionProvider = oatpp::network::tcp::client::ConnectionProvider::createShared({ "order.free.beeceptor.com", 80 });
    auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(connectionProvider);

    OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);
    return ExchangeApiClient::createShared(requestExecutor, objectMapper);
}

std::shared_ptr<ActiveOrderService> AppComponent::createActiveOrderService()
{
    OATPP_COMPONENT(std::shared_ptr<ExchangeApiClient>, exchangeApiClient);
    OATPP_COMPONENT(std::shared_ptr<TrackOrderService>, trackOrderService);
    OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);

    return std::make_shared<ActiveOrderService>(exchangeApiClient, trackOrderService, objectMapper);
}

std::shared_ptr<TrackOrderService> AppComponent::createTrackOrderService()
{
    return std::make_shared<TrackOrderService>();
}

AppComponent::AppComponent() :
    exchangeApiClient(createExchangeClient()),
    trackOrderService(createTrackOrderService()),
    activeOrderService(createActiveOrderService()) {
}