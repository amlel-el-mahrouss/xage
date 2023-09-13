/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "RemoteEventStorage.h"

#define XPLICIT_REMOTE_EVENTS_CNT (6)

namespace XPX
{
	const char* XPLICIT_REMOTE_EVENTS[XPLICIT_REMOTE_EVENTS_CNT] = {
		"Spawn",
		"Death",
		"Move",
		"Login",
		"Damage",
		"Logoff",
	};

	static const char* XPLICIT_CONNECT_SNIPPET = "function(self, Func) self.Slots[#self.Slots + 1] = Func; self.Cnt = #self.Slots; end";
	static const char* XPLICIT_STEP_UPDATE_SNIPPET = "function(self) self.Cnt = self.Cnt + 1; return self.Slots[self.Cnt - 1]; end";
	static const char* XPLICIT_DISCONNECT_SNIPPET = "function(self, Index) self.Slots[Index] = nil; end";

	RemoteEventStorage::RemoteEventStorage()
		: mClient(nullptr), mServer(nullptr), Lua::CLuaClass("World.RemoteEventStorage")
	{
		for (size_t i = 0; i < XPLICIT_REMOTE_EVENTS_CNT; ++i)
		{
			this->insert(XPLICIT_REMOTE_EVENTS[i], "{}");
			this->insert((String(XPLICIT_REMOTE_EVENTS[i]) + ".Cnt").c_str(), "0");
			this->insert((String(XPLICIT_REMOTE_EVENTS[i]) + ".Flush").c_str(), "false");
			this->insert((String(XPLICIT_REMOTE_EVENTS[i]) + ".Slots").c_str(), "{}");
			this->insert((String(XPLICIT_REMOTE_EVENTS[i]) + ".Step").c_str(), XPLICIT_STEP_UPDATE_SNIPPET);
			this->insert((String(XPLICIT_REMOTE_EVENTS[i]) + ".Connect").c_str(), XPLICIT_CONNECT_SNIPPET);
			this->insert((String(XPLICIT_REMOTE_EVENTS[i]) + ".Disconnect").c_str(), XPLICIT_DISCONNECT_SNIPPET);
		}
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

		for (size_t event_idx = 0; event_idx < (XPLICIT_REMOTE_EVENTS_CNT); ++event_idx)
		{
			if (self->mServer &&
				self->index_as_bool(std::format("{}.Flush", XPLICIT_REMOTE_EVENTS[event_idx]).c_str()))
			{
				luaL_dostring(self->state(), (String("return World.RemoteEventStorage.") + XPLICIT_REMOTE_EVENTS[event_idx] + ":Step()").c_str());

				char bytecode[XPLICIT_NETWORK_BUF_SZ];
				memset(bytecode, 0, XPLICIT_NETWORK_BUF_SZ);

				lua_Reader reader = [](lua_State* L,
					void* data,
					size_t* size) -> const char* {
						char* blk = new char[*size];
						memcpy(blk, data, *size);

						return blk;
				};

				lua_Writer writer = [](lua_State* L,
					const void* data,
					size_t size, void* ud) -> int {
						memcpy(ud, data, size);
						char* ud_str = (char*)((uintptr_t)ud + size + 1);

						ud_str[0] = 'E';
						ud_str[1] = 'O';
						ud_str[2] = 'B';

						return 0;
				};

				if (lua_load(self->state(), reader, bytecode, "bytecode", "bt") == LUA_OK)
				{
					lua_dump(self->state(), writer, bytecode, true);

					for (size_t i = 0; i < self->mServer->size(); ++i)
					{
						if (bytecode)
						{
							self->mServer->get(i)->packet.channel = XPLICIT_CHANNEL_DATA;

							memset(self->mServer->get(i)->packet.replicas[XPLICIT_REPLICA_EVENT], 0, XPLICIT_NETWORK_BUF_SZ);

							for (size_t bytecode_index = 0; bytecode_index < XPLICIT_NETWORK_BUF_SZ; ++bytecode_index)
							{
								if (bytecode[bytecode_index] != 'E')
								{
									self->mServer->get(i)->packet.replicas[XPLICIT_REPLICA_EVENT][bytecode_index] = bytecode[bytecode_index];
								}
								else
								{
									if (bytecode[bytecode_index++] == 'O' &&
										bytecode[bytecode_index++] == 'B')
										break;
								}
							}

							self->mServer->get(i)->packet.cmd[XPLICIT_NETWORK_CMD_REPL] = NETWORK_CMD_REPL;

							XPX::NetworkServerContext::send(self->mServer, self->mServer->get(i));

							memset(self->mServer->get(i)->packet.replicas[XPLICIT_REPLICA_EVENT], 0, XPLICIT_NETWORK_BUF_SZ);
						}
					}

				}

				self->assign(std::format("{}.Flush", XPLICIT_REMOTE_EVENTS[event_idx]).c_str(), "false");
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