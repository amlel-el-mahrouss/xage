/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "RemoteEventStorage.h"

namespace XPX
{
	static const char* XPLICIT_CONNECT_SNIPPET = "function(self, Func) self.Slots[#self.Slots + 1] = Func; self.Cnt = #self.Slots; end";
	static const char* XPLICIT_STEP_UPDATE_SNIPPET = "function(self) self.Cnt = self.Cnt + 1; return self.Slots[self.Cnt - 1]; end";
	static const char* XPLICIT_DISCONNECT_SNIPPET = "function(self, Index) self.Slots[Index] = nil; end";

	static int lua_attachRemoveEvent(lua_State* L)
	{
		RemoteEventStorage* event_storage = (RemoteEventStorage*)lua_topointer(L, 1);

		if (lua_gettable(L, 2) == LUA_OK)
		{
			if (lua_tostring(L, -1))
			{
				event_storage->insert(lua_tostring(L, -1), "{}");
			}

			event_storage->insert((String(lua_tostring(L, -1)) + ".Cnt").c_str(), "0");
			event_storage->insert((String(lua_tostring(L, -1)) + ".Slots").c_str(), "{}");
			event_storage->insert((String(lua_tostring(L, -1)) + ".Step").c_str(), XPLICIT_STEP_UPDATE_SNIPPET);
			event_storage->insert((String(lua_tostring(L, -1)) + ".Connect").c_str(), XPLICIT_CONNECT_SNIPPET);
			event_storage->insert((String(lua_tostring(L, -1)) + ".Disconnect").c_str(), XPLICIT_DISCONNECT_SNIPPET);

			lua_pushboolean(L, true);
			return 1;
		}

		lua_pushboolean(L, false);
		return 1;
	}

	RemoteEventStorage::RemoteEventStorage()
		: mClient(nullptr), mServer(nullptr), Lua::CLuaClass("world.RemoteEventStorage")
	{
		this->insert(CLUA_USER_DATA_SYMBOL, this, Lua::ReadOnly); // read only by default.
		this->insert("Attach", "function(self, Table) return xpxAttachRemoteEvent(self.UserData.__CxxData, Table); end"); // to actually call this.
	}

	RemoteEventStorage::~RemoteEventStorage() = default;

	RemoteEventStorage::RemoteEventStorage(NetworkComponent* client)
		: RemoteEventStorage()
	{
		XPLICIT_ASSERT(client);
		mClient = client;
	}

	RemoteEventStorage::RemoteEventStorage(NetworkServerComponent* server)
		: RemoteEventStorage()
	{
		XPLICIT_ASSERT(server);
		mServer = server;
	}

	bool RemoteEventStorage::should_update() { return true; }

	void RemoteEventStorage::update(ClassPtr _this)
	{
		RemoteEventStorage* self = (RemoteEventStorage*)_this;

		if (!self)
			return;

		for (size_t event_idx = 0; event_idx < self->count(); ++event_idx)
		{
			if (self->mServer &&
				self->symbols()[event_idx].second.find(CLUA_USER_DATA_SYMBOL) == String::npos ||
				self->symbols()[event_idx].second.find(CLUA_IDENT) == String::npos)
			{
				luaL_dostring(self->state(), (String("string.dump(return world.RemoteEventStorage.") + self->symbols()[event_idx].second + ":Step())").c_str());
				const char* bytecode = lua_tostring(self->state(), -1);

				if (bytecode)
				{
					for (size_t i = 0; i < self->mServer->size(); ++i)
					{
						if (bytecode)
						{
							self->mServer->get(i)->packet.channel = XPLICIT_CHANNEL_DATA;

							memset(self->mServer->get(i)->packet.replicas[XPLICIT_REPLICA_EVENT], 0, XPLICIT_NETWORK_BUF_SZ);

							lua_len(self->state(), -1);
							memcpy(self->mServer->get(i)->packet.replicas[XPLICIT_REPLICA_EVENT], bytecode, lua_tonumber(self->state(), -1));

							self->mServer->get(i)->packet.cmd[XPLICIT_NETWORK_CMD_REPL] = NETWORK_CMD_REPL;

							XPX::NetworkServerContext::send(self->mServer, self->mServer->get(i));

							memset(self->mServer->get(i)->packet.replicas[XPLICIT_REPLICA_EVENT], 0, XPLICIT_NETWORK_BUF_SZ);
						}
					}
				}
			}
		}

		if (self->mClient && 
			self->mClient->get().cmd[XPLICIT_NETWORK_CMD_REPL] == NETWORK_CMD_REPL)
		{
			if (luaL_loadbufferx(Lua::CLuaStateManager::get_singleton_ptr()->state(),
				self->mClient->get().replicas[XPLICIT_REPLICA_EVENT], XPLICIT_NETWORK_BUF_SZ, "bytecode", "bt") ||
				lua_pcall(Lua::CLuaStateManager::get_singleton_ptr()->state(), 0, LUA_MULTRET, 0))
			{
				XPLICIT_CRITICAL(lua_tostring(Lua::CLuaStateManager::get_singleton_ptr()->state(), -1));
			}

			self->mClient->get().cmd[XPLICIT_NETWORK_CMD_REPL] = NETWORK_CMD_INVALID;
		}
	}
}