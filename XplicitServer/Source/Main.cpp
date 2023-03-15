/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: Server.cpp
 *			Purpose: Server Main Procedure
 *
 * =====================================================================
 */

#include "SDK.h"

#include "Actor.h"
#include "ServerWatchdog.h"
#include "PlayerJoinLeaveEvent.h"

static void xplicit_send_stop_packet(Xplicit::NetworkServerInstance* server);
static void xplicit_attach_mono();
static void xplicit_load_shell();
static void xplicit_read_xml();

static void xplicit_read_xml()
{
	XPLICIT_GET_DATA_DIR(data);

	std::string path = data;
	path += "\\Server.xml";

	rapidxml::file<> xml{ path.c_str() };
	XPLICIT_ASSERT(xml.size() > 0);

	auto mono = Xplicit::InstanceManager::get_singleton_ptr()->get<Xplicit::MonoEngineInstance>("MonoEngineInstance");

	const int argc = 2;
	const char* argv[] = { "XplicitNgin", "1.0.0" };

	rapidxml::xml_document<> doc{};
	doc.parse<0>(xml.data());

	auto node = doc.first_node();

	while (node)
	{
		if (strcmp(node->name(), "Plugin") == 0)
		{
			auto dll = node->value();

			path.clear();

			path += data;
			path += "\\Lib\\";
			path += dll;

			auto csharp_dll = Xplicit::InstanceManager::get_singleton_ptr()->add<Xplicit::MonoScriptInstance>(path.c_str(), false);

			auto assembly_file = mono->open(path.c_str());

			if (assembly_file)
			{
				XPLICIT_INFO("[C#] " + path + " Loaded.");
				mono->run(assembly_file, argc, argv);
			}
			else
			{
				XPLICIT_INFO("[C#] " + path + " Was found found.");
				Xplicit::InstanceManager::get_singleton_ptr()->remove(csharp_dll);
			}
		}

		if (node->parent())
			node = node->next_sibling();
		else
			break;
	}
}

static void xplicit_attach_mono()
{
	XPLICIT_GET_DATA_DIR(data);

	std::string path = data;
	path += "\\Lib\\Xplicit.dll"; // The game dll.

	Xplicit::InstanceManager::get_singleton_ptr()->add<Xplicit::MonoEngineInstance>();
	Xplicit::InstanceManager::get_singleton_ptr()->add<Xplicit::MonoScriptInstance>(path.c_str(), false);
}

static void xplicit_print_help()
{
	puts("\a-------------- HELP --------------");
	puts("stop: Stops and exits the server.");
	puts("-------------- HELP --------------");
}

static void xplicit_load_shell()
{
	std::thread shell(
		[]() -> void
		{
			char cmd_buf[1024];

			while (Xplicit::InstanceManager::get_singleton_ptr() && Xplicit::EventDispatcher::get_singleton_ptr())
			{
				if (!Xplicit::ApplicationContext::get_singleton().ShouldExit)
					std::cout << "# ";

				std::cin.getline(cmd_buf, 1024);

				if (strcmp(cmd_buf, "stop") == 0)
				{
					auto server = Xplicit::InstanceManager::get_singleton_ptr()->get<Xplicit::NetworkServerInstance>("NetworkServerInstance");
					XPLICIT_ASSERT(server);

					xplicit_send_stop_packet(server);
					Xplicit::NetworkServerTraits::send(server);

					Xplicit::ApplicationContext::get_singleton().ShouldExit = true;

				}

				if (strcmp(cmd_buf, "help") == 0)
				{
					xplicit_print_help();
				}

			}
		}
	);

	shell.detach();
}

static void xplicit_send_stop_packet(Xplicit::NetworkServerInstance* server)
{
	XPLICIT_ASSERT(server);

	for (size_t i = 0; i < server->size(); i++)
	{
		server->get(i)->packet.cmd[XPLICIT_NETWORK_CMD_STOP] = Xplicit::NETWORK_CMD_STOP;
	}
}

// our main entrypoint.
int main(int argc, char** argv)
{
	try
	{
		WSADATA wsa;
		Xplicit::init_winsock(&wsa);

		// the address to bind to is located in the XPLICIT_SERVER_ADDR environement variable.
		const char* ip_address = XPLICIT_ENV("XPLICIT_SERVER_ADDR");

		if (!ip_address)
			return 1;

		auto server = Xplicit::InstanceManager::get_singleton_ptr()->add<Xplicit::NetworkServerInstance>(ip_address);
		XPLICIT_ASSERT(server);

		Xplicit::EventDispatcher::get_singleton_ptr()->add<Xplicit::PlayerJoinLeaveEvent>();
		Xplicit::EventDispatcher::get_singleton_ptr()->add<Xplicit::ServerWatchdogEvent>();

		xplicit_attach_mono();
		xplicit_read_xml();

		xplicit_load_shell();

		while (Xplicit::InstanceManager::get_singleton_ptr() && Xplicit::EventDispatcher::get_singleton_ptr())
		{
			if (Xplicit::ApplicationContext::get_singleton().ShouldExit)
				break;

			Xplicit::NetworkServerTraits::recv(server);

			Xplicit::EventDispatcher::get_singleton_ptr()->update();
			Xplicit::InstanceManager::get_singleton_ptr()->update();

			Xplicit::NetworkServerTraits::send(server);
		}

		return 0;
	}
	catch (std::runtime_error err)
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_CRITICAL(err.what());

#ifdef XPLICIT_WINDOWS
		MessageBoxA(nullptr, err.what(), "XplicitNgin", MB_OK);
#endif
#endif

		return -1;
	}
}