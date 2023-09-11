/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "XPXNginCore.h"

#include <CLua.hpp>

#include "NetworkComponent.h"
#include "NetworkServerComponent.h"

namespace XPX
{
	class XPLICIT_API RemoteEventStorage final : public Component, public Lua::CLuaClass
	{
	private:
		RemoteEventStorage();

	public:
		RemoteEventStorage(NetworkComponent* client);
		RemoteEventStorage(NetworkServerComponent* server);

	public:
		~RemoteEventStorage();

	public:
		XPLICIT_COPY_DEFAULT(RemoteEventStorage);

	public:
		const char* name() noexcept override { return "RemoteEventStorage"; }

	public:
		static bool should_update();
		static void update(ClassPtr self);

	private:
		NetworkServerComponent* mServer;
		NetworkComponent* mClient;

	};
}