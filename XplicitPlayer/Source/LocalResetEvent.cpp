/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: LocalResetEvent.cpp
 *			Purpose: RST timeout event.
 *
 * =====================================================================
 */

#include "LocalResetEvent.h"
#include "Application.h"
#include "CoreUI.h"

namespace Xplicit::Client
{
	constexpr const int XPLICIT_MAX_RESETS = 1000; // max RST timeout

	LocalResetEvent::LocalResetEvent()
		: m_network(nullptr), m_num_resets(0)
	{
		m_network = InstanceManager::get_singleton_ptr()->get<NetworkInstance>("NetworkInstance");
		assert(m_network);
	}

	LocalResetEvent::~LocalResetEvent() {}

	const char* LocalResetEvent::name() noexcept { return ("LocalResetEvent"); }

	void LocalResetEvent::operator()()
	{
		if (!m_network)
			return;

		auto& packet = m_network->get();

		if (packet.cmd[XPLICIT_NETWORK_CMD_STOP] == NETWORK_CMD_STOP)
		{
			if (!InstanceManager::get_singleton_ptr()->get<CoreUI::ErrorMessage>("ErrorMessage"))
			{
				InstanceManager::get_singleton_ptr()->add<CoreUI::ErrorMessage>([]()-> void {
					IRR->closeDevice();
					}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), CoreUI::ERROR_TYPE::Shutdown);

				return;
			}
		}

		if (m_network->is_reset())
		{
			++m_num_resets;

			if (m_num_resets > XPLICIT_MAX_RESETS)
			{
				if (!InstanceManager::get_singleton_ptr()->get<CoreUI::ErrorMessage>("ErrorMessage"))
				{
					InstanceManager::get_singleton_ptr()->add<CoreUI::ErrorMessage>([]()-> void {
						IRR->closeDevice();
						}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), CoreUI::ERROR_TYPE::NetworkError);

				}
			}
		}
	}

}