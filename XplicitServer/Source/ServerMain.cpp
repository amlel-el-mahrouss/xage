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

#include <Root.h>
#include <codecvt>
#include <XplicitID.h>

#include "CommonInclude.h"
#include "SpawnComponent.h"
#include "PlayerComponent.h"
#include "PlayerTimeoutEvent.h"
#include "PlayerMovementEvent.h"
#include "PlayerJoinLeaveEvent.h"
#include "PlayerSpawnDeathEvent.h"

static void xplicit_load_mono();
static void xplicit_read_xml();

static const char* XPLICIT_MANIFEST_FILE = "Manifest.xml";
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
		dll_path += "Library/ScriptNgine.dll";

		Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::MonoEngineComponent>();
		Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::MonoScriptComponent>(dll_path.c_str(), false);
	}
	catch (...)
	{
		XPLICIT_CRITICAL("ScriptNgine.dll not found, to load a C# DLL use Manifest.xml.");
	}
}

/* Application main entrypoint */
int main(int argc, char** argv)
{
	try
	{
		Xplicit::String title = "XplicitNgine (xconnect v";
		title += std::to_string(XPLICIT_NETWORK_VERSION);
		title += ")";

		SetConsoleTitleA(title.c_str());

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

		auto server = Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::NetworkServerComponent>(ip4);
		
		XPLICIT_ASSERT(server);

		if (server == nullptr)
			return 1;

		Xplicit::ComponentManager::get_singleton_ptr()->add<Xplicit::SpawnComponent>(Xplicit::Quaternion(0.f, 0.f, 0.f));

		Xplicit::EventManager::get_singleton_ptr()->add<Xplicit::PlayerJoinLeaveEvent>();
		Xplicit::EventManager::get_singleton_ptr()->add<Xplicit::PlayerTimeoutEvent>();
		Xplicit::EventManager::get_singleton_ptr()->add<Xplicit::PlayerSpawnDeathEvent>();
		Xplicit::EventManager::get_singleton_ptr()->add<Xplicit::PlayerMovementEvent>();
		
		xplicit_load_mono();
		xplicit_read_xml();

		do
		{
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