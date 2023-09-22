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

#include <NpMovementServerEvent.h>

namespace XPX
{
	static String XPLICIT_CONNECT_SNIPPET = "function(self, UniqueName, Func) self.Slots[UniqueName] = Func end";
	static String XPLICIT_DISCONNECT_SNIPPET = "function(self, UniqueName) self.Slots[UniqueName] = nil; end";
	static String XPLICIT_CLONE_SNIPPET = "function(self) return false; end";
	static String XPLICIT_UPDATE_SNIPPET = "function (self, Restrict) for k, v in pairs(self.Slots) do if k == Restrict then v(self); end end end";

	ClassComponent::ClassComponent(
		const char* parent,
		const char* name)
		:
		Lua::CLuaClass(fmt::format("{}{}{}", parent, ".", name)),
		mName(name),
		mParent(parent)
	{
		this->insert("ClassName", fmt::format("\'{}\'", mName));
		this->insert("Parent", mParent.c_str());

		this->insert("Update", XPLICIT_UPDATE_SNIPPET);

		this->insert("Archivable", "false");

		this->insert("Anchor", "false");
		this->insert("Locked", "false");
		this->insert("Collide", "true");

		this->insert("Slots", "{ }");

		//! Connect and disconnect methods
		//! Use this to connect specific functions to this class.
		this->insert("Connect", XPLICIT_CONNECT_SNIPPET);
		this->insert("Clone", XPLICIT_CLONE_SNIPPET);
		this->insert("Disconnect", XPLICIT_DISCONNECT_SNIPPET);
	}

	ClassComponent::ClassComponent(
		const Vector<NetworkFloat>& position,
		const Vector<NetworkFloat>& size,
		const Color<NetworkFloat>& color,
		const char* script,
		const char* parent,
		const char* name)
		:
		ClassComponent::ClassComponent(parent, name)
	{
		this->pos() = position;
		this->scale() = size;
		this->color() = color;

		this->insert("Scale", "{ X =  0, Y = 0, Z = 0 }");
		this->insert("Position", "{ X =  0, Y = 0, Z = 0 }");
		this->insert("Rotation", "{ X =  0, Y = 0, Z = 0 }");

		this->insert("Rotation", "{ X = 0, Y = 0, Z = 0 }");
		this->insert("Color", "{ R = 1, G = 1, B = 1, A = 1 }");

		if (script)
		{
			this->script(ComponentSystem::get_singleton_ptr()->add<LuaScriptComponent>(script));
			XPLICIT_ASSERT(this->script());

			if (this->script())
				this->script()->run_script();
		}

		if (auto mov = EventSystem::get_singleton_ptr()->get<NpMovementServerEvent>("NpMovementServerEvent");
			mov)
		{
			mov->insert_node(this);
		}
	}

	ClassComponent::~ClassComponent()
	{
		if (auto mov = EventSystem::get_singleton_ptr()->get<NpMovementServerEvent>("NpMovementServerEvent");
			mov)
		{
			if (mov->remove_node(this))
			{
				NetworkPacket pckt{};

				pckt.channel = XPLICIT_CHANNEL_PHYSICS;
				pckt.version = XPLICIT_NETWORK_VERSION;

				pckt.magic[0] = XPLICIT_NETWORK_MAG_0;
				pckt.magic[1] = XPLICIT_NETWORK_MAG_1;
				pckt.magic[2] = XPLICIT_NETWORK_MAG_2;

				memcpy(pckt.additional_data, this->mName.data(), this->mName.size());

				pckt.cmd[XPLICIT_NETWORK_CMD_DESTROY] = NETWORK_CMD_DESTROY;

				static auto server = ComponentSystem::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent");

				NetworkServerContext::send_all(server, &pckt);
			}
		}
	}

	const char* ClassComponent::parent() noexcept { return mParent.c_str(); }
	
	bool ClassComponent::should_update() noexcept { return false; }

	void ClassComponent::update(ClassPtr _self)
	{
		ClassComponent* self = static_cast<ClassComponent*>(_self);

		if (!IsValidHeapPtr(self))
			return;

		self->collide(self->index_as_bool("Collide"));
		self->locked(self->index_as_bool("Locked"));
		self->archivable(self->index_as_bool("Archivable"));

		self->alpha(self->index_as_number<int>("Color.A"));
		self->color().A = self->alpha();

		self->color().R = self->index_as_number<int>("Color.R");
		self->color().G = self->index_as_number<int>("Color.G");
		self->color().B = self->index_as_number<int>("Color.B");

		self->scale().X = self->index_as_number<NetworkFloat>("Scale.X");
		self->scale().Y = self->index_as_number<NetworkFloat>("Scale.Y");
		self->scale().Z = self->index_as_number<NetworkFloat>("Scale.Z");

		self->pos().X = self->index_as_number<NetworkFloat>("Position.X");
		self->pos().Y = self->index_as_number<NetworkFloat>("Position.Y");
		self->pos().Z = self->index_as_number<NetworkFloat>("Position.Z");
	}
}