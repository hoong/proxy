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
	base::net::SockAddr outer_addr_;
	base::net::SockAddr external_addr_;
	base::net::SockAddr internal_addr_;

public:
	bool putProxy(boost::shared_ptr<TransferHandler>);
	bool popProxy(boost::shared_ptr<TransferHandler>&);

public:
	void registerOuterHandler(boost::shared_ptr<OuterHandler>);

private:
	base::ConcurrentQueue< boost::weak_ptr<TransferHandler> > handler_queue_;

	boost::weak_ptr<OuterHandler> outer_handler_;
};








}}

#endif
