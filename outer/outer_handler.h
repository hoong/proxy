#ifndef OUTER_HANDLER_H_
#define OUTER_HANDLER_H_
#include "../transfer_handler.h"

namespace proxy
{
namespace outer
{





class OuterHandler::public base::packet::PacketHandler
{
public:
	OuterHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl);
	virtual ~OuterHandler();

	virtual void onOpen();
	virtual void onPacketArrive(const Header& header, Packet& body);
};

/*
class OuterHandler:public TransferHandler
{
public:
	OuterHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl);
	virtual ~OuterHandler();
public:

protected:
	boost::shared_ptr<HandlerQueue > sp_queue_;
};
*/

class ExternalHandler:public TransferHandler
{
public:
	ExternalHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl);
	virtual ~ExternalHandler();

	virtual void onOpen();

};

class InternalHandler:public TransferHandler
{
public:
	InternalHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl);
	virtual ~InternalHandler();
	virtual void onOpen();

};


/*
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
*/

}
}
