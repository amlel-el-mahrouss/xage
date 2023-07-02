/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "PartComponent.h"

#define XPLICIT_LUA_GLOBAL "_G."
#define XPLICIT_LUA_NAMESPACE "Game."

namespace Xplicit
{
	PartComponent::PartComponent(const Vector<float>& position, 
		const Vector<float>& size, 
		const Color<float>& color, 
		const char* name)
		: mName(name)
	{
		mAttribute.pos() = position;
		mAttribute.scale() = size;
		mAttribute.color() = color;
	}

	PartComponent::~PartComponent() = default;

	bool PartComponent::should_update() noexcept
	{
		return true;
	}
	
	void PartComponent::update()
	{
		static const char* pos[] = { "X", "Y", "Z" };
		const float pos_raw[] = { mAttribute.pos().X, mAttribute.pos().Y, mAttribute.pos().Z };

		for (size_t i = 0; i < 3; i++)
		{
			String fmt = XPLICIT_LUA_GLOBAL;

			fmt += XPLICIT_LUA_NAMESPACE;
			fmt += mName;
			fmt += ".Position.";
			fmt += pos[i];
			fmt += "= " + std::to_string(pos_raw[i]) + ";";

			Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
		}

		static const char* scale[] = { "X", "Y", "Z" };
		const float scale_raw[] = { mAttribute.scale().X, mAttribute.scale().Y, mAttribute.scale().Z };

		for (size_t i = 0; i < 3; i++)
		{
			String fmt = XPLICIT_LUA_GLOBAL;

			fmt += XPLICIT_LUA_NAMESPACE;
			fmt += mName;
			fmt += ".Size.";
			fmt += pos[i];
			fmt += "= " + std::to_string(scale_raw[i]) + ";";

			Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
		}

		static const char* clr[] = { "R", "G", "B" };
		const float clr_raw[] = { mAttribute.color().R, mAttribute.color().G, mAttribute.color().B };

		for (size_t i = 0; i < 3; i++)
		{
			String fmt = XPLICIT_LUA_GLOBAL;

			fmt += XPLICIT_LUA_NAMESPACE;
			fmt += mName;
			fmt += ".Color.";
			fmt += clr[i];
			fmt += "= " + std::to_string(clr_raw[i]) + ";";

			Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
		}
	}

	XAttribute& PartComponent::get_attribute() noexcept { return mAttribute; }
}