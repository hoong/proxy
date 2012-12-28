/*
 * main.cpp
 *
 *  Created on: 2012-11-2
 *      Author: root
 */

#include "base/logger.h"
#include "base/exception.h"
#include "proxy_service.h"
#include "outer/outer_service.h"
#include "inner/inner_service.h"

int main(int argc, char* argv[])
{
	try
	{
		service_engine::CommandLineParser service_chooser_cmdline;
		service_chooser_cmdline.addStringOption("service-type","outer:outer service,inner:inner service,proxy:proxy service");
		std::string service_type;
		if(service_chooser_cmdline.parse(argc,argv).hasOption("service-type") )
			service_type = service_chooser_cmdline.getStringOption("service-type");

		if (service_type == "outer")
		{
			service_engine::CommandLineParser cmdline;
			cmdline.addIntOption("agent-port", "agent port");
			cmdline.addIntOption("external-port", "external port");
			cmdline.addIntOption("internal-port", "internal port");

			proxy::outer::OuterService s(cmdline.parse(argc, argv));

			LOG(info) << "\n\n====== outer proxy server start ======\n\n";

			s.run();
		}
		else if (service_type == "inner")
		{
			service_engine::CommandLineParser cmdline;
			cmdline.addStringOption("agent-ip", "agent ip");
			cmdline.addIntOption("agent-port", "agent port");
			cmdline.addStringOption("internal-ip", "internal port");
			cmdline.addIntOption("internal-port", "internal port");

			proxy::inner::InnerService s(cmdline.parse(argc, argv));

			LOG(info) << "\n\n====== outer proxy server start ======\n\n";

			s.run();
		}
		else
		{
			service_engine::CommandLineParser cmdline;
			cmdline.addIntOption("listen-port", "listen port");
			cmdline.addStringOption("dest-ip", "dest ip");
			cmdline.addIntOption("dest-port", "dest port");

			proxy::ProxyService s(cmdline.parse(argc, argv));

			LOG(info) << "\n\n====== server start ======\n\n";

			s.run();
		}

	}
	catch (exception& e)
	{
		LOG(error) << EXCEPTION_DIAG_INFO(e) << ENDL;
	}

	LOG(info) << "exit.";

	return 0;
}


