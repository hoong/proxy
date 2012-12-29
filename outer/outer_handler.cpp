#include "outer_handler.h"
#include "outer_service.h"


namespace proxy 
{
namespace outer 
{

OuterHandler::OuterHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl)
	:base::packet::PacketHandler(fd,addr,reactor_impl)
{
}

OuterHandler::~OuterHandler()
{
}

void Outerhandler::onOpen()
{
	OuterService::instance()->registerOuterHandler(this->shared_from_this());
}

void Outerhandler::onPacketArrive(const Header& header, Packet& body)
{
	LOG(trace)<<"Receive Response."<<ENDL;
}


ExternalHandler::ExternalHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl)
	:TransferHandler(fd,addr,reactor_impl)
{
}

ExternalHandler::~ExternalHandler()
{
}

void ExternalHandler::onOpen()
{

	OuterHandler::onOpen();

	if ( !OuterService::instance()->putProxy(boost::dynamic_pointer_cast<TransferHandler>(this->shared_from_this())) )
	{
		LOG(error)<<"Inner Service Not Available!"<<ENDL;
		close();
	}

}


InternalHandler::InternalHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl)
	:TransferHandler(fd,addr,reactor_impl)
{
}

InternalHandler::~InternalHandler()
{
}

void InternalHandler::onOpen()
{
	OuterHandler::onOpen();


	boost::shared_ptr<Transferhandler> peer ; 
	if (!OuterService::instance()->popProxy(peer) )
	{
		close();
		return;
	}
	registerPeer(peer);

}





}
}
