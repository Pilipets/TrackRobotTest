#pragma once

#include "dto/ClientOrderDto.h"

#include <unordered_map>
#include <mutex>
using std::mutex;
using std::unordered_map;

class OrderExecuteService {
	mutex mx;
	std::unordered_map<int, int> signals;
public:
	void acceptOrder(const oatpp::Object<ClientOrderDto>& order);
};