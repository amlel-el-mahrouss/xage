/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: GameVar.h
 *			Purpose: C++ Game Variable Implementation
 *
 * =====================================================================
 */

#include "GameVar.h"

 /**
 @file
 */

namespace Xplicit
{
	GameVarView::GameVarView(const char* name, const char* default_value, int flags)
		: m_name(name), m_value(default_value), m_flags(flags)
	{

	}

	GameVarView::~GameVarView()
	{
		GameVarManager::get_singleton_ptr()->remove(this);
	}

	int32_t GameVarView::flags() noexcept { return m_flags; }

	int32_t GameVarView::as_int() noexcept { return std::stoi(m_value); }

	float GameVarView::as_float() noexcept { return std::stof(m_value); }

	std::string& GameVarView::as_str() noexcept { return m_value; }

	const char* GameVarView::name() noexcept { return m_name.c_str(); }

	GameVarManager* GameVarManager::get_singleton_ptr() noexcept 
	{
		static GameVarManager* singleton = nullptr;

		if (!singleton)
			singleton = new GameVarManager();

		return singleton;
	}

	GameVarViewPtr GameVarManager::get(const char* name) 
	{
		if (!name ||
			*name == 0)
			return {};

		for (auto* cvar : m_cvars)
		{
			if (cvar && strcmp(cvar->name(), name) == 0)
			{
				return GameVarViewPtr(cvar);
			}
		}

		return {};
	}

	GameVarViewPtr GameVarManager::create(const char* name, const char* default_value, int flags)
	{
		GameVarView* cvar = new GameVarView(name, default_value, flags);
		XPLICIT_ASSERT(cvar);

		if (cvar)
		{
			m_cvars.push_back(cvar);
			return GameVarViewPtr(cvar);
		}

		return {};
	}

	void GameVarManager::remove(GameVarView* ptr)
	{
		if (!ptr)
			return;

		auto it = std::find(m_cvars.cbegin(), m_cvars.cend(), ptr);

		if (it != m_cvars.cend())
			m_cvars.erase(it);
	}
}