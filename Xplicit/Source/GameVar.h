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
	class XPLICIT_API GameVar final
	{
	private:
		GameVar(const char* name, const char* default_value, int flags);

	public:
		~GameVar();

		GameVar& operator=(const GameVar&) = default;
		GameVar(const GameVar&) = default;

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

	using GameVarPtr = std::shared_ptr<GameVar>;

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
		GameVarPtr create(const char* name, const char* default_value, int flags);
		GameVarPtr get(const char* name);
		void remove(GameVar* ptr);
		
	private:
		std::vector<GameVar*> m_cvars;

	};
}