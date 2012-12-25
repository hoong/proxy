#ifndef OUTER_HANDLER_H_
#define OUTER_HANDLER_H_
#include "../transfer_handler.h"
#include "handler_queue.h"

namespace proxy
{
namespace outer
{

class OuterHandler:public TransferHandler
{
public:
	OuterHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl,boost::shared_ptr<HandlerQueue> queue);
	virtual ~OuterHandler();
public:

protected:
	boost::shared_ptr<HandlerQueue > sp_queue_;
};

class ExternalOuterHandler:public OuterHandler
{
public:
	virtual ~ExternalOuterHandler();

	virtual void onOpen();

};

class InternalOuterHandler:public OuterHandler
{
public:
	virtual ~ExternalOuterHandler();
	virtual void onOpen();

};


template <typename T>
class OuterHandlerCreatorStrategy: public HandlerCreatorStrategyBase
{
public:
	OuterHandlerCreatorStrategy(boost::shared_ptr<HandlerQueue> queue ):sp_queue_(queue) {}
	virtual ~OuterHandlerCreatorStrategy() {}

	virtual boost::shared_ptr<RefHandler> create(int fd, const SockAddr& addr, boost::shared_ptr<ReactorImpl> reactor_impl)
	{
		return boost::shared_ptr<RefHandler>(new T(fd,addr,reactor_impl,sp_queue_));
	}

private:
	boost::shared_ptr<HandlerQueue > sp_queue_;
};

}
}
