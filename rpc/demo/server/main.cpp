#include "../../rpc.h"
#include <iostream>
using namespace std;
int n = 0;
void test(const rpc::rpc_request& rpcRequest, rpc::rpc_response& rpcResponse)
{
	++n;
	rpcResponse.setContent("OK");
	cout << "number : " << " : " << n<< endl;
}

int main()
{
	rpc::rpc_init rpcInit;
	rpc::rpc_server rpcServer("0.0.0.0", 50001);
	try
	{
		rpc::rpc_service* p_rpcService = new rpc::rpc_service();
		p_rpcService->regFunction("test", std::bind(test, std::placeholders::_1, std::placeholders::_2));
		rpcServer.bindService(*p_rpcService);
		rpcServer.start();
	}
	catch(...)
	{
		printf("Server start error!\n");
	}
	int n = 0;
	std::cin >>n;
	rpcServer.stop();
	return 0;
}