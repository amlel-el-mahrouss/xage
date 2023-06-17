/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: Server.cpp
 *			Purpose: Server main unit
 *
 * =====================================================================
 */

 /**
 @file
 */

#include <XplicitID.h>
#include <codecvt>
#include <Root.h>

#include "CommonInc.h"
#include "SpawnComponent.h"
#include "PlayerComponent.h"
#include "PlayerMovementEvent.h"
#include "PlayerJoinLeaveEvent.h"
#include "PlayerSpawnDeathEvent.h"

static void xplicit_send_stop_packet(Xplicit::NetworkServerComponent* server);
static void xplicit_load_mono();
static void xplicit_load_sh();
static void xplicit_read_xml();

static constexpr const char* XPLICIT_MANIFEST_FILE = "Manifest.xml";
static bool XPLICIT_SHOULD_EXIT = false;

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

static void xplicit_load_mono()
{
	try
	{
		XPLICIT_GET_DATA_DIR(data);

		Xplicit::String dll_path = data;
		dll_path += "Lib/ScriptNgine.dll";

		Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::MonoEngineComponent>();
		Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::MonoScriptComponent>(dll_path.c_str(), false);
	}
	catch (...)
	{
		XPLICIT_CRITICAL("ScriptNgine.dll not found, to load a C# DLL use Manifest.xml.");
	}
}

static void xplicit_print_help()
{
	XPLICIT_INFO("\a+-------------- Xplicit Game Server Manual --------------+");
	XPLICIT_INFO("exit: Exits the current server.");
	XPLICIT_INFO("xdp: Get XDP network status.");
	XPLICIT_INFO("+-------------- Xplicit Game Server Manual --------------+");
}

static void xplicit_load_sh()
{
	std::thread shell(
		[]() -> void
		{
			char cmd_buf[1024];

			while (Xplicit::ComponentManager::get_singleton_ptr() && Xplicit::EventManager::get_singleton_ptr())
			{
				if (!XPLICIT_SHOULD_EXIT)
					std::cout << "> ";

				std::cin.getline(cmd_buf, 1024);

				if (strcmp(cmd_buf, "exit") == 0)
				{
					XPLICIT_SHOULD_EXIT = true;

					auto server = Xplicit::ComponentManager::get_singleton_ptr()->get<Xplicit::NetworkServerComponent>("NetworkServerComponent");
					XPLICIT_ASSERT(server != nullptr);

					xplicit_send_stop_packet(server);
				}
				
				if (strcmp(cmd_buf, "help") == 0)
					xplicit_print_help();

				if (strcmp(cmd_buf, "xdp") == 0)
				{

					const char* ip4 = XPLICIT_ENV("XPLICIT_SERVER_ADDR");

					if (!ip4)
						XPLICIT_CRITICAL("CLI: Ip Address is invalid, please define XPLICIT_SERVER_ADDR again in order to be able to reboot the server.");

					XPLICIT_INFO(Xplicit::String("IP: ") + (ip4 ? ip4 : "?"));
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

	std::cout << server->size() << std::endl;

	for (size_t i = 0; i < server->size(); i++)
	{
		server->get(i)->packet.cmd[XPLICIT_NETWORK_CMD_SHUTDOWN] = Xplicit::NETWORK_CMD_SHUTDOWN;
		server->get(i)->packet.hash = server->get(i)->hash;
	}

	Xplicit::NetworkServerHelper::send(server);
}

// our main entrypoint.
int main(int argc, char** argv)
{
	try
	{
		Xplicit::Root::get_singleton_ptr()->set(irr::createDevice(irr::video::EDT_NULL));

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
			XPLICIT_INFO("XPLICIT_SERVER_ADDR is undefined! please set this in the envpath.");
			return 1;
		}

		auto server = Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::NetworkServerComponent>(ip4);
		
		XPLICIT_ASSERT(server);

		if (server == nullptr)
			return 1;

		/* Add C++ spawn component */
		Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::SpawnComponent>(Xplicit::Quaternion<float>(0.f, 0.f, 0.f));

		/* add C++ events */
		Xplicit::EventManager::get_singleton_ptr()->add<Xplicit::PlayerMovementEvent>();
		Xplicit::EventManager::get_singleton_ptr()->add<Xplicit::PlayerJoinLeaveEvent>();
		Xplicit::EventManager::get_singleton_ptr()->add<Xplicit::PlayerSpawnDeathEvent>();
		
		xplicit_load_mono();
		xplicit_read_xml();
		xplicit_load_sh();

		std::atexit([]() -> void {
			auto comp = Xplicit::ComponentManager::get_singleton_ptr()->get<Xplicit::NetworkServerComponent>("NetworkServerComponent");

			if (comp)
				xplicit_send_stop_packet(comp);
		});

		do
		{
			if (XPLICIT_SHOULD_EXIT)
				break;

			Xplicit::NetworkServerHelper::recv(server);
			Xplicit::NetworkServerHelper::correct(server);

			Xplicit::ComponentManager::get_singleton_ptr()->update();
			Xplicit::EventManager::get_singleton_ptr()->update();

			Xplicit::NetworkServerHelper::send(server);
		} while (Xplicit::ComponentManager::get_singleton_ptr() && 
			Xplicit::EventManager::get_singleton_ptr());

		return 0;
	}
	catch (const std::exception& err)
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_CRITICAL(err.what());

#ifdef XPLICIT_WINDOWS
		std::wstring exit;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

		exit += L"What: ";
		exit += converter.from_bytes(err.what());
		exit += L"\n";

		Xplicit::DialogHelper::message_box(L"Xplicit Engine", 
			L"Program Exited", 
			exit.c_str(), 
			TD_INFORMATION_ICON, 
			_TASKDIALOG_COMMON_BUTTON_FLAGS::TDCBF_OK_BUTTON);
#endif
#endif

		return -1;
	}
}