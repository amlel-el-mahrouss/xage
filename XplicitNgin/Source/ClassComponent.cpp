/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "ClassComponent.h"

namespace Xplicit
{
	static const char* XPLICIT_CONNECT_SNIPPET = "function(self, Name, Func) self.Slots[Name] = Func end";
	static const char* XPLICIT_DISCONNECT_SNIPPET = "function(self, Name) self.Slots[Name] = nil; end";

	const String XPLICIT_DESTROY_SNIPPET(const String& name, const String& parent) noexcept
	{
		String func_proto = "function(self) World.ClassService.Destroy(";

		func_proto += "\"";
		func_proto += name;
		func_proto += "\"";
		func_proto += ",";
		func_proto += "\"";
		func_proto += parent;
		func_proto += "\"";
		func_proto += "); end";

		return func_proto;
	}

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


		this->insert("Scale", "{ X = 0, Y = 0, Z = 0 }");
		this->insert("Position", "{ X = 0, Y = 0, Z = 0 }");
		this->insert("Color", "{ R = 0, G = 0, B = 0 }");

		this->insert("Alpha", "1.0");
		this->insert("Slots", "{ }");

		this->insert("Anchored", "true");
		this->insert("Archivable", "false");
		this->insert("Locked", "false");
		this->insert("Collide", "true");

		this->insert("Name", std::format("\'{}\'", mName).c_str());
		this->insert("Destroy", XPLICIT_DESTROY_SNIPPET(mName, mParent).c_str());

		//! Connect and disconnect methods
		//! Use this to connect specific function to this class.
		this->insert("Connect", XPLICIT_CONNECT_SNIPPET);
		this->insert("Disconnect", XPLICIT_DISCONNECT_SNIPPET);

		if (script)
		{
			Lua::CLuaStateManager::get_singleton_ptr()->run_string(std::format("_G.Script.{} = {}", mName, "{}").c_str());
			Lua::CLuaStateManager::get_singleton_ptr()->run_string(std::format("_G.Script.{}.Parent = {}", mName, std::format("_G.{}{}", mParent, mName)).c_str());

			mAttribute.script(ComponentSystem::get_singleton_ptr()->add<LuaScriptComponent>(script, true));
			XPLICIT_ASSERT(mAttribute.script());	
		}
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