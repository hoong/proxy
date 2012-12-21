/*
 * proxy_handler.cpp
 *
 *  Created on: 2012-11-5
 *      Author: root
 */

#include "proxy_handler.h"
#include "proxy_service.h"

namespace proxy
{

ProxyHandler::ProxyHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl)
: base::net::BufferedHandler(fd, addr, reactor_impl),
  flush_and_close_(false)
{
}

ProxyHandler::~ProxyHandler()
{
}

void ProxyHandler::onOpen()
{
	LOG(info) << "proxy handler open: " << addr() << ENDL;
}

void ProxyHandler::onInputData()
{
	boost::shared_ptr<ProxyHandler> peer = peer_handler_.lock();
	if (!peer)
	{
		close();
		return;
	}

	base::net::Buffer buf;
	{
		mutext_t::scoped_lock lock(inbuf_mutex_);
		inbuf_.getDataAtMost(buf);
	}
	try
	{
		peer->write(buf);
	}
	catch (...)
	{
	}
}

void ProxyHandler::onDisconnected()
{
	LOG(info) << "proxy handler disconnected: " << addr() << ENDL;

	boost::shared_ptr<ProxyHandler> peer = peer_handler_.lock();
	if (peer)
	{
		peer->flushAndClose();
	}

	{
		mutext_t::scoped_lock lock(inbuf_mutex_);
		if (!inbuf_.empty())
		{
			LOG(error) << "left data in recv buffer: " << inbuf_ << ENDL;
		}
	}
	{
		mutext_t::scoped_lock lock(outbuf_mutex_);
		if (!outbuf_.empty())
		{
			LOG(error) << "left data in send buffer: " << outbuf_ << ENDL;
		}
	}
}

void ProxyHandler::onOutputData()
{
	mutext_t::scoped_lock lock(outbuf_mutex_);
	if (flush_and_close_ && outbuf_.empty())
	{
		close();
	}
}

void ProxyHandler::flushAndClose()
{
	mutext_t::scoped_lock lock(outbuf_mutex_);
	if (outbuf_.empty())
	{
		close();
		return;
	}

	flush_and_close_ = true;

}


SrcProxyHandler::SrcProxyHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl)
: ProxyHandler(fd, addr, reactor_impl)
{
}

SrcProxyHandler::~SrcProxyHandler()
{
}

void SrcProxyHandler::onOpen()
{
	ProxyHandler::onOpen();

	boost::shared_ptr<DstProxyHandler> spdst = ProxyService::instance()->connect_to_dest();

	peer_handler_ = spdst;
	spdst->peer_handler_ = boost::static_pointer_cast<ProxyHandler>(this->shared_from_this());
}


DstProxyHandler::DstProxyHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl)
: ProxyHandler(fd, addr, reactor_impl)
{
}

DstProxyHandler::~DstProxyHandler()
{
}


} /* namespace c2s */
