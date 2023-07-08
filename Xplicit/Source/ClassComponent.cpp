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
		Lua::CLuaClass((String(parent) + name)),
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
		this->insert("Locked", "false");
		this->insert("Collide", "true");

		String func_proto = "func() Game.ComponentService.Destroy(";
		func_proto += "\"";
		func_proto += mName;
		func_proto += "\"";
		func_proto += ",";
		func_proto += "\"";
		func_proto += mParent;
		func_proto += "\"";
		func_proto += "); end";

		this->insert("Destroy", func_proto.c_str());
	}

	ClassComponent::~ClassComponent() = default;

	const char* ClassComponent::parent() noexcept { return mParent.c_str(); }
	
	void ClassComponent::update(void* _this)
	{
		ClassComponent* this_ptr = (ClassComponent*)_this;

		this_ptr->get_attribute().locked(this_ptr->index_as_bool("Locked"));

		if (this_ptr->get_attribute().is_locked())
			return;

		this_ptr->get_attribute().anchor(this_ptr->index_as_bool("Anchored"));
		this_ptr->get_attribute().archivable(this_ptr->index_as_bool("Archivable"));
		this_ptr->get_attribute().collide(this_ptr->index_as_bool("Collide"));

		this_ptr->get_attribute().pos().X = this_ptr->index_as_number("Position.X");
		this_ptr->get_attribute().pos().Y = this_ptr->index_as_number("Position.Y");
		this_ptr->get_attribute().pos().Z = this_ptr->index_as_number("Position.Z");

		this_ptr->get_attribute().scale().X = this_ptr->index_as_number("Scale.X");
		this_ptr->get_attribute().scale().Y = this_ptr->index_as_number("Scale.Y");
		this_ptr->get_attribute().scale().Z = this_ptr->index_as_number("Scale.Z");

		this_ptr->get_attribute().color().R = this_ptr->index_as_number("Color.R");
		this_ptr->get_attribute().color().G = this_ptr->index_as_number("Color.G");
		this_ptr->get_attribute().color().B = this_ptr->index_as_number("Color.B");
	}

	XAttribute& ClassComponent::get_attribute() noexcept { return mAttribute; }
}