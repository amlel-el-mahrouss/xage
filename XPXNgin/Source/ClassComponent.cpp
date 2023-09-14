/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "LuaScriptComponent.h"
#include "ClassComponent.h"

#include <NpMovementSharedEvent.h>

namespace XPX
{
	static const char* XPLICIT_CONNECT_SNIPPET = "function(self, UniqueName, Func) self.Slots[UniqueName] = Func end";
	static const char* XPLICIT_DISCONNECT_SNIPPET = "function(self, UniqueName) self.Slots[UniqueName] = nil; end";

	static const char* XPLICIT_UPDATE_SNIPPET = "function (self, Restrict) for k, v in pairs(self.Slots) do if k == Restrict then v(self); end end end";

	ClassComponent::ClassComponent(
		const char* parent,
		const char* name)
		:
		Lua::CLuaClass((String(parent) + "." + name)),
		mName(name),
		mParent(parent)
	{
		this->insert("ClassName", fmt::format("\'{}\'", mName).c_str());
		this->insert("Parent", mParent.c_str());

		this->insert("Update", XPLICIT_UPDATE_SNIPPET);

		this->insert("Anchored", "true");
		this->insert("Archivable", "false");
		this->insert("Locked", "false");
		this->insert("Collide", "true");

		this->insert("Slots", "{ }");

		//! Connect and disconnect methods
		//! Use this to connect specific functions to this class.
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
		ClassComponent::ClassComponent(parent, name)
	{
		this->pos() = position;
		this->scale() = size;
		this->color() = color;

		this->insert("Scale", "{ X = 0, Y = 0, Z = 0 }");
		this->insert("Position", "{ X = 0, Y = 0, Z = 0 }");
		this->insert("Rotation", "{ X = 0, Y = 0, Z = 0 }");
		this->insert("Color", "{ R = 0, G = 0, B = 0, A = 1 }");

		if (script)
		{
			this->script(ComponentSystem::get_singleton_ptr()->add<LuaScriptComponent>(script));
			XPLICIT_ASSERT(this->script());	
		}

		if (auto mov = EventSystem::get_singleton_ptr()->get<NpMovementSharedEvent>("NpMovementSharedEvent");
			mov)
		{
			mov->insert_node(this);
		}
	}

	ClassComponent::~ClassComponent()
	{
		if (auto mov = EventSystem::get_singleton_ptr()->get<NpMovementSharedEvent>("NpMovementSharedEvent");
			mov)
		{
			mov->remove_node(this);
		}
	}

	const char* ClassComponent::parent() noexcept { return mParent.c_str(); }
	
	bool ClassComponent::should_update() noexcept { return true; }

	void ClassComponent::update(ClassPtr _self)
	{
		ClassComponent* self = static_cast<ClassComponent*>(_self);

		self->collide(self->index_as_bool("Collide"));
		self->locked(self->index_as_bool("Locked"));
		self->archivable(self->index_as_bool("Archivable"));
		self->anchor(self->index_as_bool("Anchored"));
	
		if (!self->index_as_bool("Locked"))
		{
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
}