#include "TrackingOrderDto.h"

#include "ExchangeOrderDto.h"

TrackingOrderDto::TrackingOrderDto(int signal_id, const oatpp::Object<ExchangeOrderDto>& exchangeOrder):
	ExecutedOrderDto(exchangeOrder), signal_id(signal_id) {
}