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
	if (getCmdline().hasOption("agent-port"))
		getProperties().set("server.agent_port", getCmdline().getIntOption("agent-port"));
	if (getCmdline().hasOption("external-port"))
		getProperties().set("server.external_port", getCmdline().getIntOption("external-port"));
	if (getCmdline().hasOption("internal-port"))
		getProperties().set("server.internal_port", getCmdline().getIntOption("internal-port"));

	if ( 0 != AgentMethodParam::instance().agent_addr().set(localhost,getProperties().get<int>("server.agent_port")))
	{
		THROW_FATAL_EXCEPTION("agent address invalid!");
	}
	if ( 0 != AgentMethodParam::instance().external_addr().set(localhost,getProperties().get<int>("server.external_por")))
	{
		THROW_FATAL_EXCEPTION("external address invalid!");
	}
	if ( 0 != AgentMethodParam::instance().internal_addr().set(localhost,getProperties().get<int>("server.internal_port")))
	{
		THROW_FATAL_EXCEPTION("internal address invalid!");
	}
}

OuterService::~OuterService()
{
}

void OuterService::run()
{
	// agent service
	boost::shared_ptr<RpcStubsPoolTraits> pool_trait(new service_engine::RpcStubsMultiThreadPoolTraits(2) );
	service_engine::RpcStubsPool stubs_pool(pool_trait);
	service_engine::RpcStubManager stubs_mgr(stubs_pool);
	stubs_mgr.registerStub<agent::MethodAgentRegister>();
	RpcAcceptorT agent_acceptor(AgentMethodParam::instance().agent_addr().getPort(),communicator_,&stubs_mgr);
	
	//internal service
	base::net::Acceptor internal_acceptor(communicator_.reactor());
	auto_ptr<base::net::HandlerCreatorStrategyBase> internal_handler_stg(new base::net::HandlerCreatorStrategy<InternalOuterHandler>());
	internal_acceptor.open(
			AgentMethodParam::instance().internal_addr().getPort(),
			-1,
			internal_handler_stg);


	//external service
	base::net::Acceptor external_acceptor(communicator_.reactor());
	auto_ptr<base::net::HandlerCreatorStrategyBase> external_handler_stg(new base::net::HandlerCreatorStrategy<ExternalOuterHandler>());
	acceptor.open(
			AgentMethodParam::instance().external_addr().getPort(),
			-1,
			external_handler_stg);




	//finally
	communicator_.wait();
}








///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////
///////////////////////////////////////////////////




void ProxyAgent::registerProxy(const std::string addr,boost::weak_ptr<TransferHandler> proxy)
{
	queue_.push(proxy);
}


bool ProxyAgent::registerAgentHandler(boost::shared_ptr<RpcHandler> agent)
{
	boost::shared_ptr<RpcHandler> a = agent_handler_.lock();
	if (!a)
	{
		agent_handler_ = agent;
		return true;
	}
	else
	{
		return false;
	}
}

bool ProxyAgent::requestProxy(boost::weak_ptr<TransferHandler>& proxy)
{
	std::string ret = newProxy();
	if (ret.empty() )
	{
		return false;
	}

	service_engine::RpcProxy<NewProxy> rpc();

	while (queue_.try_pop(proxy) )
	{
		boost::shared_ptr<TransferHandler> peer = proxy.lock();
		if (! peer)
		{
			continue;
		}

		return true;
	}

	return false;

}

std::string ProxyAgent::newProxy()
{
	boost::shared_ptr<service_engine::RpcHandler> agent = agent_handler_.lock();
	std::string ret;
	ret.erase();
	if (!agent)
	{
		LOG(error)<<"Agent service not ready !" <<ENDL;
		return ret;
	}
	else
	{
		try
		{
			service_engine::RpcProxy<NewProxy> rpc(agent);
			NewProxy::RequestType req;
			NewProxy::ResponseType rsp;
			rpc.call(req,rsp,10000);
			ret = rsp.addr();
			return ret;
		}
		catch(RpcCallException& e)
		{
			LOG(error)<<"NewProxy Failed:"<<e.what()<<ENDL;
			return ret;
		}
		catch(...)
		{
			return ret;
		}
	}
}









}}
