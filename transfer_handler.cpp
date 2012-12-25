#include "transfer_handler.h"



namespace proxy
{

const uint32_t wait_time_ = 120;

TransferHandler::TransferHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl)
	:ProxyHandler(fd,addr,reactor_impl),ready_for_transfer_(false)
{

}

TransferHandler::~TransferHandler()
{

}

void TransferHandler::onOpen()
{
	LOG(info) << "transfer handler open: " << addr() << ENDL;
	expire_time = boost::get_system_time() + wait_time_ ;
}

void TransferHandler::onInputData()
{
	if (ready_for_transfer_)
	{
		ProxyHandler::onInputData();
	}
	else
	{
		if (expire_time >= boost::get_system_time() )
		{
			ProxyHandler::flushAndClose();
		}
	}
}

void TransferHandler::registerPeer(boost::shared_ptr<TransferHandler> peer)
{
	peer_handler_ = boost::static_pointer_cast<ProxyHandler>(peer);
	ready_for_transfer_ = true;

	peer->peer_handler_ = boost::static_pointer_cast<ProxyHandler>(this->shared_from_this());
	peer->ready_for_transfer_ = true;

	onInputData();
	peer->onInputData();
}

}
