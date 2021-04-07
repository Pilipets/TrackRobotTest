#include "AppComponent.h"

#include "client/ExchangeApiClient.h"
#include "service/ActiveOrderService.h"
#include "service/TrackOrderService.h"

std::shared_ptr<ExchangeApiClient> AppComponent::createExchangeClient() {
    const char* host = "order.free.beeceptor.com";
    v_uint16 port = 80;
    OATPP_LOGI("ExchangeApiClient", "Using host='%s:%d'", host, port);

    auto connectionProvider = oatpp::network::tcp::client::ConnectionProvider::createShared({ host, port });
    auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(connectionProvider);

    OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);
    return ExchangeApiClient::createShared(requestExecutor, objectMapper);
}

std::shared_ptr<ActiveOrderService> AppComponent::createActiveOrderService()
{
    OATPP_COMPONENT(std::shared_ptr<ExchangeApiClient>, exchangeApiClient);
    OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);

    return std::make_shared<ActiveOrderService>(exchangeApiClient, objectMapper);
}

std::shared_ptr<TrackOrderService> AppComponent::createTrackOrderService()
{
    OATPP_COMPONENT(std::shared_ptr<ExchangeApiClient>, exchangeApiClient);
    OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);

    auto updateInterval = std::chrono::seconds(30);
    int maxConcurrentUpdates = 5;
    OATPP_LOGI("TrackOrderService", "Update each %d seconds, in groups of %d", updateInterval, maxConcurrentUpdates);

    return std::make_shared<TrackOrderService>(
        std::move(updateInterval), maxConcurrentUpdates, exchangeApiClient, objectMapper);
}

AppComponent::AppComponent() :
    exchangeApiClient(createExchangeClient()) {

    OATPP_COMPONENT(std::shared_ptr<ActiveOrderService>, activeOrderService);
    OATPP_COMPONENT(std::shared_ptr<TrackOrderService>, trackOrderService);

    activeOrderService->setTrackingService(trackOrderService);
    trackOrderService->setActiveService(activeOrderService);
}