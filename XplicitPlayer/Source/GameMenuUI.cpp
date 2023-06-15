/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: CoreUI.cpp
 *			Purpose: Client UI
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "GameMenuUI.h"

/* Application framework. */
#include "Application.h"

namespace Xplicit::Player
{
	/*
	 * This is a popup class
	 * used to create pop-up according to a state.
	 */

	// constructor
	PopupComponent::PopupComponent(const std::function<void()>& onClick, const vector2di pos, const POPUP_TYPE popupType, const char* id) noexcept
		: m_onClick(onClick), m_vecPos(pos), m_strPopupId(id)
	{
		XPLICIT_ASSERT(onClick);
		XPLICIT_ASSERT(!m_strPopupId.empty());

		String path;

		switch (popupType)
		{
		case POPUP_TYPE::KICK:
			path += "Kicked.png";
			break;
		case POPUP_TYPE::NETWORK_ERROR:
			path += "NetworkError.png";
			break;
		case POPUP_TYPE::SHUTDOWN:
			path += "Shutdown.png";
			break;
		case POPUP_TYPE::WAITING_ACK:
			path += "Download.png";
			break;
		}

		m_pTexture = IRR->getVideoDriver()->getTexture(path.c_str());

		XPLICIT_ASSERT(m_pTexture);

		if (!m_pTexture)
			throw EngineError();
	}

	// c++ destructor
	PopupComponent::~PopupComponent()
	{
		if (m_pTexture)
			m_pTexture->drop();
	}

	// update function
	void PopupComponent::update()
	{
		IRR->getVideoDriver()->draw2DImage(m_pTexture, m_vecPos);

		if (KB->key_down(KEY_ESCAPE))
		{
			m_onClick();
		}
	}

	const char* PopupComponent::name() noexcept
	{
		return m_strPopupId.c_str();
	}

	PopupComponent::INSTANCE_TYPE PopupComponent::type() noexcept
	{
		return PopupComponent::INSTANCE_GUI;
	}
	
	/* Heads up display */
	HUDComponent::HUDComponent()
		: m_health(0), 
		  m_network(nullptr)
	{
		m_network = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");
		XPLICIT_ASSERT(m_network);
	}

	HUDComponent::~HUDComponent() = default;

	void HUDComponent::update()
	{
		if (!m_network)
			return;

		auto packet = m_network->get();
		
		for (size_t network_cmd = XPLICIT_LAST_RESERVED_CMD; network_cmd < XPLICIT_NETWORK_CMD_MAX; ++network_cmd)
		{
			if (packet.cmd[network_cmd] == NETWORK_CMD_DAMAGE)
				m_health = packet.health;
		}
	}
}