#ifndef INNER_AGENT_HANDLER_H_
#define INNER_AGENT_HANDLER_H_

#include "../transfer_handler.h"
#include "packet/packet_handler.h"

namespace proxy
{
namespace inner
{

class InnerHandler:public base::packet::PacketHandler
{
public:
	InnerHandler(int fd, const SockAddr& addr, boost::shared_ptr<ReactorImpl> reactor_impl)
		:base::packet::PacketHandler(fd,addr,reactor_impl) {}
	virtual ~InnerHandler(){}
	virtual void onPacketArrive(const Header& header, Packet& body);

};



}}
