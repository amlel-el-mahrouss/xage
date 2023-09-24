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
		mParent(parent),
		PhysicsDelegate(nullptr)
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
		const Vector<NetworkFloat>& pos,
		const Vector<NetworkFloat>& size,
		const Color<NetworkFloat>& color,
		const char* script,
		const char* parent,
		const char* name)
		:
		ClassComponent::ClassComponent(parent, name)
	{
		this->pos() = (pos.X == 0 &&
			pos.Y == 0 &&
			pos.Z == 0) ? XPLICIT_ORIGIN : pos;

		this->scale() = size;
		this->color() = color;

		this->insert("Scale", "{ X = 0, Y = 0, Z = 0}");

		this->insert("Scale.X", std::to_string(size.X));
		this->insert("Scale.Y", std::to_string(size.Y));
		this->insert("Scale.Z", std::to_string(size.Z));

		this->insert("Position", "{ X = 0, Y = 0, Z = 0}");

		this->insert("Position.X", std::to_string(pos.X));
		this->insert("Position.Y", std::to_string(pos.Y));
		this->insert("Position.Z", std::to_string(pos.Z));

		this->insert("Rotation", "{ X = 0, Y = 0, Z = 0}");

		this->insert("Rotation.X", "0");
		this->insert("Rotation.Y", "0");
		this->insert("Rotation.Z", "0");

		this->insert("Color", "{ A = 0, R = 0, G = 0, B = 0}");

		this->insert("Color.R", std::to_string(color.R));
		this->insert("Color.G", std::to_string(color.G));
		this->insert("Color.B", std::to_string(color.B));
		this->insert("Color.A", std::to_string(color.A));

		if (script)
		{
			this->script(ComponentSystem::get_singleton_ptr()->add<LuaScriptComponent>(script));
			XPLICIT_ASSERT(this->script());

			if (this->script())
				this->script()->run_script();
		}
	}

	ClassComponent::~ClassComponent()
	{
		static auto mov = EventSystem::get_singleton_ptr()->get<NpMovementServerEvent>("NpMovementServerEvent");

		if (mov &&
			mov->remove_node(this))
		{
			//! tell ALL of the players to delete this class.
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

	const char* ClassComponent::parent() noexcept { return mParent.c_str(); }
	
	bool ClassComponent::should_update() noexcept { return true; }

	void ClassComponent::update(ClassPtr _self)
	{
		ClassComponent* self = static_cast<ClassComponent*>(_self);

		if (!self)
			return;

		self->collide(self->index_as_bool("Collide"));
		self->locked(self->index_as_bool("Locked"));
		self->archivable(self->index_as_bool("Archivable"));
		self->anchor(self->index_as_bool("Anchor"));

		self->alpha(self->index_as_number<int>("Color.A"));

		self->color().A = self->alpha();

		self->color().R = self->index_as_number<int>("Color.R");
		self->color().G = self->index_as_number<int>("Color.G");
		self->color().B = self->index_as_number<int>("Color.B");

		self->scale().X = self->index_as_number<NetworkFloat>("Scale.X");
		self->scale().Y = self->index_as_number<NetworkFloat>("Scale.Y");
		self->scale().Z = self->index_as_number<NetworkFloat>("Scale.Z");

		self->rotation().X = self->index_as_number<NetworkFloat>("Rotation.X");
		self->rotation().Y = self->index_as_number<NetworkFloat>("Rotation.Y");
		self->rotation().Z = self->index_as_number<NetworkFloat>("Rotation.Z");

		self->pos().X = self->index_as_number<NetworkFloat>("Position.X");
		self->pos().Y = self->index_as_number<NetworkFloat>("Position.Y");
		self->pos().Z = self->index_as_number<NetworkFloat>("Position.Z");
	}
}