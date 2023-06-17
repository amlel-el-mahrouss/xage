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
		: mName(name), mValue(default_value), mFlags(flags)
	{

	}

	GameVarView::~GameVarView()
	{
		GameVarManager::get_singleton_ptr()->remove(this);
	}

	int32_t GameVarView::flags() noexcept { return mFlags; }

	int32_t GameVarView::as_int() noexcept { return std::stoi(mValue); }

	float GameVarView::as_float() noexcept { return std::stof(mValue); }

	std::string& GameVarView::as_str() noexcept { return mValue; }

	const char* GameVarView::name() noexcept { return mName.c_str(); }

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