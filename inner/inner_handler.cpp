#include "inner_handler.h"
#include "inner_service.h"

namespace proxy
{
namespace inner
{


void InnerHandler::onPacketArrive(const Header& header, Packet& body)
{
	uint32_t external_local_port = 0;
	boost::shared_ptr<TransferHandler> internal = InnerService::instance()->connectToInternal();
	boost::shared_ptr<TransferHandler> external = InnerService::instance()->connectToExternal();
	if (!internal || !external)
	{
		write(header,external_local_port);
		return;
	}
	external.registerPeer(internal);
	struct sockaddr sock;
	int sock_len;
	::getsockname(external->fd(),(struct sockaddr*)&sock,&sock_len);
	base::net::SockAddr addr(sock);
	external_local_port = addr.getPort();

	write(header,external_local_port);
}







}}
