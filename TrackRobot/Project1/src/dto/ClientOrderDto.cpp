#include "ClientOrderDto.h"

ClientOrderDto::ClientOrderDto(bool side, float price, int quantity, int signal_id):
	BasicOrderDto(side, price, quantity), signal_id(signal_id) {
}
