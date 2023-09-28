/*
 * =====================================================================
 *
 *			XPXServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include <Http.h>
#include <Root.h>
#include <codecvt>
#include <XplicitID.h>
#include <NpPhysicsEngine.h>

#include "LoginEvent.h"
#include "ServerConfig.h"
#include "TimeoutEvent.h"
#include "CharacterMovementEvent.h"
#include "SpawnComponent.h"
#include "CharacterHealthMonitorEvent.h"

static void XplicitPrintHelp()
{
	XPLICIT_INFO("\a+-------------- XPX Manual --------------+");
	XPLICIT_INFO("exit: Exits the current server.");
	XPLICIT_INFO("xinfo: Gets Xconnect info.");
	XPLICIT_INFO("ls: Lists players.");
	XPLICIT_INFO("+-------------- XPX Manual --------------+");
}

static void XplicitLoadShell()
{
	char cmd_buf[1024];
	const auto network = XPX::ComponentSystem::get_singleton_ptr()->get<XPX::NetworkServerComponent>("NetworkServerComponent");

	while (XPX::ComponentSystem::get_singleton_ptr() && XPX::EventSystem::get_singleton_ptr())
	{
		std::cin.getline(cmd_buf, 1024);

		if (strcmp(cmd_buf, "exit") == 0)
		{

			std::exit(-30);
			break;
		}

		if (strcmp(cmd_buf, "man") == 0)
			XplicitPrintHelp();

		if (strcmp(cmd_buf, "ls") == 0)
		{
			for (int index = 0; index < network->size(); ++index)
			{
				if (network->get(index)->status == XPX::NETWORK_STAT_CONNECTED)
					std::cout << "CONNECTED, ";

				if (network->get(index)->status == XPX::NETWORK_STAT_DISCONNECTED)
					std::cout << "DISCONNECTED, ";

				if (network->get(index)->status == XPX::NETWORK_STAT_INVALID)
					std::cout << "INVALID, ";

				if (network->get(index)->status == XPX::NETWORK_STAT_STASIS)
					std::cout << "IN-CHECK, ";

				std::cout << "INET: " << network->get(index)->ip_address << std::endl;
			}
		}

		if (strcmp(cmd_buf, "xinfo") == 0)
		{
			const char* ip4 = XPLICIT_ENV("XPLICIT_SERVER_ADDR");

			if (!ip4)
				XPLICIT_CRITICAL("xinfo: address is invalid, please define XPLICIT_SERVER_ADDR again in order to be able to reboot the server.");

			XPLICIT_INFO(XPX::String("IPv4: ") + (ip4 ? ip4 : "?"));
			XPLICIT_INFO(XPX::String("Version: ") + std::to_string(XPLICIT_NETWORK_VERSION));
		}
	}
}

/* Application main entrypoint */
int main(int argc, char** argv)
{
	try
	{
		if (argc < 2)
		{
			std::cout << "XPXServer, Self-Hostable XPXNgin instance.\n";
			std::cout << "Usage: XPXServer.exe <Port> <PlacePath>";
			std::cin.get();

			return 1;
		}

#ifdef XPLICIT_WINDOWS
		WSADATA wsa;
		RtlZeroMemory(&wsa, sizeof(WSADATA));

		XPX::init_winsock(&wsa);
#endif

		/*
			The address to bind
			is located in the XPLICIT_SERVER_ADDR
		*/

		const char* ip4 = XPLICIT_ENV("XPLICIT_SERVER_ADDR");

		if (!ip4)
		{
			std::cout << "XPXServer, Self-Hostable XAGE instance.\n";
			std::cout << "Please define XPLICIT_SERVER_ADDR, it is needed to bound to a specific address.\n";
			std::cin.get();

			return -32;
		}

		const char* port4 = argv[1];

		if (!port4)
			return -33;

		const auto network = XPX::ComponentSystem::get_singleton_ptr()->add<XPX::NetworkServerComponent>(ip4, port4);

		XPX::ComponentSystem::get_singleton_ptr()->add<XPX::SpawnComponent>(XPLICIT_ORIGIN);

		XPX::EventSystem::get_singleton_ptr()->add<XPX::CharacterMovementEvent>();
		XPX::EventSystem::get_singleton_ptr()->add<XPX::NpPhysicsEvent>();
		XPX::EventSystem::get_singleton_ptr()->add<XPX::CharacterHealthMonitorEvent>();
		XPX::EventSystem::get_singleton_ptr()->add<XPX::TimeoutEvent>();
		XPX::EventSystem::get_singleton_ptr()->add<XPX::LoginEvent>();

#ifdef XPLICIT_WINDOWS

		XPX::String title = XPLICIT_ENV("XPLICIT_SERVER_ADDR");

		title += ":";
		title += std::to_string(network->port());
		title += " (xconnect v";
		title += std::to_string(XPLICIT_NETWORK_VERSION);
		title += ")";

		SetConsoleTitleA(title.c_str());

#endif // XPLICIT_WINDOWS

		NPLICIT_SPLASH_SCREEN;

		XPX::Thread job(XplicitLoadShell);

		while (XPX::ComponentSystem::get_singleton_ptr() &&
			XPX::EventSystem::get_singleton_ptr())
		{
			XPX::NetworkServerContext::recv_all(network);

			XPX::ComponentSystem::get_singleton_ptr()->update();
			XPX::EventSystem::get_singleton_ptr()->update();

			XPX::Lua::CLuaStateManager::get_singleton_ptr()->run_string("world:RenderOneFrame()");

			XPX::NetworkServerContext::send_all(network);
		};

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

		exit += L"WHAT: ";
		exit += converter.from_bytes(err.what());
		exit += L"\n";

		XPX::DialogHelper::message_box(L"XAGE lobby server", 
			L"XAGGE crashed!",
			exit.c_str(), 
			TD_INFORMATION_ICON, 
			TDCBF_OK_BUTTON);
#endif // ifdef XPLICIT_WINDOWS

		return -1;
	}
}