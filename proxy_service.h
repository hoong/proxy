/*
 * proxy_service.h
 *
 *  Created on: 2012-10-25
 *      Author: root
 */

#ifndef PROXY_SERVICE_H_
#define PROXY_SERVICE_H_

#include "service_engine/service.h"
#include "service_engine/communicator.h"
#include "base/net/connector.h"
#include "proxy_handler.h"

namespace proxy
{

class ProxyService : public service_engine::ServiceT<ProxyService>
{
public:
	ProxyService(const service_engine::CommandLineParser& parser);
	virtual ~ProxyService();

	virtual void run();

	boost::shared_ptr<DstProxyHandler> connect_to_dest();

private:
	service_engine::Communicator communicator_;
	auto_ptr<base::net::Connector> sp_connector_;

	base::net::SockAddr dest_addr_;
};

} /* namespace c2s */
#endif /* C2S_SERVICE_H_ */
