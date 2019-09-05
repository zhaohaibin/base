#include "RCF/RCF.hpp"
#include "rpc_init.h"
namespace rpc
{
	static RCF::RcfInit*  pRcfInit = NULL;
	rpc_init::rpc_init()
	{
		if(pRcfInit == NULL)
			pRcfInit = new RCF::RcfInit();
	}

	rpc_init::~rpc_init()
	{
	}
}


