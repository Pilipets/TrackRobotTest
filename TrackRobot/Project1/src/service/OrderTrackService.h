#pragma once

#include "dto/ClientOrderDto.h"

#include <list>
#include <mutex>
using std::mutex;
using std::unordered_map;

class OrderTrackService {
	mutex mx;
	std::list<int> orders;
};