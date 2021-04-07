#include "ExchangeOrderDto.h"

// normally it will be uuid
int ExchangeOrderDto::cnt = 0;

ExchangeOrderDto::ExchangeOrderDto(bool side, float price, int quantity) :
	BasicOrderDto(side, price, quantity), order_id(cnt++) {
}
