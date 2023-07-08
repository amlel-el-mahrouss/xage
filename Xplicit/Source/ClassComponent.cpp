/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "ClassComponent.h"

namespace Xplicit
{
	ClassComponent::ClassComponent(
		const Vector<float>& position, 
		const Vector<float>& size, 
		const Color<float>& color,
		const char* script,
		const char* parent,
		const char* name)
		: 
		mCLua(std::make_unique<Lua::CLuaClass>((String(parent) + name).c_str())),
		mName(name),
		mParent(parent)
	{
		mAttribute.pos() = position;
		mAttribute.scale() = size;
		mAttribute.color() = color;

		if (script)
		{
			mAttribute.script(ComponentSystem::get_singleton_ptr()->add<LuaScriptComponent>(script, true));
			XPLICIT_ASSERT(mAttribute.script());
		}

		mCLua->insert("Scale", "{ X = 0, Y = 0, Z = 0 }");
		mCLua->insert("Position", "{ X = 0, Y = 0, Z = 0 }");
		mCLua->insert("Color", "{ R = 0, G = 0, B = 0 }");
		mCLua->insert("Anchored", "true");
		mCLua->insert("Archivable", "true");
		mCLua->insert("Locked", "false");
		mCLua->insert("Collide", "true");

		String func_proto = "func() Game.ComponentService.Destroy(";
		func_proto += "\"";
		func_proto += mName;
		func_proto += "\"";
		func_proto += ",";
		func_proto += "\"";
		func_proto += mParent;
		func_proto += "\"";
		func_proto += "); end";

		mCLua->insert("Destroy", func_proto.c_str());
	}

	ClassComponent::~ClassComponent() = default;

	const char* ClassComponent::parent() noexcept { return mParent.c_str(); }

	bool ClassComponent::should_update() noexcept { return true; }
	
	void ClassComponent::update()
	{
		this->get_attribute().locked(mCLua->index_as_bool("Locked"));

		if (this->get_attribute().is_locked())
			return;

		this->get_attribute().anchor(mCLua->index_as_bool("Anchored"));
		this->get_attribute().archivable(mCLua->index_as_bool("Archivable"));
		this->get_attribute().collide(mCLua->index_as_bool("Collide"));

		this->get_attribute().pos().X = mCLua->index_as_number("Position.X");
		this->get_attribute().pos().Y = mCLua->index_as_number("Position.Y");
		this->get_attribute().pos().Z = mCLua->index_as_number("Position.Z");

		this->get_attribute().scale().X = mCLua->index_as_number("Scale.X");
		this->get_attribute().scale().Y = mCLua->index_as_number("Scale.Y");
		this->get_attribute().scale().Z = mCLua->index_as_number("Scale.Z");

		this->get_attribute().color().R = mCLua->index_as_number("Color.R");
		this->get_attribute().color().G = mCLua->index_as_number("Color.G");
		this->get_attribute().color().B = mCLua->index_as_number("Color.B");
	}

	XAttribute& ClassComponent::get_attribute() noexcept { return mAttribute; }

	Lua::CLuaClass* ClassComponent::operator->() noexcept
	{
		XPLICIT_ASSERT(mCLua);
		return mCLua.get();
	}

	Lua::CLuaClass* ClassComponent::get() noexcept
	{
		return mCLua.get();
	}
}