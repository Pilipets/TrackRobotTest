#include "ActiveSignalDto.h"

ActiveSignalDto::ActiveSignalDto(int signal_id):
	signal_id(signal_id), active_quantity(0) {

}

void ActiveSignalDto::addFinished(Object<ExecutedOrderDto>&& order) {
	finished_orders->push_back(std::move(order));
}
