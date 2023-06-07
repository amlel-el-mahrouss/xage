/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
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
		std::string m_value;
		std::string m_name;

		int32_t m_flags;

		friend class GameVarSingleton;

	};

	using GameVarViewPtr = std::shared_ptr<GameVarView>;

	class XPLICIT_API GameVarSingleton final
	{
	private:
		GameVarSingleton() = default;

	public:
		~GameVarSingleton() = default;

	public:
		static GameVarSingleton* get_singleton_ptr() noexcept;

	public:
		GameVarViewPtr get(const char* name);
		GameVarViewPtr create(const char* name, const char* default_value, int flags);
		
	public:
		void remove(GameVarView* ptr);

	private:
		std::vector<GameVarView*> m_cvars;

	};
}