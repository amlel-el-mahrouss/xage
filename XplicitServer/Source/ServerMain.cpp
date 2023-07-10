/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include <Http.h>
#include <Root.h>
#include <RoXML.h>
#include <codecvt>
#include <XplicitID.h>

#include "ServerConfig.h"
#include "CommonInclude.h"
#include "SpawnComponent.h"
#include "LoginEvent.h"
#include "TimeoutEvent.h"
#include "MovementEvent.h"
#include "HealthMonitorEvent.h"
#include "ServerReplicationManager.h"

const char* XPLICIT_MANIFEST_FILE = "Manifest.xml";
bool XPLICIT_EXIT_REQUESTED = false;

static void xplicit_print_help()
{
	XPLICIT_INFO("\a+-------------- Xplicit Server Manual --------------+");
	XPLICIT_INFO("exit: Exits the current server.");
	XPLICIT_INFO("xconnect: Get Xconnect network info.");
	XPLICIT_INFO("+-------------- Xplicit Server Manual --------------+");
}

static void xplicit_load_sh()
{
	char cmd_buf[1024];
	const auto network = Xplicit::ComponentSystem::get_singleton_ptr()->get<Xplicit::NetworkServerComponent>("NetworkServerComponent");

	while (Xplicit::ComponentSystem::get_singleton_ptr() && Xplicit::EventSystem::get_singleton_ptr())
	{
		std::cin.getline(cmd_buf, 1024);

		if (strcmp(cmd_buf, "exit") == 0)
		{
			XPLICIT_EXIT_REQUESTED = true;
			break;
		}

		if (strcmp(cmd_buf, "man") == 0)
			xplicit_print_help();

		if (strcmp(cmd_buf, "ls") == 0)
		{
			for (int index = 0; index < network->size(); ++index)
			{
				if (network->get(index)->status == Xplicit::NETWORK_STAT_CONNECTED)
					std::cout << "CONNECTED, ";

				if (network->get(index)->status == Xplicit::NETWORK_STAT_DISCONNECTED)
					std::cout << "DISCONNECTED, ";

				if (network->get(index)->status == Xplicit::NETWORK_STAT_INVALID)
					std::cout << "INVALID, ";

				if (network->get(index)->status == Xplicit::NETWORK_STAT_STASIS)
					std::cout << "IN-CHECK, ";

				std::cout << "INET: " << network->get(index)->ip_address << std::endl;
			}
		}

		if (strcmp(cmd_buf, "xconnect") == 0)
		{
			const char* ip4 = XPLICIT_ENV("XPLICIT_SERVER_ADDR");

			if (!ip4)
				XPLICIT_CRITICAL("xconnect: address is invalid, please define XPLICIT_SERVER_ADDR again in order to be able to reboot the server.");

			XPLICIT_INFO(Xplicit::String("IP: ") + (ip4 ? ip4 : "?"));
			XPLICIT_INFO(Xplicit::String("Version: ") + std::to_string(XPLICIT_NETWORK_VERSION));
		}
	}
}

extern void XplicitLoadServerLua() noexcept;

/* Application main entrypoint */
int main(int argc, char** argv)
{
	try
	{
		if (argc < 2)
		{
			std::cout << "XplicitServer, Self-Hostable XplicitNgin instance.\n";
			std::cout << "usage: XplicitServer.exe <Port> <PlacePath>";
			std::cin.get();

			return 1;
		}

#ifdef XPLICIT_WINDOWS
		WSADATA wsa;
		RtlZeroMemory(&wsa, sizeof(WSADATA));

		Xplicit::init_winsock(&wsa);
#endif

		/*
			The address to bind
			is located in the XPLICIT_SERVER_ADDR
		*/

		const char* ip4 = XPLICIT_ENV("XPLICIT_SERVER_ADDR");

		if (!ip4)
		{
			std::cout << "XplicitServer, Self-Hostable XplicitNgin instance.\n";
			std::cout << "Please define XPLICIT_SERVER_ADDR, it is needed to bound to a specific address.\n";
			std::cin.get();

			return 1;
		}

		const char* port4 = argv[1];

		const auto network = Xplicit::ComponentSystem::get_singleton_ptr()->add<Xplicit::NetworkServerComponent>(ip4, port4);

#ifdef XPLICIT_WINDOWS

		Xplicit::String title = XPLICIT_ENV("XPLICIT_SERVER_ADDR");

		title += ":";
		title += std::to_string(network->port());
		title += " (xconnect v";
		title += std::to_string(XPLICIT_NETWORK_VERSION);
		title += ")";

		::SetConsoleTitleA(title.c_str());

#endif // XPLICIT_WINDOWS

		Xplicit::EventSystem::get_singleton_ptr()->add<Xplicit::HealthMonitorEvent>();
		Xplicit::EventSystem::get_singleton_ptr()->add<Xplicit::TimeoutEvent>();
		Xplicit::EventSystem::get_singleton_ptr()->add<Xplicit::MovementEvent>();
		Xplicit::EventSystem::get_singleton_ptr()->add<Xplicit::LoginEvent>();

		XplicitLoadBaseLua();
		XplicitLoadServerLua();

		XPLICIT_PLACE_ID = argv[2];

		Xplicit::RoXML::RoXMLDocumentParameters params;
		params.Has3D = false;
		params.NoLua = false;
		params.Path = XPLICIT_PLACE_ID;

		Xplicit::RoXML::RoXMLDocumentParser parser;
		parser.load(params);

		Xplicit::Thread logic([&]() {
			const auto net = Xplicit::ComponentSystem::get_singleton_ptr()->get<Xplicit::NetworkServerComponent>("NetworkServerComponent");

			while (Xplicit::ComponentSystem::get_singleton_ptr() &&
				Xplicit::EventSystem::get_singleton_ptr())
			{
				Xplicit::NetworkServerContext::recv_all(net);

				Xplicit::ComponentSystem::get_singleton_ptr()->update();
				Xplicit::EventSystem::get_singleton_ptr()->update();

				Xplicit::NetworkServerContext::send_all(net);
			};
		});

		logic.detach();

		xplicit_load_sh();
		
		return 0;
	}
	catch (const std::runtime_error& err)
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_CRITICAL(err.what());

#endif // ifdef XPLICIT_DEBUG

#ifdef XPLICIT_WINDOWS
		std::wstring exit;

		// UTF-8 to UTF-16 converter which transforms char bytes into wchar_t
		// this is need due to the nature of utf-16 as it takes more bytes to encode a character.
		// also deprecated since c++17 for retarded reasons.

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

		exit += L"What: ";
		exit += converter.from_bytes(err.what());
		exit += L"\n";

		Xplicit::DialogHelper::message_box(L"Xplicit World Server", 
			L"Program Exit", 
			exit.c_str(), 
			TD_INFORMATION_ICON, 
			TDCBF_OK_BUTTON);
#endif // ifdef XPLICIT_WINDOWS

		return -1;
	}
}