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

#pragma once

#include "Xplicit.h"

namespace Xplicit
{
	class XPLICIT_API GameVarView final
	{
	private:
		GameVarView(const char* name, const char* default_value, int flags);

	public:
		~GameVarView();

		GameVarView& operator=(const GameVarView&) = default;
		GameVarView(const GameVarView&) = default;

		enum FLAGS : int32_t
		{
			FLAG_SERVER_ONLY,
			FLAG_CLIENT_ONLY,
			FLAG_SHARED,
			FLAG_CHEAT,
			FLAG_COUNT,
		};

	public:
		std::string& as_str() noexcept;
		int32_t as_int() noexcept;
		float as_float() noexcept;
		int32_t flags() noexcept;

	public:
		const char* name() noexcept;

	private:
		std::string mValue;
		std::string mName;

		int32_t mFlags;

		friend class GameVarManager;

	};

	using GameVarViewPtr = std::shared_ptr<GameVarView>;

	class XPLICIT_API GameVarManager final
	{
	private:
		GameVarManager() = default;

	public:
		~GameVarManager() = default;

		XPLICIT_COPY_DEFAULT(GameVarManager);

	public:
		static GameVarManager* get_singleton_ptr() noexcept;

	public:
		GameVarViewPtr create(const char* name, const char* default_value, int flags);
		GameVarViewPtr get(const char* name);
		void remove(GameVarView* ptr);
		
	private:
		std::vector<GameVarView*> m_cvars;

	};
}