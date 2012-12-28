#ifndef OUTER_SERVICE_H_
#define OUTER_SERVICE_H_
#include "service_engine/service.h"
#include "service_engine/communicator.h"
#include "base/net/connector.h"

namespace proxy
{
namespace outer
{

class OuterService : public service_engine::ServiceT<OuterService>
{
public:
	OuterService(const service_engine::CommandLineParser& parser);
	virtual ~OuterService();

	virtual void run();


private:
	service_engine::Communicator communicator_;

///////////////////////////////////////////////
///////////////////////////////////////////////
///////////////////////////////////////////////
///////////////////////////////////////////////
///////////////////////////////////////////////
///////////////////////////////////////////////


public:
	void registerProxy(const std::string ,boost::weak_ptr<TransferHandler>);
	bool requestProxy(boost::weak_ptr<TransferHandler>&);
public:
	bool registerAgentHandler(boost::shared_ptr<RpcHandler>);
	bool isAgentAvailable();
private:
	//return peer address
	std::string newProxy();
private:
	tbb::concurrent_queue<boost::weak_ptr<TransferHandler> > queue_;
	boost::weak_ptr<RpcHandler> agent_handler_;

};








}}

#endif
