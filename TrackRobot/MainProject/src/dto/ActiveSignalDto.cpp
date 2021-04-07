#include "ActiveSignalDto.h"

ActiveSignalDto::ActiveSignalDto(
	int signal_id,
	List<Object<ExecutedOrderDto>> finished_orders):
	signal_id(signal_id), active_quantity(0), finished_orders(finished_orders) {

}

void ActiveSignalDto::addFinished(Object<ExecutedOrderDto>&& order) {
	finished_orders->push_back(std::move(order));
}
