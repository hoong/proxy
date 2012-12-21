/*
 * proxy_service.cpp
 *
 *  Created on: 2012-10-25
 *      Author: root
 */

#include "proxy_service.h"
#include "proxy_handler.h"
#include "base/logger.h"
#include "base/exception.h"
#include "base/net/acceptor.h"

namespace proxy
{

ProxyService::ProxyService(const service_engine::CommandLineParser& parser)
: service_engine::ServiceT<ProxyService>(parser)
{
	auto_ptr<base::net::HandlerCreatorStrategyBase> handler_stg(new base::net::HandlerCreatorStrategy<DstProxyHandler>());
	sp_connector_.reset(new base::net::Connector(communicator_.reactor(),
			handler_stg));

	if (getCmdline().hasOption("listen-port"))
		getProperties().set("server.listen_port", getCmdline().getIntOption("listen-port"));
	if (getCmdline().hasOption("dest-ip"))
		getProperties().set("server.dest_ip", getCmdline().getStringOption("dest-ip"));
	if (getCmdline().hasOption("dest-port"))
		getProperties().set("server.dest_port", getCmdline().getIntOption("dest-port"));

	if (0 != dest_addr_.set(getProperties().get<string>("server.dest_ip", "127.0.0.1").c_str(), getProperties().get<int>("server.dest_port")))
		THROW_FATAL_EXCEPTION("dest address invalid!");
}

ProxyService::~ProxyService()
{
}

boost::shared_ptr<DstProxyHandler> ProxyService::connect_to_dest()
{
	boost::shared_ptr<base::net::RefHandler> outhandler;
	sp_connector_->open(dest_addr_, &outhandler);
	return boost::static_pointer_cast<DstProxyHandler>(outhandler);
}

void ProxyService::run()
{
	base::net::Acceptor acceptor(communicator_.reactor());
	auto_ptr<base::net::HandlerCreatorStrategyBase> handler_stg(new base::net::HandlerCreatorStrategy<SrcProxyHandler>());
	acceptor.open(
			getProperties().get<int>("server.listen_port"),
			-1,
			handler_stg);


	communicator_.wait();
}

} /* namespace c2s */
