/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "LuaScriptComponent.h"
#include "ClassComponent.h"

namespace XPX
{
	static const char* XPLICIT_CONNECT_SNIPPET = "function(self, Name, Func) self.Slots[Name] = Func end";
	static const char* XPLICIT_DISCONNECT_SNIPPET = "function(self, Name) self.Slots[Name] = nil; end";

	static const String XPLICIT_DESTROY_SNIPPET(const String& name, const String& parent) noexcept
	{
		String func_proto = std::format("function(self) XPXDestroyClass(\"{}\", \"{}\"); end",
			name, parent);

		return func_proto;
	}

	ClassComponent::ClassComponent(
		const char* parent,
		const char* name)
		:
		Lua::CLuaClass((String(parent) + "." + name)),
		mName(name),
		mParent(parent)
	{
		this->insert("Name", std::format("\'{}\'", mName).c_str());
		this->insert("Parent", mParent.c_str());

		this->insert("Anchored", "true");
		this->insert("Archivable", "false");
		this->insert("Locked", "false");
		this->insert("Collide", "true");

		this->insert("Destroy", XPLICIT_DESTROY_SNIPPET(mName, mParent).c_str());

		this->insert("Slots", "{ }");

		//! Connect and disconnect methods
		//! Use this to connect specific function to this class.
		this->insert("Connect", XPLICIT_CONNECT_SNIPPET);
		this->insert("Disconnect", XPLICIT_DISCONNECT_SNIPPET);
	}

	ClassComponent::ClassComponent(
		const Vector<float>& position, 
		const Vector<float>& size, 
		const Color<float>& color,
		const char* script,
		const char* parent,
		const char* name)
		: 
		Lua::CLuaClass((String(parent) + "." + name)),
		mName(name),
		mParent(parent)
	{
		this->insert("Name", mName.c_str());
		this->insert("Parent", mParent.c_str());

		this->pos() = position;
		this->scale() = size;
		this->color() = color;

		this->insert("Scale", "{ X = 0, Y = 0, Z = 0 }");
		this->insert("Position", "{ X = 0, Y = 0, Z = 0 }");
		this->insert("Rotation", "{ X = 0, Y = 0, Z = 0 }");
		this->insert("Color", "{ R = 0, G = 0, B = 0, A = 1 }");

		this->insert("Slots", "{ }");

		this->insert("Anchored", "true");
		this->insert("Archivable", "false");
		this->insert("Locked", "false");
		this->insert("Collide", "true");

		this->insert("Destroy", XPLICIT_DESTROY_SNIPPET(mName, mParent).c_str());

		//! Connect and disconnect methods
		//! Use this to connect specific function to this class.
		this->insert("Connect", XPLICIT_CONNECT_SNIPPET);
		this->insert("Disconnect", XPLICIT_DISCONNECT_SNIPPET);

		if (script)
		{
			this->script(ComponentSystem::get_singleton_ptr()->add<LuaScriptComponent>(script));
			XPLICIT_ASSERT(this->script());	
		}
	}

	ClassComponent::~ClassComponent() = default;

	const char* ClassComponent::parent() noexcept { return mParent.c_str(); }
	
	bool ClassComponent::should_update() noexcept { return true; }

	void ClassComponent::update(ClassPtr _self)
	{
		ClassComponent* self = static_cast<ClassComponent*>(_self);

		self->collide(self->index_as_bool("Collide"));
		self->locked(self->index_as_bool("Locked"));
		self->archivable(self->index_as_bool("Archivable"));
		self->anchor(self->index_as_bool("Anchored"));
	
		self->alpha(self->index_as_number<float>("Color.A"));

		self->color().A = self->alpha();

		self->color().R = self->index_as_number<float>("Color.R");
		self->color().G = self->index_as_number<float>("Color.G");
		self->color().B = self->index_as_number<float>("Color.B");

		self->scale().X = self->index_as_number<float>("Scale.X");
		self->scale().Y = self->index_as_number<float>("Scale.Y");
		self->scale().Z = self->index_as_number<float>("Scale.Z");

		self->pos().X = self->index_as_number<float>("Position.X");
		self->pos().Y = self->index_as_number<float>("Position.Y");
		self->pos().Z = self->index_as_number<float>("Position.Z");
	}
}