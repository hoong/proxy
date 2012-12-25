#include "outer_handler.h"


namespace proxy 
{
namespace outer 
{

OuterHandler::OuterHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl,boost::shared_ptr<HandlerQueue> queue)
	:TransferHandler(fd,addr,reactor_impl),sp_queue_(queue)
{
}

OuterHandler::~OuterHandler()
{
}

void OuterHandler::onOpen()
{
	TransferHandler::onOpen();

	/*
	//TO-DO

	*/

}

ExternalOuterHandler::ExternalOuterHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl,boost::shared_ptr<HandlerQueue> queue)
	:OuterHandler(fd,addr,reactor_impl,queue)
{
}

ExternalOuterHandler::~ExternalOuterHandler()
{
}

void ExternalOuterHandler::onOpen()
{
	boost::weak_ptr<TransferHandler> peer = boost::static_pointer_cast<TransferHandler>(this->shared_from_this() );
	sp_queue_->push(peer);
}


InternalOuterHandler::InternalOuterHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl,boost::shared_ptr<HandlerQueue> queue)
	:OuterHandler(fd,addr,reactor_impl,queue)
{
}

InternalOuterHandler::~InternalOuterHandler()
{
}

void InternalOuterHandler::onOpen()
{
	boost::weak_ptr<TransferHandler> peer;
	while(sp_queue_->pop(peer))
	{
		boost::shared_ptr<TransferHandler> spto = peer->lock();
		if (!spto)
		{
			continue;
		}
		else
		{
			registerPeer(spto);
			return;
		}
	}

	close();
}


}
}
