/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "LocalReplicationComponent.h"

#include <CommonEngine.h>
#include <lua.hpp>
#include <Uri.h>

#ifndef XPLICIT_XASSET_IDENT
#	define XPLICIT_XASSET_IDENT ("xasset")
#endif // ifndef XPLICIT_XASSET_IDENT

namespace Xplicit::Player
{
	COMPONENT_TYPE LocalReplicationComponent::type() noexcept { return COMPONENT_REPLICATION; }

	const char* LocalReplicationComponent::name() noexcept { return "LocalReplicationManager"; }

	bool LocalReplicationComponent::should_update() noexcept { return true; }

	/*
	 *	This update function takes care of these 3 events:
	 * 
	 *	 - Create
	 *   - Update
	 *   - Destroy
	 */

	void LocalReplicationComponent::update()
	{
		if (!mNetwork)
			return;
		
		NetworkPacket packet{};

		if (!mNetwork->read(packet))
			return;
		
		if (packet.cmd[XPLICIT_REPL_CREATE] == NETWORK_REPL_CMD_CREATE)
		{
			switch (packet.id)
			{
			case COMPONENT_ID_SCRIPT:
			{
				auto script = Utils::UriParser(packet.buffer);
				script /= packet.buffer;

				if (script.protocol() != XPLICIT_XASSET_IDENT)
					return;

				break;
			}
			case COMPONENT_ID_SOUND:
			{
				auto sound = Utils::UriParser(packet.buffer);
				sound /= packet.buffer;

				if (sound.protocol() != XPLICIT_XASSET_IDENT)
					return;
				
				break;
			}
			default:
				break;
			}
		}
		else if (packet.cmd[XPLICIT_REPL_DESTROY] == NETWORK_REPL_CMD_DESTROY)
		{
			switch (packet.id)
			{
			case COMPONENT_ID_SCRIPT:
				break;
			case COMPONENT_ID_SOUND:
				break;
			default:
				break;
			}
		}
	}
}