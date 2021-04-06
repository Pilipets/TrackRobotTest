#include "OrderExecuteService.h"

#include "oatpp/core/Types.hpp"

#include "client/ExchangeApiClient.h"
#include "dto/ExchangeOrderDto.h"
#include "dto/ExchangeExecutionDto.h"
#include "dto/ClientOrderDto.h"

Status OrderExecuteService::acceptOrder(const oatpp::Object<ClientOrderDto>& order) {
	// Create exchange order
	auto exchangeOrder = ExchangeOrderDto::createShared(
		order->side, order->price, order->quantity);

	// Send execution order
	OATPP_LOGD("OrderExecutorService", "[setOrder] body='%s'", objectMapper->writeToString(exchangeOrder)->c_str());
	auto response = exchangeApiClient->postOrder(exchangeOrder);

	OATPP_LOGD("OrderExecutorService", "[setOrder] code=%d, msg='%s'",
		response->getStatusCode(), response->getStatusDescription()->c_str());
	if (response->getStatusCode() != Status::CODE_200.code)
		return Status(response->getStatusCode(), response->getStatusDescription()->c_str());

	// Add tracking for created order
	auto body = response->readBodyToString();
	OATPP_LOGD("OrderExecutorService", "[setOrder] response='%s'", body->c_str());

	auto executions = objectMapper->readFromString<oatpp::List<oatpp::Object<ExchangeExecution>>>(body);
	return { 200, "Order accepted" };
}
