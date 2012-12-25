#ifndef HANDLER_QUEUE_H_
#define HANDLER_QUEUE_H_

#include "../TransferHandler.h"
#include "RpcHandler.h"
#include <tbb/concurrent_queue.h>
namespace proxy
{
namespace outer
{

class HandlerQueue
{
public:
	void push(boost::weak_ptr<TransferHandler>);
	bool pop(boost::weak_ptr<TransferHandler>);
	void registerAgent(boost::weak_ptr<RpcHandler> handler);
private:
	void requestNewConnection();
private:
	tbb::concurrent_queue<boost::weak_ptr<TransferHandler> > queue_;
	boost::weak_ptr<RpcHandler> agent_handler_;
};

}
}



#endif
