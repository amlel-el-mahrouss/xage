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

#include "ConVar.h"

 /**
 @file
 */

namespace Xplicit
{
	ConVar::ConVar(const char* name, const char* default_value, int flags)
		: m_name(name), m_value(default_value), m_flags(flags)
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("ConVar::ConVar");
#endif
	}

	ConVar::~ConVar()
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("ConVar::~ConVar");
#endif
	}

	int32_t ConVar::flags() noexcept { return m_flags; }

	int32_t ConVar::as_int() noexcept { return std::stoi(m_value); }
	float ConVar::as_float() noexcept { return std::stof(m_value); }
	const char* ConVar::as_str() noexcept { return m_value.c_str(); }
}