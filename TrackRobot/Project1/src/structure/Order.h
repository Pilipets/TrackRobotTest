#pragma once

#include "oatpp/core/Types.hpp"

#include <list>

class ExchangeOrderDto;
class ExchangeExecution;

namespace order_types {
	class FinishedOrder {
		oatpp::Object<ExchangeOrderDto> order;
		std::list<oatpp::Object<ExchangeExecution>> executions;
	public:
		FinishedOrder(const oatpp::Object<ExchangeOrderDto>& order) :
			order(order) {

		}
	};

	class ActiveSignal {
		int acive_quantity = 0;
		std::list<FinishedOrder> finished_orders;
	};

	class TrackingOrder {
		int signal_id;
		FinishedOrder order;

	public:
		TrackingOrder(int signal_id, const oatpp::Object<ExchangeOrderDto>& exchangeOrder):
			signal_id(signal_id), order(FinishedOrder(exchangeOrder)) {
		}
	};
}