/*
 * main.cpp
 *
 *  Created on: 2012-11-2
 *      Author: root
 */

#include "base/logger.h"
#include "base/exception.h"
#include "proxy_service.h"

int main(int argc, char* argv[])
{
	try
	{
		service_engine::CommandLineParser cmdline;
		cmdline.addIntOption("listen-port", "listen port");
		cmdline.addStringOption("dest-ip", "dest ip");
		cmdline.addIntOption("dest-port", "dest port");

		proxy::ProxyService s(cmdline.parse(argc, argv));

		LOG(info) << "\n\n====== server start ======\n\n";

		s.run();

	}
	catch (exception& e)
	{
		LOG(error) << EXCEPTION_DIAG_INFO(e) << ENDL;
	}

	LOG(info) << "exit.";

	return 0;
}


