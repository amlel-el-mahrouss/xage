/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include <Root.h>
#include <codecvt>
#include <XplicitID.h>

#include "CommonInclude.h"
#include "SpawnComponent.h"
#include "PlayerLoginEvent.h"
#include "PlayerTimeoutEvent.h"
#include "PlayerMovementEvent.h"
#include "PlayerSpawnDeathEvent.h"
#include "ServerReplicationManager.h"

const char* XPLICIT_MANIFEST_FILE = "Manifest.xml";
bool XPLICIT_EXIT_REQUESTED = false;
std::mutex XPLICIT_MUTEX;

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
	const char* argv[] = { "XplicitNgine", "1.1.1" };

	rapidxml::xml_document<> doc{};
	doc.parse<0>(xml.data());

	auto node = doc.first_node();

	while (node)
	{
		if (strcmp(node->name(), "Dll") == 0)
		{
			auto dll = node->value();

			path.clear();

			path += data_path;
			path += "/Library/";
			path += dll;

			const auto csharp_dll = Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::MonoScriptComponent>(path.c_str(), false);
			const auto assembly_file = mono->open(path.c_str());

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
		dll_path += "Plugin/PluginNgine.dll";

		Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::MonoEngineComponent>();
		Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::MonoScriptComponent>(dll_path.c_str(), false);
	}
	catch (...)
	{
		XPLICIT_CRITICAL("XplicitPluginRuntime not found, to load a C# DLL use Manifest.xml.");
	}
}

static void xplicit_print_help()
{
	XPLICIT_INFO("\a+-------------- Xplicit Game Server Manual --------------+");
	XPLICIT_INFO("exit: Exits the current server.");
	XPLICIT_INFO("xconnect: Get Xconnect network info.");
	XPLICIT_INFO("+-------------- Xplicit Game Server Manual --------------+");
}

static void xplicit_load_sh()
{
	char cmd_buf[1024];

	while (Xplicit::ComponentManager::get_singleton_ptr() && Xplicit::EventManager::get_singleton_ptr())
	{
		std::cout << "> ";

		std::cin.getline(cmd_buf, 1024);

		if (strcmp(cmd_buf, "exit") == 0)
		{
			XPLICIT_EXIT_REQUESTED = true;
			std::exit(0);
		}

		if (strcmp(cmd_buf, "help") == 0)
			xplicit_print_help();

		if (strcmp(cmd_buf, "script_test") == 0)
		{
			const auto net = Xplicit::ComponentManager::get_singleton_ptr()->get<Xplicit::NetworkServerComponent>("NetworkServerComponent");

			if (net)
			{
				for (std::size_t index = 0UL; index < net->size(); ++index)
				{
					if (net->get(index)->status != Xplicit::NETWORK_STAT_CONNECTED)
						continue;

					Xplicit::ServerReplicationManager::get_singleton_ptr()->create(Xplicit::COMPONENT_ID_SCRIPT, "xasset://xplicit-client.lua", net->get(index)->public_hash);
				}
			}

			Xplicit::NetworkServerContext::send_all(net);
		}

		if (strcmp(cmd_buf, "xconnect") == 0)
		{
			const char* ip4 = XPLICIT_ENV("XPLICIT_SERVER_ADDR");

			if (!ip4)
				XPLICIT_CRITICAL("xconnect: IP address is invalid, please define XPLICIT_SERVER_ADDR again in order to be able to reboot the server.");

			XPLICIT_INFO(Xplicit::String("IP: ") + (ip4 ? ip4 : "?"));
			XPLICIT_INFO(Xplicit::String("Protocol version: ") + std::to_string(XPLICIT_NETWORK_VERSION));
		}
	}
}

/* Application main entrypoint */
int main(int argc, char** argv)
{
	try
	{
		Xplicit::Root::get_singleton_ptr()->set(irr::createDevice(EDT_NULL));

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
			XPLICIT_INFO("Please define XPLICIT_SERVER_ADDR.");
			return 1;
		}

		const auto network = Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::NetworkServerComponent>(ip4);

#ifdef XPLICIT_WINDOWS

		Xplicit::String title = XPLICIT_ENV("XPLICIT_SERVER_ADDR");

		title += ":";
		title += std::to_string(network->port());
		title += " (xconnect v";
		title += std::to_string(XPLICIT_NETWORK_VERSION);
		title += ")";

		SetConsoleTitleA(title.c_str());

#endif // XPLICIT_WINDOWS

		Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::SpawnComponent>(Xplicit::Quaternion(0.f, 0.f, 0.f));

		Xplicit::EventManager::get_singleton_ptr()->add<Xplicit::PlayerSpawnDeathEvent>();
		Xplicit::EventManager::get_singleton_ptr()->add<Xplicit::PlayerMovementEvent>();
		Xplicit::EventManager::get_singleton_ptr()->add<Xplicit::PlayerTimeoutEvent>();
		Xplicit::EventManager::get_singleton_ptr()->add<Xplicit::PlayerLoginEvent>();

		xplicit_load_mono();
		xplicit_read_xml();

		std::atexit([]() -> void {
			const auto net = Xplicit::ComponentManager::get_singleton_ptr()->get<Xplicit::NetworkServerComponent>("NetworkServerComponent");

			if (net)
			{
				for (std::size_t index = 0UL; index < net->size(); ++index)
				{
					net->get(index)->packet.cmd[XPLICIT_NETWORK_CMD_SHUTDOWN] = Xplicit::NETWORK_CMD_SHUTDOWN;
					net->get(index)->packet.channel = XPLICIT_CHANNEL_CHAT;

					memcpy(net->get(index)->packet.buffer, "XplicitNgin, goodbye...", strlen("XplicitNgin, goodbye..."));
				}
			}

		});

		auto id = std::this_thread::get_id();

		Xplicit::Thread net([&]() {
			const auto net = Xplicit::ComponentManager::get_singleton_ptr()->get<Xplicit::NetworkServerComponent>("NetworkServerComponent");

			while (Xplicit::ComponentManager::get_singleton_ptr() &&
				Xplicit::EventManager::get_singleton_ptr())
			{
				if (XPLICIT_EXIT_REQUESTED)
					break;

				Xplicit::NetworkServerContext::recv_all(net);

				XPLICIT_MUTEX.lock();

				Xplicit::NetworkServerContext::send_all(net);
				
				XPLICIT_MUTEX.unlock();
			};
		});

		net.detach();

		Xplicit::Thread logic([&]() {
			const auto net = Xplicit::ComponentManager::get_singleton_ptr()->get<Xplicit::NetworkServerComponent>("NetworkServerComponent");

			while (Xplicit::ComponentManager::get_singleton_ptr() &&
				Xplicit::EventManager::get_singleton_ptr())
			{
				if (XPLICIT_EXIT_REQUESTED)
					break;

				XPLICIT_MUTEX.lock();

				Xplicit::ComponentManager::get_singleton_ptr()->update();
				Xplicit::EventManager::get_singleton_ptr()->update();

				XPLICIT_MUTEX.unlock();
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

#ifdef XPLICIT_WINDOWS
		std::wstring exit;

		// UTF-8 to UTF-16 converter which transforms char bytes into wchar_t
		// this is need due to the nature of utf-16 as it takes more bytes to encode a character.
		// also deprecated since c++17 for retarded reasons.

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

		exit += L"What: ";
		exit += converter.from_bytes(err.what());
		exit += L"\n";

		Xplicit::DialogHelper::message_box(L"Xplicit Engine", 
			L"Program Exited", 
			exit.c_str(), 
			TD_INFORMATION_ICON, 
			TDCBF_OK_BUTTON);
#endif
#endif

		return -1;
	}
}