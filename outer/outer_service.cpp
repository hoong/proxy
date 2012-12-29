#include "outer_service.h"
#include "outer_handler.h"


namespace proxy
{
namespace outer
{


OuterService::OuterService(const service_engine::CommandLineParser& parser)
: service_engine::ServiceT<ProxyService>(parser)
{
	std::string localhost = "127.0.0.1";
	if (getCmdline().hasOption("outer-port"))
		getProperties().set("server.outer_port", getCmdline().getIntOption("outer-port"));
	if (getCmdline().hasOption("external-port"))
		getProperties().set("server.external_port", getCmdline().getIntOption("external-port"));
	if (getCmdline().hasOption("internal-port"))
		getProperties().set("server.internal_port", getCmdline().getIntOption("internal-port"));

	if ( 0 != outer_addr_.set(localhost,getProperties().get<int>("server.outer_port")))
	{
		THROW_FATAL_EXCEPTION("outeraddress invalid!");
	}
	if ( 0 != external_addr_.set(localhost,getProperties().get<int>("server.external_por")))
	{
		THROW_FATAL_EXCEPTION("external address invalid!");
	}
	if ( 0 != internal_addr_.set(localhost,getProperties().get<int>("server.internal_port")))
	{
		THROW_FATAL_EXCEPTION("internal address invalid!");
	}
}

OuterService::~OuterService()
{
}

void OuterService::run()
{
	//outer service
	base::net::Acceptor outer_acceptor(communicator_.reactor());
	auto_ptr<base::net::HandlerCreatorStrategyBase> outer_handler_stg(new base::net::HandlerCreatorStrategy<OuterHandler>());
	internal_acceptor.open(
			outer_addr_.getPort(),
			-1,
			outer_handler_stg);

	
	//internal service
	base::net::Acceptor internal_acceptor(communicator_.reactor());
	auto_ptr<base::net::HandlerCreatorStrategyBase> internal_handler_stg(new base::net::HandlerCreatorStrategy<InternalHandler>());
	internal_acceptor.open(
			internal_addr_.getPort(),
			-1,
			internal_handler_stg);


	//external service
	base::net::Acceptor external_acceptor(communicator_.reactor());
	auto_ptr<base::net::HandlerCreatorStrategyBase> external_handler_stg(new base::net::HandlerCreatorStrategy<ExternalHandler>());
	external_acceptor.open(
			external_addr_.getPort(),
			-1,
			external_handler_stg);


	//finally
	communicator_.wait();
}



bool OuterService::putProxy(boost::shared_ptr<TransferHandler> proxy)
{
	boost::shared_ptr<OuterHandler> outer = outer_handler_.lock();
	if (!outer || outer->isClose())
	{
		return false;
	}

	handler_queue_.push(proxy);

	base::packet::Header hdr;
	const uint32_t whatever = 0;
	outer_handler_->write(hdr,whatever);
	
	return true;
}



bool OuterService::popProxy(boost::shared_ptr<TransferHandler>& proxy)
{
	boost::weak_ptr<TransferHandler> peer;
	
	while(handler_queue_.try_pop(peer))
	{
		proxy = peer.lock();
		if (!proxy)
			continue;
		else
			return true;
	}

	return false;

}


void OuterService::registerOuterHandler(boost::shared_ptr<OuterHandler> peer)
{
	boost::shared_ptr<OuterHandler> a = outer_handler_.lock();
	if (a.get())
	{
		a->close();
	}
	outer_handler_=peer;
}


}}
