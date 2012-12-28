#include "outer_handler.h"
#include "../agent/proxy_agent.h"


namespace proxy 
{
namespace outer 
{
/*
OuterHandler::OuterHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl)
	:TransferHandler(fd,addr,reactor_impl)
{
}

OuterHandler::~OuterHandler()
{
}

void OuterHandler::onOpen()
{
	TransferHandler::onOpen();

	//TO-DO


}
*/

ExternalOuterHandler::ExternalOuterHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl)
	:TransferHandler(fd,addr,reactor_impl)
{
}

ExternalOuterHandler::~ExternalOuterHandler()
{
}

void ExternalOuterHandler::onOpen()
{
	OuterHandler::onOpen();
	boost::weak_ptr<TransferHandler> peer;
	if (PROXY_AGENT::instance().requestProxy(peer) )
	{
		registerPeer(peer);
	}
	else
	{
		close();
	}


	/*
	boost::weak_ptr<TransferHandler> peer = boost::static_pointer_cast<TransferHandler>(this->shared_from_this() );
	sp_queue_->push(peer);
	*/
}


InternalOuterHandler::InternalOuterHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl)
	:TransferHandler(fd,addr,reactor_impl)
{
}

InternalOuterHandler::~InternalOuterHandler()
{
}

void InternalOuterHandler::onOpen()
{
	OuterHandler::onOpen();
	boost::weak_ptr<TransferHandler> peer = boost::static_pointer_cast<TransferHandler>(this->shared_from_this());
	PROXY_AGENT::instance().registerProxy(addr().toString(),peer);

	/*
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
	*/
}


}
}
