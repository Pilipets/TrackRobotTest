#include "ExchangeOrderDto.h"

// normally it will be uuid
uint32_t ExchangeOrderDto::cnt = 0;

ExchangeOrderDto::ExchangeOrderDto(int8_t side, float price, int quantity) :
	BasicOrderDto(side, price, quantity), order_id(cnt++) {
}
