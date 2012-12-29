#ifndef PROXY_INNER_SERVICE_H_
#define PROXY_INNER_SERVICE_H_
#include "net/sock.h"


namespace proxy
{
namespace inner
{

class InnerService : public service_engine::ServiceT<InnerService>
{
public:
	InnerService(const service_engine::CommandLineParser& parser);
	virtual ~InnerService();

	virtual void run();

	boost::shared_ptr<InnerHandler> connectToOuter();
	boost::shared_ptr<TransferHandler> connectToExternal();
	boost::shared_ptr<TransferHandler> connectToInternal();
private:
	boost::shared_ptr<TransferHandler> connectToProxy(const base::net::SockAddr);

private:
	service_engine::Communicator communicator_;

	base::net::SockAddr outer_addr_;
	base::net::SockAddr external_addr_;
	base::net::SockAddr internal_addr_;

	boost::shared_ptr<base::net::Connector> sp_connector;
};




}}
#endif
