/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: LocalWatchdogEvent.cpp
 *			Purpose: Watchdog client.
 *
 * =====================================================================
 */

#include "LocalWatchdogEvent.h"
#include "Application.h"
#include "CoreUI.h"

namespace Xplicit::Client
{
	LocalWatchdogEvent::LocalWatchdogEvent(const int64_t& id)
		: m_network(nullptr), m_id(id)
	{
		m_network = InstanceManager::get_singleton_ptr()->get<NetworkInstance>("NetworkInstance");
		XPLICIT_ASSERT(m_network);
	}

	LocalWatchdogEvent::~LocalWatchdogEvent()
	{

	}

	void LocalWatchdogEvent::operator()()
	{
		if (m_network)
		{
			auto& packet = m_network->get();

			if (packet.cmd[XPLICIT_NETWORK_CMD_WATCHDOG] == NETWORK_CMD_WATCHDOG)
			{
				packet.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;
				packet.hash = m_id;

				m_network->send(packet);
			}

			if (packet.cmd[XPLICIT_NETWORK_CMD_KICK] == NETWORK_CMD_KICK)
			{
				if (!InstanceManager::get_singleton_ptr()->get<CoreUI::ErrorMessage>("ErrorMessage"))
				{
					InstanceManager::get_singleton_ptr()->add<CoreUI::ErrorMessage>([]()-> void {
						IRR->closeDevice();
						}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), CoreUI::ERROR_TYPE::Kicked);

				}
			}
		}
	}
}