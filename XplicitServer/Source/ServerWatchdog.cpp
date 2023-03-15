/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: ServerWatchdog.cpp
 *			Purpose: Server Watchdog
 *
 * =====================================================================
 */

#include "ServerWatchdog.h"

namespace Xplicit
{
	ServerWatchdogEvent::ServerWatchdogEvent() 
		: Event(), m_watchdog(false)
	{

	}

	ServerWatchdogEvent::~ServerWatchdogEvent() {}

	void ServerWatchdogEvent::operator()() 
	{
		auto server = InstanceManager::get_singleton_ptr()->get<NetworkServerInstance>("NetworkServerInstance");

		if (server)
		{
			if (this->m_watchdog)
			{
				for (size_t i = 0; i < server->size(); i++)
				{
					if (server->get(i)->stat == NETWORK_STAT_DISCONNECTED)
						continue;

					if (server->get(i)->packet.cmd[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK)
						continue;

					if (server->get(i)->packet.cmd[XPLICIT_NETWORK_CMD_ACK] != NETWORK_CMD_ACK ||
						server->get(i)->packet.hash != server->get(i)->hash)
					{
						server->get(i)->packet.cmd[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_KICK;
						XPLICIT_INFO("[WATCHDOG] " + uuids::to_string(server->get(i)->unique_addr.uuid));
					}
				}

				this->m_watchdog = false;
			}
		}
	}

	void ServerWatchdogEvent::enable(const bool enable) noexcept { this->m_watchdog = enable; }

	const char* ServerWatchdogEvent::name() noexcept { return ("ServerWatchdogEvent"); }
}