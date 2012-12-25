#ifndef TRANSFER_HANDLER_H_
#define TRANSFER_HANDLER_H_

#include "proxy_handler.h"


namespace proxy
{

class TransferHandler:public ProxyHandler
{

public:
	TransferHandler(int fd, const base::net::SockAddr& addr, boost::shared_ptr<base::net::ReactorImpl> reactor_impl);
	virtual ~TransferHandler();

public:
	virtual void onOpen();
	virtual void onInputData();
	//virtual void onDisconnected();

public:
	void registerPeer(boost::shared_ptr<TransHandler>);
private:
	bool ready_for_transfer_;
	boost::system_time expire_time_;

};


}
