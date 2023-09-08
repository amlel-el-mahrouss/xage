/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright PlayXPlicit, all rights reserved.
 *
 *			File: GameVar.h
 *			Purpose: C++ World Variable Implementation
 *
 * =====================================================================
 */

#include "GameVar.h"

 /**
 @file
 */

namespace Xplicit
{
	GameVar::GameVar(const char* name, const char* value, int flags)
		: mName(name), mValue(value), mFlags(flags), Lua::CLuaClass((String("World.Settings.") + name).c_str())
	{
		this->insert("__newindex", "function() error('GameVar is read-only!'); end");

		this->insert("Name", name);
		this->insert("Value", value);
		this->insert("Flags", std::to_string(flags).c_str());
	}

	GameVar::~GameVar()
	{
		GameVarManager::get_singleton_ptr()->remove(this);
	}

	int32_t GameVar::flags() noexcept { return mFlags; }

	int32_t GameVar::as_int() noexcept { return std::stoi(mValue); }

	float GameVar::as_float() noexcept { return std::stof(mValue); }

	std::string& GameVar::as_str() noexcept { return mValue; }

	const char* GameVar::name() noexcept { return mName.c_str(); }

	GameVarManager* GameVarManager::get_singleton_ptr() noexcept 
	{
		static GameVarManager* singleton = nullptr;

		if (!singleton)
			singleton = new GameVarManager();

		return singleton;
	}

	GameVarPtr GameVarManager::get(const char* name) 
	{
		if (!name ||
			*name == 0)
			return {};

		for (auto* cvar : m_cvars)
		{
			if (cvar && strcmp(cvar->name(), name) == 0)
			{
				return GameVarPtr(cvar);
			}
		}

		return {};
	}

	GameVarPtr GameVarManager::create(const char* name, const char* default_value, int flags)
	{
		GameVar* cvar = new GameVar(name, default_value, flags);
		XPLICIT_ASSERT(cvar);

		if (cvar)
		{
			m_cvars.push_back(cvar);
			return GameVarPtr(cvar);
		}

		return {};
	}

	void GameVarManager::remove(GameVar* ptr)
	{
		if (!ptr)
			return;

		auto it = std::find(m_cvars.cbegin(), m_cvars.cend(), ptr);

		if (it != m_cvars.cend())
			m_cvars.erase(it);
	}
}