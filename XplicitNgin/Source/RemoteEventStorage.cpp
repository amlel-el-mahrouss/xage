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
#define XPLICIT_REMOTE_EVENTS_CNT (7)

	inline const char* XPLICIT_REMOTE_EVENTS[XPLICIT_REMOTE_EVENTS_CNT] = {
		"Spawn",
		"Death",
		"Move",
		"Login",
		"Logoff",
		"ClassDestroy",
		"ClassCreate",
	};

	static const char* XPLICIT_CONNECT_SNIPPET = "function(self, Name, Func) self.Slots[Name] = Func end";
	static const char* XPLICIT_DISCONNECT_SNIPPET = "function(self, Name) self.Slots[Name] = nil; end";

	RemoteEventStorage::RemoteEventStorage()
		: mClient(nullptr), mServer(nullptr), Lua::CLuaClass("RemoteEventStorage")
	{
		for (size_t i = 0; i < XPLICIT_REMOTE_EVENTS_CNT; ++i)
		{
			this->insert(XPLICIT_REMOTE_EVENTS[i], "{}");
			this->insert((String(XPLICIT_REMOTE_EVENTS[i]) + ".Slots").c_str(), "{}");
			this->insert((String(XPLICIT_REMOTE_EVENTS[i]) + ".Connect").c_str(), XPLICIT_CONNECT_SNIPPET);
			this->insert((String(XPLICIT_REMOTE_EVENTS[i]) + ".Disconnect").c_str(), XPLICIT_DISCONNECT_SNIPPET);
		}
	}

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
		RemoteEventStorage* event_storage = (RemoteEventStorage*)_this;
		
		for (size_t i = 0; i < XPLICIT_REMOTE_EVENTS_CNT; ++i)
		{

		}
	}
}