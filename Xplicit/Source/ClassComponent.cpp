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
		Lua::CLuaClass((String(parent) + name).c_str()),
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

		this->insert("Scale", "{ X = 0, Y = 0, Z = 0 }");
		this->insert("Position", "{ X = 0, Y = 0, Z = 0 }");
		this->insert("Color", "{ R = 0, G = 0, B = 0 }");
		this->insert("Anchored", "true");
		this->insert("Archivable", "true");
		this->insert("Locked", "true");
		this->insert("Collide", "true");

		String func_proto = "func(self) _G.Game.ComponentService.Destroy(\"";
		func_proto += mName;
		func_proto += "\"); end";

		this->insert("Destroy", func_proto.c_str());
	}

	ClassComponent::~ClassComponent() = default;

	const char* ClassComponent::parent() noexcept { return mParent.c_str(); }

	bool ClassComponent::should_update() noexcept { return true; }
	
	void ClassComponent::update()
	{
		this->get_attribute().anchor((bool)this->index_as_number("Anchored"));

		if (this->get_attribute().is_locked())
			return;

		this->get_attribute().locked((bool)this->index_as_number("Locked"));
		this->get_attribute().archivable((bool)this->index_as_number("Archivable"));
		this->get_attribute().collide((bool)this->index_as_number("Collide"));

		this->get_attribute().pos().X = this->index_as_number("Position.X");
		this->get_attribute().pos().Y = this->index_as_number("Position.Y");
		this->get_attribute().pos().Z = this->index_as_number("Position.Z");

		this->get_attribute().scale().X = this->index_as_number("Scale.X");
		this->get_attribute().scale().Y = this->index_as_number("Scale.Y");
		this->get_attribute().scale().Z = this->index_as_number("Scale.Z");

		this->get_attribute().color().R = this->index_as_number("Color.R");
		this->get_attribute().color().G = this->index_as_number("Color.G");
		this->get_attribute().color().B = this->index_as_number("Color.B");
	}

	XAttribute& ClassComponent::get_attribute() noexcept { return mAttribute; }
}