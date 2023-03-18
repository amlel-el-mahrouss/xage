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

 /**
 @file
 */

#include "ServerWatchdog.h"

namespace Xplicit
{
	ServerWatchdogEvent::ServerWatchdogEvent() 
		: Event(), m_watchdog_timer(1000), m_server(ComponentManager::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent"))
	{
		XPLICIT_ASSERT(m_server);
	}

	ServerWatchdogEvent::~ServerWatchdogEvent() {}

	void ServerWatchdogEvent::operator()() 
	{
		if (m_server)
		{
			if (m_watchdog_timer <= 0)
			{
				for (size_t i = 0; i < m_server->size(); i++)
				{
					if (m_server->get(i)->stat == NETWORK_STAT_DISCONNECTED)
						continue;

					if (m_server->get(i)->packet.cmd[XPLICIT_NETWORK_CMD_ACK] != NETWORK_CMD_ACK)
					{
						m_server->get(i)->packet.cmd[XPLICIT_NETWORK_CMD_KICK] = NETWORK_CMD_KICK;

						XPLICIT_INFO("[WATCHDOG] Flagged " + uuids::to_string(m_server->get(i)->unique_addr.get()));
					}
				}

				m_watchdog_timer = 1000;
			}
			else
			{
				for (size_t i = 0; i < m_server->size(); i++)
				{
					if (m_server->get(i)->stat == NETWORK_STAT_DISCONNECTED)
						continue;

					m_server->get(i)->packet.cmd[XPLICIT_NETWORK_CMD_WATCHDOG] = NETWORK_CMD_WATCHDOG;
				}

				--m_watchdog_timer;
			}
		}
	}

	const char* ServerWatchdogEvent::name() noexcept { return ("ServerWatchdogEvent"); }
}