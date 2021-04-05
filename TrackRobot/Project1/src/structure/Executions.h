#pragma once

#include <cstdint>
#include <vector>
using namespace std;

class Execution {
	int execution_id;
	int order_id;
	int8_t side;
	int execution_price;
	int quantity;
};


class TrackingOrder {
	int active_quantity;
	int signal_id;
	vector<Execution> executions;
};