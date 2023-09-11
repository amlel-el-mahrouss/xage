/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "RemoteEventStorage.h"

namespace XPX
{
#define XPLICIT_REMOTE_EVENTS_CNT (6)

	inline const char* XPLICIT_REMOTE_EVENTS[XPLICIT_REMOTE_EVENTS_CNT] = {
		"Spawn",
		"Death",
		"Move",
		"Login",
		"Damage",
		"Logoff",
	};

	static const char* XPLICIT_CONNECT_SNIPPET = "function(self, Name, Func) self.Slots[Name] = Func end";
	static const char* XPLICIT_DISCONNECT_SNIPPET = "function(self, Name) self.Slots[Name] = nil; end";

	static const char* XPLICIT_UPDATE_SNIPPET = "function (self) for _, v in pairs(self.Slots) do v(self); end end";

	RemoteEventStorage::RemoteEventStorage()
		: mClient(nullptr), mServer(nullptr), Lua::CLuaClass("RemoteEventStorage")
	{
		for (size_t i = 0; i < XPLICIT_REMOTE_EVENTS_CNT; ++i)
		{
			this->insert(XPLICIT_REMOTE_EVENTS[i], "{}");
			this->insert((String(XPLICIT_REMOTE_EVENTS[i]) + ".Slots").c_str(), "{}");
			this->insert((String(XPLICIT_REMOTE_EVENTS[i]) + ".ShouldUpdate").c_str(), "false");
			this->insert((String(XPLICIT_REMOTE_EVENTS[i]) + ".Update").c_str(), XPLICIT_UPDATE_SNIPPET);
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
		
		for (size_t i = 0; i < XPLICIT_REMOTE_EVENTS_CNT; ++i)
		{
			if (self->mServer && 
				self->index_as_bool((String(XPLICIT_REMOTE_EVENTS[i]) + ".ShouldUpdate").c_str()))
			{
				auto bytecode = self->call_method((String(XPLICIT_REMOTE_EVENTS[i]) + ":Update").c_str());

				for (size_t i = 0; i < self->mServer->size(); ++i)
				{
					self->mServer->get(i)->packet.channel = XPLICIT_CHANNEL_DATA;

					memset(self->mServer->get(i)->packet.replicas[XPLICIT_REPLICA_EVENT], 0, XPLICIT_NETWORK_BUF_SZ);
					memcpy(self->mServer->get(i)->packet.replicas[XPLICIT_REPLICA_EVENT], bytecode.c_str(), bytecode.size());

					XPX::NetworkServerContext::send_all(self->mServer);
				}

				self->assign((String(XPLICIT_REMOTE_EVENTS[i]) + ".ShouldUpdate").c_str(), "false");

			}
			else if (self->mClient &&
				self->index_as_bool((String(XPLICIT_REMOTE_EVENTS[i]) + ".ShouldUpdate").c_str()))
			{
				Lua::CLuaStateManager::get_singleton_ptr()->run_string(self->mClient->get().buffer);
			}
		}
	}
}