/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: ConVar.h
 *			Purpose: C++ Console Variable
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

namespace Xplicit
{
	class XPLICIT_API ConVar final
	{
	public:
		ConVar(const char* name, const char* default_value, int flags);
		~ConVar();

		ConVar& operator=(const ConVar&) = default;
		ConVar(const ConVar&) = default;

	public:
		const char* as_str() noexcept;
		int32_t as_int() noexcept;
		float as_float() noexcept;
		int32_t flags() noexcept;

	private:
		std::string m_value;
		std::string m_name;

		int32_t m_flags;

	};

	using ConVarPtr = std::unique_ptr<ConVar>;
}