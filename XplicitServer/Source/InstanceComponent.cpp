/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "InstanceComponent.h"

#define XPLICIT_LUA_GLOBAL "_G."
#define XPLICIT_LUA_NAMESPACE "Game."

namespace Xplicit
{
	InstanceComponent::InstanceComponent(
		const Vector<float>& position, 
		const Vector<float>& size, 
		const Color<float>& color, 
		const char* parent,
		const char* name,
		const bool read_only)
		: 
		mName(name),
		mParent(parent),
		mReadOnly(read_only)
	{
		mAttribute.pos() = position;
		mAttribute.scale() = size;
		mAttribute.color() = color;
	}

	InstanceComponent::~InstanceComponent() = default;

	bool InstanceComponent::should_update() noexcept { return true; }
	
	void InstanceComponent::update()
	{
		if (this->mReadOnly)
		{
			static const char* pos[] = { "X", "Y", "Z" };
			const float pos_raw[] = { mAttribute.pos().X, mAttribute.pos().Y, mAttribute.pos().Z };

			for (size_t i = 0; i < 3; i++)
			{
				String fmt = XPLICIT_LUA_GLOBAL;

				fmt += XPLICIT_LUA_NAMESPACE;
				fmt += mName;
				fmt += ".";
				fmt += mParent;
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

			String fmt = XPLICIT_LUA_GLOBAL;

			fmt += XPLICIT_LUA_NAMESPACE;
			fmt += mName;
			fmt += ".Archivable = ";
			fmt += this->get_attribute().is_archivable() ? "true" : "false";

			Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

			fmt.clear();

			fmt = XPLICIT_LUA_GLOBAL;

			fmt += XPLICIT_LUA_NAMESPACE;
			fmt += mName;
			fmt += ".Locked = ";
			fmt += this->get_attribute().is_locked() ? "true" : "false";

			Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

			fmt.clear();

			fmt = XPLICIT_LUA_GLOBAL;

			fmt += XPLICIT_LUA_NAMESPACE;
			fmt += mName;
			fmt += ".Anchor = ";
			fmt += this->get_attribute().is_anchored() ? "true" : "false";

			Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

			fmt.clear();

			fmt = XPLICIT_LUA_GLOBAL;

			fmt += XPLICIT_LUA_NAMESPACE;
			fmt += mName;
			fmt += ".Collide = ";
			fmt += this->get_attribute().has_no_collide() ? "false" : "true";

			Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

			fmt.clear();

			fmt = XPLICIT_LUA_GLOBAL;

			fmt += XPLICIT_LUA_NAMESPACE;
			fmt += mName;
			fmt += ".Alpha = ";
			fmt += std::to_string(this->get_attribute().alpha());

			Lua::XLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
		}
		else
		{

		}
	}

	XAttribute& InstanceComponent::get_attribute() noexcept { return mAttribute; }
}