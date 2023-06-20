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

namespace Xplicit::Player
{
	COMPONENT_TYPE LocalReplicationComponent::type() noexcept { return COMPONENT_LOGIC; }

	const char* LocalReplicationComponent::name() noexcept { return "LocalReplicationManager"; }

	bool LocalReplicationComponent::should_update() noexcept { return true; }

	void LocalReplicationComponent::update()
	{
		if (!mNetwork)
			return;

		// first acknowledge.
		auto& packet = mNetwork->get();
		packet.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;

		if (packet.cmd[XPLICIT_REPL_CREATE] == NETWORK_REPL_CMD_CREATE)
		{
			switch (packet.id)
			{
			case COMPONENT_ID_SCRIPT:
				break;
			case COMPONENT_ID_TEXTURE:
				break;
			case COMPONENT_ID_TOOL:
				break;
			case COMPONENT_ID_SOUND:
				break;
			case COMPONENT_ID_SHAPE:
				break;
			}

			packet.id = -1;
			packet.cmd[XPLICIT_REPL_CREATE] = NETWORK_CMD_INVALID;
		}
		else if (packet.cmd[XPLICIT_REPL_UPDATE] == NETWORK_REPL_CMD_UPDATE)
		{

			packet.cmd[XPLICIT_REPL_UPDATE] = NETWORK_CMD_INVALID;
		}
		else if (packet.cmd[XPLICIT_REPL_DESTROY] == NETWORK_REPL_CMD_DESTROY)
		{
			switch (packet.id)
			{
			case COMPONENT_ID_SCRIPT:
				break;
			case COMPONENT_ID_TEXTURE:
				break;
			case COMPONENT_ID_TOOL:
				break;
			case COMPONENT_ID_SOUND:
				break;
			case COMPONENT_ID_SHAPE:
				break;
			}

			packet.id = -1;
			packet.cmd[XPLICIT_REPL_DESTROY] = NETWORK_CMD_INVALID;
		}
		else if (packet.cmd[XPLICIT_REPL_TOUCH] == NETWORK_REPL_CMD_TOUCH)
		{

			packet.cmd[XPLICIT_REPL_TOUCH] = NETWORK_CMD_INVALID;
		}
		else if (packet.cmd[XPLICIT_REPL_CLICK] == NETWORK_REPL_CMD_CLICK)
		{

			packet.cmd[XPLICIT_REPL_CLICK] = NETWORK_CMD_INVALID;
		}
		else if (packet.cmd[XPLICIT_REPL_DBL_CLICK] == NETWORK_REPL_CMD_DBL_CLICK)
		{

			packet.cmd[XPLICIT_REPL_DBL_CLICK] = NETWORK_CMD_INVALID;
		}
	}
}