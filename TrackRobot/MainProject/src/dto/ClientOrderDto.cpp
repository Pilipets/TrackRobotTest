#include "ClientOrderDto.h"

ClientOrderDto::ClientOrderDto(int8_t side, float price, int quantity, int signal_id):
	BasicOrderDto(side, price, quantity), signal_id(signal_id) {
}
