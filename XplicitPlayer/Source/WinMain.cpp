/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright PlayXPlicit, all rights reserved.
 *
 *			File: WinMain.cpp
 *			Purpose: Main Application entrypoint.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "Application.h"

#include <NetworkProtocol.h>
#include <XplicitSound.h>
#include <DriverD3D11.h>
#include <BaseLuaAPI.h>
#include <Component.h>
#include <ImGUI.h>
#include <Event.h>
#include <Bites.h>
#include <codecvt>

extern void XplicitLoadClientLua() noexcept;

static void XplicitThrowException(Xplicit::EngineError& err);

#ifdef _WIN32
static void XplicitThrowException(Xplicit::Win32Error& err);
#endif

#ifdef _WIN32
XPLICIT_MAIN()
#else
int main(int argc, char** argv)
#endif
{
	try
	{
#ifdef _WIN32
        XPLICIT_INIT_COM;

#ifdef XPLICIT_DEBUG
        Xplicit::open_terminal();
#endif // XPLICIT_DEBUG
#endif

		// parse the connection uri.
		Xplicit::Utils::UriParser uri{ XPLICIT_XCONNECT_PROTOCOL };

#ifdef _WIN32
        std::string cmd_line = pCmdLine;
#else
        std::string cmd_line = argv[1];
#endif

		if (cmd_line.empty() ||
			cmd_line.find(XPLICIT_XCONNECT_PROTOCOL) == Xplicit::String::npos)
			return 1;

		cmd_line = cmd_line.erase(cmd_line.find(XPLICIT_XCONNECT_PROTOCOL), strlen(XPLICIT_XCONNECT_PROTOCOL));

		uri /= cmd_line;

		Xplicit::Utils::InternetProtocolChecker checker;

		if (!checker(uri.get().c_str()))
			return 1;

		//! Load Basic lua calls
		XplicitLoadBaseLua();

		//! Register client-side Lua calls, such as PlaySound
		XplicitLoadClientLua();

		struct Vector3
		{
			float X, Y, Z;

			static int add(lua_State* L)
			{
				Vector3* vec = (Vector3*)lua_touserdata(L, 1);

				auto X = lua_tonumber(L, 2);
				auto Y = lua_tonumber(L, 3);
				auto Z = lua_tonumber(L, 4);

				vec->X += X;
				vec->Y += Y;
				vec->Z += Z;

				return 0;
			}

			static int mul(lua_State* L)
			{
				Vector3* vec = (Vector3*)lua_touserdata(L, 1);

				auto X = lua_tonumber(L, 2);
				auto Y = lua_tonumber(L, 3);
				auto Z = lua_tonumber(L, 4);

				vec->X *= X;
				vec->Y *= Y;
				vec->Z *= Z;

				return 0;
			}

			static int sub(lua_State* L)
			{
				Vector3* vec = (Vector3*)lua_touserdata(L, 1);

				auto X = lua_tonumber(L, 2);
				auto Y = lua_tonumber(L, 3);
				auto Z = lua_tonumber(L, 4);

				vec->X -= X;
				vec->Y -= Y;
				vec->Z -= Z;

				return 0;
			}

		};

		Xplicit::RLua::RuntimeClass<Vector3> vector;

		vector.begin_class("Vector3");

		vector.append_proc("Add", &Vector3::add);
		vector.append_proc("Mul", &Vector3::mul);
		vector.append_proc("Sub", &Vector3::sub);

		vector.append_proc("PrintCoord", [](Vector3* ptr) {
			std::cout << ptr->X << ", " << ptr->Y << ", " << ptr->Z << std::endl;
		});

		vector.end_class();

		Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("local vec = Vector3(); vec:Add(1, 2, 2); vec:PrintCoord();");

		std::unique_ptr<Xplicit::Bites::Application> app_ptr = std::make_unique<Xplicit::Bites::Application>(uri);

		if (!app_ptr)
			throw Xplicit::EngineError("XPLICIT couldn't continue; we're sorry!");

		//! The Main Logic and Render loop.
		while (RENDER->run() &&
			Xplicit::ComponentSystem::get_singleton_ptr() &&
			Xplicit::EventSystem::get_singleton_ptr())
		{
			RENDER->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 0x87, 0xCE, 0xBB));

			Xplicit::Audio::XAudioEngine::get_singleton_ptr()->update();
			Xplicit::ComponentSystem::get_singleton_ptr()->update();
			Xplicit::EventSystem::get_singleton_ptr()->update();

			RENDER->getSceneManager()->drawAll();
			RENDER->getGUIEnvironment()->drawAll();

			RENDER->getVideoDriver()->endScene();

			Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string("World:Tick()");
		}
	}
	catch (Xplicit::EngineError& err)
	{
		XplicitThrowException(err);
	}
#ifdef _WIN32
	catch (Xplicit::Win32Error& err)
	{
		XplicitThrowException(err);
	}
#endif


#ifdef _WIN32
	XPLICIT_FINI_COM;
#endif
	return 0;
}

static void XplicitThrowException(Xplicit::EngineError& err)
{
#ifdef XPLICIT_DEBUG
	XPLICIT_INFO(err.what());
#endif

	std::wstring exit;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	exit += L"What: ";
	exit += converter.from_bytes(err.what());
	exit += L"\n";

#ifdef _WIN32
	Xplicit::DialogHelper::message_box(L"XplicitPlayer",
		L"XPLICIT Couldn't continue!",
		exit.c_str(),
		TD_INFORMATION_ICON,
		TDCBF_OK_BUTTON);
#endif
}

#ifdef _WIN32
static void XplicitThrowException(Xplicit::Win32Error& err)
{
#ifdef XPLICIT_DEBUG
	XPLICIT_INFO(err.what());
#endif

	std::wstring exit;
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	exit += L"WHAT: ";
	exit += converter.from_bytes(err.what());
	exit += L"HRESULT: ";
	exit += std::to_wstring(err.hr());
	exit += L"\n";

	Xplicit::DialogHelper::message_box(L"XplicitPlayer",
		L"XPLICIT Couldn't continue!",
		exit.c_str(),
		TD_INFORMATION_ICON,
		TDCBF_OK_BUTTON);
}
#endif
