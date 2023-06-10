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

 /**
 @file
 */

#include "LocalResetEvent.h"
#include "Application.h"
#include "CoreUI.h"

namespace Xplicit::Client
{
	constexpr const int XPLICIT_MAX_RESETS = 10000; // Max RST

	LocalResetEvent::LocalResetEvent(int64_t hash)
		: m_network(nullptr), m_resets(0), m_hash(hash)
	{
		m_network = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");
		assert(m_network);
	}

	LocalResetEvent::~LocalResetEvent() {}

	const char* LocalResetEvent::name() noexcept { return ("LocalResetEvent"); }

	void LocalResetEvent::operator()()
	{
		if (!m_network)
			return;

		auto& packet = m_network->get();

		if (packet.cmd[XPLICIT_NETWORK_CMD_STOP] == NETWORK_CMD_STOP &&
			m_hash == packet.hash)
		{
			if (!ComponentManager::get_singleton_ptr()->get<CoreUI::Popup>("StopPopup"))
			{
				ComponentManager::get_singleton_ptr()->add<CoreUI::Popup>([]()-> void {
					IRR->closeDevice();
					}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 2.8, Xplicit::Client::XPLICIT_DIM.Height / 4), CoreUI::POPUP_TYPE::SHUTDOWN, "ResetConnPopup");

				return;
			}
		}

		if (m_network->is_reset())
		{
			++m_resets;

			if (m_resets > XPLICIT_MAX_RESETS)
			{
				if (!ComponentManager::get_singleton_ptr()->get<CoreUI::Popup>("ResetPopup"))
				{
					ComponentManager::get_singleton_ptr()->add<CoreUI::Popup>([]()-> void {
						IRR->closeDevice();
						}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), CoreUI::POPUP_TYPE::NETWORK_ERROR, "ResetPopup");

				}
			}
		}
	}

}