#include "handler_queue.h"


namespace proxy
{
namespace outer
{
void HandlerQueue::push(boost::weak_ptr<TransferHandler> peer)
{
	queue_->push(peer);	
	requestNewConnection();
}

bool HandlerQueue::push(boost::weak_ptr<TransferHandler>& peer)
{
	return queue_->pop(peer);
}

void HandlerQueue::registerAgent(boost::weak_ptr<RpcHandler> handler)
{
	agent_handler_ = handler;
}

}
}
