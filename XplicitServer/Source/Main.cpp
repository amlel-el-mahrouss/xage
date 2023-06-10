/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Server.cpp
 *			Purpose: Server main unit
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "SDK.h"

#include "SpawnComponent.h"
#include "PlayerComponent.h"
#include "PlayerMovementEvent.h"
#include "PlayerJoinLeaveEvent.h"
#include "PlayerSpawnDeathEvent.h"

static void xplicit_send_stop_packet(Xplicit::NetworkServerComponent* server);
static void xplicit_attach_mono();
static void xplicit_load_sh();
static void xplicit_read_xml();

static constexpr const char* XPLICIT_MANIFEST_FILE = "Manifest.xml";
static bool g_ShouldExit = false;

static void xplicit_read_xml()
{
	XPLICIT_GET_DATA_DIR(data_path);
	std::string path = data_path;

	path += "/"; 
	path += XPLICIT_MANIFEST_FILE;

	rapidxml::file<> xml{ path.c_str() };
	XPLICIT_ASSERT(xml.size() > 0);

	auto mono = Xplicit::ComponentManager::get_singleton_ptr()->get<Xplicit::MonoEngineComponent>("MonoEngineComponent");

	const int argc = 2;
	const char* argv[] = { "XplicitNgin", "1.0.1" };

	rapidxml::xml_document<> doc{};
	doc.parse<0>(xml.data());

	auto node = doc.first_node();

	while (node)
	{
		if (strcmp(node->name(), "Plugin") == 0)
		{
			auto dll = node->value();

			path.clear();

			path += data_path;
			path += "/Lib/";
			path += dll;

			auto csharp_dll = Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::MonoScriptComponent>(path.c_str(), false);
			auto assembly_file = mono->open(path.c_str());

			if (assembly_file)
			{
				XPLICIT_INFO("[C#] Running: " + path);
				mono->run(assembly_file, argc, argv);
			}
			else
			{
				XPLICIT_INFO("[C#] " + path + " No such file.");
				Xplicit::ComponentManager::get_singleton_ptr()->remove(csharp_dll);
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

	Xplicit::String dll_path = data;
	dll_path += "Lib/ScriptNgine.dll";

	Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::MonoEngineComponent>();
	Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::MonoScriptComponent>(dll_path.c_str(), false);
}

static void xplicit_print_help()
{
	XPLICIT_INFO("\a+-------------- Xplicit Game Server Manual --------------+");
	XPLICIT_INFO("exit: Exits the current server.");
	XPLICIT_INFO("netstat: Get Network status.");
	XPLICIT_INFO("+-------------- Xplicit Game Server Manual --------------+");
}

static void xplicit_load_sh()
{
	std::thread shell(
		[]() -> void
		{

			char cmd_buf[1024];

			while (Xplicit::ComponentManager::get_singleton_ptr() && Xplicit::EventDispatcher::get_singleton_ptr())
			{
				if (!g_ShouldExit)
					std::cout << "$ ";

				std::cin.getline(cmd_buf, 1024);

				if (strcmp(cmd_buf, "exit") == 0)
				{
					auto server = Xplicit::ComponentManager::get_singleton_ptr()->get<Xplicit::NetworkServerComponent>("NetworkServerComponent");
					XPLICIT_ASSERT(server != nullptr);

					if (server)
					{
						xplicit_send_stop_packet(server);
						Xplicit::NetworkServerTraits::send(server);
					}
					else
					{
						XPLICIT_ERROR("ERROR: server is not connected to the internet.");
					}

					g_ShouldExit = true;
				}

				if (strcmp(cmd_buf, "man") == 0)
					xplicit_print_help();

				if (strcmp(cmd_buf, "netstat") == 0)
				{

					const char* ip_address = XPLICIT_ENV("XPLICIT_SERVER_ADDR");

					if (!ip_address)
						XPLICIT_CRITICAL("CLI: Ip Address is invalid, please define XPLICIT_SERVER_ADDR again in order to be able to reboot the server.");

					XPLICIT_INFO(Xplicit::String("IP: ") + (ip_address ? ip_address : "?"));
					XPLICIT_INFO(Xplicit::String("Protocol version: ") + std::to_string(XPLICIT_NETWORK_VERSION));
				}
			}
		}
	);

	shell.detach();
}

static void xplicit_send_stop_packet(Xplicit::NetworkServerComponent* server)
{
	XPLICIT_ASSERT(server != nullptr);

	if (!server)
		return;

	for (size_t i = 0; i < server->size(); i++)
	{
		server->get(i)->packet.cmd[XPLICIT_NETWORK_CMD_STOP] = Xplicit::NETWORK_CMD_STOP;
		server->get(i)->packet.hash = server->get(i)->hash;
	}
}

static void xplicit_load_logic(Xplicit::NetworkServerComponent* server)
{
	std::thread logic(
		[&]() -> void
		{
			do
			{
				Xplicit::NetworkServerTraits::recv(server);
				Xplicit::EventDispatcher::get_singleton_ptr()->update();
				Xplicit::ComponentManager::get_singleton_ptr()->update();
				Xplicit::NetworkServerTraits::send(server);
			} while (Xplicit::ComponentManager::get_singleton_ptr() && Xplicit::EventDispatcher::get_singleton_ptr());
		}
	);

	logic.detach();
}

// our main entrypoint.
int main(int argc, char** argv)
{
	try
	{
#ifdef XPLICIT_WINDOWS
		WSADATA wsa;
		RtlZeroMemory(&wsa, sizeof(WSADATA));

		Xplicit::init_winsock(&wsa);
#endif

		/*
			The address to bind
			is located in the XPLICIT_SERVER_ADDR
		*/

		const char* ip_address = XPLICIT_ENV("XPLICIT_SERVER_ADDR");

		if (!ip_address)
		{
			XPLICIT_INFO("XPLICIT_SERVER_ADDR is undefined!please set this in the env.");
			return 1;
		}

		auto server = Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::NetworkServerComponent>(ip_address);
		XPLICIT_ASSERT(server);

		if (!server)
			return 1;

		Xplicit::EventDispatcher::get_singleton_ptr()->add<Xplicit::PlayerJoinLeaveEvent>();
		Xplicit::EventDispatcher::get_singleton_ptr()->add<Xplicit::PlayerSpawnDeathEvent>();
		Xplicit::EventDispatcher::get_singleton_ptr()->add<Xplicit::PlayerMovementEvent>();

		xplicit_attach_mono();
		xplicit_read_xml();
		xplicit_load_sh();
		xplicit_load_logic(server);

		do
		{
			if (g_ShouldExit)
				break;
		} while (!g_ShouldExit);

		return 0;
	}
	catch (std::runtime_error& err)
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_CRITICAL(err.what());

#ifdef XPLICIT_WINDOWS
		Xplicit::Dialog::message_box(L"XplicitNgin", L"Something bad happened... exiting!", MB_ICONASTERISK | MB_OK);
#endif
#endif

		return -1;
	}
}