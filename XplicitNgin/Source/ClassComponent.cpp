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
	static const char* XPLICIT_CONNECT_SNIPPET = "function(self, Func)"
		"\treturn table.insert(self, { Func = Func })"
		"end";

	static const char* XPLICIT_DISCONNECT_SNIPPET =
		"function(self, Index)"
		"self[Index] = nil;"
		"end";

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
		this->insert("Alpha", "1");
		this->insert("Anchored", "true");
		this->insert("Archivable", "true");
		this->insert("Locked", "true");
		this->insert("Collide", "true");

		String func_proto = "function() World.ClassService.Destroy("
							"\"";
		
		func_proto += mName;
		func_proto += ""
					","
					"\"";

		func_proto += mParent;
		
		func_proto += "\"";
		func_proto += "); end";

		this->insert("Destroy", func_proto.c_str());

		this->insert("Connect", XPLICIT_CONNECT_SNIPPET);
		this->insert("Disconnect", XPLICIT_DISCONNECT_SNIPPET);
	}

	ClassComponent::~ClassComponent() = default;

	const char* ClassComponent::parent() noexcept { return mParent.c_str(); }
	
	void ClassComponent::update(ClassPtr _this)
	{
		ClassComponent* this_ptr = static_cast<ClassComponent*>(_this);

		this_ptr->get_attribute().collide(this_ptr->index_as_bool("Collide"));
		this_ptr->get_attribute().locked(this_ptr->index_as_bool("Locked"));
		this_ptr->get_attribute().archivable(this_ptr->index_as_bool("Archivable"));
		this_ptr->get_attribute().anchor(this_ptr->index_as_bool("Anchored"));

		this_ptr->get_attribute().alpha(this_ptr->index_as_number<float>("Alpha"));

		this_ptr->get_attribute().color().R = this_ptr->index_as_number<float>("Color.R");
		this_ptr->get_attribute().color().G = this_ptr->index_as_number<float>("Color.G");
		this_ptr->get_attribute().color().B = this_ptr->index_as_number<float>("Color.B");

		this_ptr->get_attribute().scale().X = this_ptr->index_as_number<float>("Scale.X");
		this_ptr->get_attribute().scale().Y = this_ptr->index_as_number<float>("Scale.Y");
		this_ptr->get_attribute().scale().Z = this_ptr->index_as_number<float>("Scale.Z");

		this_ptr->get_attribute().pos().X = this_ptr->index_as_number<float>("Position.X");
		this_ptr->get_attribute().pos().Y = this_ptr->index_as_number<float>("Position.Y");
		this_ptr->get_attribute().pos().Z = this_ptr->index_as_number<float>("Position.Z");
	}

	XAttribute& ClassComponent::get_attribute() noexcept { return mAttribute; }
}