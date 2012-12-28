#include "inner_service.h"
#include "rpc_channel.h"
#include "../transfer_handler.h"

namespace proxy
{
namespace inner
{

InnerService::InnerService(const service_engine::CommandLineParser& parser)
{
	auto_ptr<base::net::HandlerCreatorStrategyBase> handler_stg(new base::net::HandlerCreatorStrategy<TransferHandler>());
	sp_connector_.reset(new base::net::Connector(communicator_.reactor(),
			handler_stg));


	if (getCmdline().hasOption("outer-ip"))
	{
		getProperties().set("server.outer_ip", getCmdline().getStringOption("outer-ip"));
		external_ip_ =  getCmdline().getStringOption("outer-ip");
	}
	if (getCmdline().hasOption("outer-port"))
		getProperties().set("server.outer_port", getCmdline().getIntOption("outer-port"));

	if (getCmdline().hasOption("internal-ip"))
		getProperties().set("server.internal_ip", getCmdline().getStringOption("internal-ip"));
	if (getCmdline().hasOption("internal-port"))
		getProperties().set("server.internal_port", getCmdline().getIntOption("internal-port"));
	if (getCmdline().hasOption("external-ip"))
		getProperties().set("server.external_ip", getCmdline().getStringOption("external-ip"));
	if (getCmdline().hasOption("external-port"))
		getProperties().set("server.external_port", getCmdline().getIntOption("external-port"));



	if (0 != outer_addr_.set(getProperties().get<string>("server.outer_ip", "127.0.0.1").c_str(), getProperties().get<int>("server.outer_port")))
		THROW_FATAL_EXCEPTION("outer address invalid!");

	if (0 != internal_addr_.set(getProperties().get<string>("server.internal_ip", "127.0.0.1").c_str(), getProperties().get<int>("server.internal_port")))
		THROW_FATAL_EXCEPTION("internal address invalid!");

	if (0 != external_addr_.set(getProperties().get<string>("server.external_ip", "127.0.0.1").c_str(), getProperties().get<int>("server.external_port")))
		THROW_FATAL_EXCEPTION("external address invalid!");

}

InnerService::~InnerService()
{
}

void InnerService::run()
{
	boost::shared_ptr<InnerHandler> outer = connectToOuter();
	//finally
	communicator_.wait();
}

boost::shared_ptr<InnerHandler> InnerService::connectToOuter()
{
	auto_ptr<base::net::HandlerCreatorStrategyBase> handler_stg(new base::net::HandlerCreatorStrategy<InnerHandler>());
	base::net::Connector conn(communicator_.reactor(), handler_stg);

	boost::shared_ptr<base::net::RefHandler> outhandler;
	conn->open(outer_addr_, &outhandler);
	return boost::static_pointer_cast<InnerHandler>(outhandler);
}


}}
