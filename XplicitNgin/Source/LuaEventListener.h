/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright PlayXPlicit, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"
#include "Event.h"

#include <CLua/CLua.hpp>

namespace XPX
{
	class XPLICIT_API LuaEventListener final : public EventListener
	{
	public:
		LuaEventListener(const char* snippet);
		~LuaEventListener() override;

	public:
		XPLICIT_COPY_DEFAULT(LuaEventListener);

	public:
		void update(EventPtr _event) override;
		const char* name() noexcept override;

	private:
		String mSnippet;

	};
}