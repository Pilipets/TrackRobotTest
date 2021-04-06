#pragma once

#include <list>
#include <mutex>
using std::mutex;

class OrderTrackService {
	mutex mx;
	std::list<int> orders;
};