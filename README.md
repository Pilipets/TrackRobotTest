Comments for the project.

Details:
1. Stack: C++14, oat++
2. Console logging.
3. Manual testing via "beeceptor"(mock server), postman(mock client).
4. ThreadPools in ActiveOrderService, TrackOrderService which work with coroutines - AddOrderCoroutine, UpdateOrderCoroutine, UpdateSignalCoroutine.
5. ExchangeApiClient interface and component management logic which are similar to Spring Feign, Spring Boot.
6. Compiled static "oatpp" libraries can be added upon request.

Flow:
1. A client posts order to the client/order endpoint -> ClientController::setNewOrder -> ActiveOrderService accepts the order -> ActiveOrderService::acceptOrder -> ActiveOrderService posts the order to the exchange market -> ExchangeApiClient::postOrder -> if an error occurs, returns to the client, otherwise proccesses the order asynchronously -> AddOrderCoroutine::act -> Creates the signal with ActiveOrderService::addSignal and adds order for tracking OrderTrackService::addOrder;
2. A robot asynchronously updates the order execution statuses and signals -> TrackOrderService::updateOrders -> take next k orders from the queue using ExchangeApiClient::getOrder and request updates with UpdateOrderCoroutine::act -> if changed, TrackOrderService::updateOrder + ActiveOrderService::updateSignal...
 

Potential upgrades:
1. Unhandled exceptions in coroutines might lead to missing the track of the order - left untouched at the moment.
2. Replace sync API calls with async through combining coroutines.
3. Add logging to the file, DB.
4. Use Object when it's required as it creates performance overhead with shared_ptr.
5. Add tests using oatpp-test.