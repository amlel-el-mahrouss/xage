/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "LocalSoundComponent.h"
#include <ClassComponent.h>

namespace Xplicit::Player
{
	LocalSoundComponent::LocalSoundComponent() 
		: mPosition(0.0f, 0.0f, 0.0f), mVolume(0.5f), mLoop(false) 
	{
		String fmt = String(XPLICIT_LUA_GLOBAL);
		fmt += ".Game.";
		fmt += "SoundMixer ";
		fmt += " = { Volume = 0.5, Position = { 0, 0, 0 }, Loop = false }";

		Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
	}

	LocalSoundComponent::~LocalSoundComponent()
	{
		String fmt = String(XPLICIT_LUA_GLOBAL);
		fmt += ".Game.";
		fmt += "SoundMixer ";
		fmt += " = nil";

		Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
	}

	const char* LocalSoundComponent::name() noexcept { return "LocalSoundComponent"; }

	COMPONENT_TYPE LocalSoundComponent::type() noexcept { return COMPONENT_SOUND; }

	bool LocalSoundComponent::should_update() noexcept { return true; }

	PHYSICS_TYPE LocalSoundComponent::physics() noexcept { return PHYSICS_SIMPLE; }

	void LocalSoundComponent::set_position(const Vector<float> pos) noexcept { Vector<float> mPosition = pos; }

	void LocalSoundComponent::set_volume(const float volume) noexcept
	{
		if (volume > 1.0f)
		{
			mVolume = 1.0f;
			return;
		}

		mVolume = volume;
	}

	void LocalSoundComponent::update()
	{
		lua_pushstring(Lua::CLuaStateManager::get_singleton_ptr()->state(),
			std::format("{}{}{}.Loop;", XPLICIT_LUA_GLOBAL, XPLICIT_LUA_NAMESPACE, "Sound").c_str());

		mLoop = lua_toboolean(Lua::CLuaStateManager::get_singleton_ptr()->state(), -1);

		lua_pop(Lua::CLuaStateManager::get_singleton_ptr()->state(), -1);

		lua_pushstring(Lua::CLuaStateManager::get_singleton_ptr()->state(),
			std::format("{}{}{}.Volume;", XPLICIT_LUA_GLOBAL, XPLICIT_LUA_NAMESPACE, "Sound").c_str());

		mLoop = lua_tonumber(Lua::CLuaStateManager::get_singleton_ptr()->state(), -1);

		lua_pop(Lua::CLuaStateManager::get_singleton_ptr()->state(), -1);

		lua_pushstring(Lua::CLuaStateManager::get_singleton_ptr()->state(),
			std::format("{}{}{}.Position.X;", XPLICIT_LUA_GLOBAL, XPLICIT_LUA_NAMESPACE, "Sound").c_str());

		mPosition.X = lua_tonumber(Lua::CLuaStateManager::get_singleton_ptr()->state(), -1);

		lua_pop(Lua::CLuaStateManager::get_singleton_ptr()->state(), -1);

		lua_pushstring(Lua::CLuaStateManager::get_singleton_ptr()->state(),
			std::format("{}{}{}.Position.Y;", XPLICIT_LUA_GLOBAL, XPLICIT_LUA_NAMESPACE, "Sound").c_str());

		mPosition.Y = lua_tonumber(Lua::CLuaStateManager::get_singleton_ptr()->state(), -1);

		lua_pop(Lua::CLuaStateManager::get_singleton_ptr()->state(), -1);

		lua_pushstring(Lua::CLuaStateManager::get_singleton_ptr()->state(),
			std::format("{}{}{}.Position.Z;", XPLICIT_LUA_GLOBAL, XPLICIT_LUA_NAMESPACE, "Sound").c_str());

		mPosition.Z = lua_tonumber(Lua::CLuaStateManager::get_singleton_ptr()->state(), -1);

		lua_pop(Lua::CLuaStateManager::get_singleton_ptr()->state(), -1);
	}

	void LocalSoundComponent::should_loop(const bool enable) noexcept
	{
		mLoop = enable;
	}

	void LocalSoundComponent::play_2d(const String& path) noexcept
	{
		Xplicit::Audio::XAudioEngine::get_singleton_ptr()->set_volume(mVolume);

		Thread job([&](String _path) {
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cvt;
			std::shared_ptr<Audio::XAudioEngine::XAudioHandle> audio = Xplicit::Audio::XAudioEngine::get_singleton_ptr()->make_audio(cvt.from_bytes(_path).c_str());
			
			if (audio)
				audio->play();
		}, path);

		job.detach();
	}

	void LocalSoundComponent::play(const String& path) noexcept
	{
		Xplicit::Audio::XAudioEngine::get_singleton_ptr()->set_volume(mVolume);

		Thread job([&](String _path) {
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> cvt;
			std::shared_ptr<Audio::XAudioEngine::XAudioHandle> audio = Xplicit::Audio::XAudioEngine::get_singleton_ptr()->make_audio(cvt.from_bytes(_path).c_str());
			
			if (audio)
				audio->play_3d(mPosition, &mLoop);
		}, path);

		job.detach();
	}
}