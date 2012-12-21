/*
 * proxy_handler.h
 *
 *  Created on: 2012-11-5
 *      Author: root
 */

#ifndef PROXY_HANDLER_H_
#define PROXY_HANDLER_H_

#include "base/net/buffered_handler.h"

namespace proxy
{

class ProxyHandler : public base::net::BufferedHandler
{
public:
	ProxyHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl);
	virtual ~ProxyHandler();

public:
	virtual void onOpen();
	virtual void onInputData();
	virtual void onOutputData();
	virtual void onDisconnected();


private:
	void flushAndClose();

protected:
	boost::weak_ptr<ProxyHandler> peer_handler_;
	bool flush_and_close_;

};


class SrcProxyHandler : public ProxyHandler
{

public:
	SrcProxyHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl);
	virtual ~SrcProxyHandler();

public:
	virtual void onOpen();
};


class DstProxyHandler : public ProxyHandler
{
	friend class SrcProxyHandler;
public:
	DstProxyHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl);
	virtual ~DstProxyHandler();

};




} /* namespace proxy */
#endif /* PROXY_HANDLER_H_ */
