/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "NginCore.h"
#include "Event.h"

#include <CLua.hpp>

namespace XPX
{
	class XPLICIT_API LuaEventListener final : public EventListener
	{
	public:
		LuaEventListener(RLua::RLuaProc proc);
		~LuaEventListener() override;

	public:
		XPLICIT_COPY_DEFAULT(LuaEventListener);

	public:
		void update(EventPtr _event) override;
		const char* name() noexcept override;

	private:
		RLua::RLuaProc mProcedure;

	};
}