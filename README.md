Comments for the project.

Dependencies:
1. C++14.
2. https://github.com/oatpp/oatpp
3. Compiled Debug x64 with VisualStudio/16.9.3+31129.286.

Details:
1. Console logging.
2. Manual testing via "beeceptor"(mock server), postman(mock client).
3. ThreadPools in ActiveOrderService, TrackOrderService which work with coroutines - AddOrderCoroutine, UpdateOrderCoroutine, UpdateSignalCoroutine.
4. ExchangeApiClient interface and component management logic which are similar to Spring Feign, Spring Boot.
5. Compiled static "oatpp" libraries can be added upon request.

Flow:
FLOW WAS CHANGED to update executions as soon as possible!!!
1. A client posts order to the client/order endpoint -> ClientController::setNewOrder -> ActiveOrderService accepts the order -> ActiveOrderService::acceptOrder -> ActiveOrderService posts the order to the exchange market -> ExchangeApiClient::postOrder -> if an error occurs, returns to the client, otherwise proccesses the order asynchronously -> AddOrderCoroutine::act -> Creates the signal with ActiveOrderService::addSignal and adds order for tracking OrderTrackService::addOrder;
2. A robot asynchronously updates the order execution statuses and signals -> TrackOrderService::updateOrders -> take next k orders from the queue using ExchangeApiClient::getOrder and request updates with UpdateOrderCoroutine::act -> if changed, TrackOrderService::updateOrder + ActiveOrderService::updateSignal...
3. Open .sln file in Visual Studio 2019, and click Build, or convert the VS project to CMakeLists.txt :)
4. On Windows run .exe from TrackRobot/x64/Debug.

Potential upgrades:
1. Add event dispatcher system to remove strong coupling between components.
2. Unhandled exceptions in coroutines might lead to missing the track of the order - left untouched at the moment.
3. Replace sync API calls with async through combining coroutines.
4. Add logging to the file, DB.
5. Use Object when it's required as it creates performance overhead with shared_ptr.
6. Add tests using oatpp-test.
7. Add configuration file.