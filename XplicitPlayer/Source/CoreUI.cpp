/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: CoreUI.cpp
 *			Purpose: Client UI
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "Application.h"
#include "CoreUI.h"

namespace Xplicit::CoreUI
{
	Popup::Popup(const std::function<void()>& onClick, const vector2di pos, const POPUP_TYPE popupType, const char* id) noexcept
		: m_onClick(onClick), m_vecPos(pos), m_strPopupId(id)
	{
		XPLICIT_ASSERT(onClick);
		XPLICIT_ASSERT(!m_strPopupId.empty());

		String path;

		switch (popupType)
		{
		case POPUP_TYPE::KICK:
			path += "network_kicked.png";
			break;
		case POPUP_TYPE::NETWORK_ERROR:
			path += "network_error.png";
			break;
		case POPUP_TYPE::SHUTDOWN:
			path += "network_shutdown.png";

			break;
		}

		m_pTexture = IRR->getVideoDriver()->getTexture(path.c_str());

		XPLICIT_ASSERT(m_pTexture);

		if (!m_pTexture)
			throw EngineError();
	}

	Popup::~Popup()
	{
		if (m_pTexture)
			m_pTexture->drop();
	}

	void Popup::update()
	{
		IRR->getVideoDriver()->draw2DImage(m_pTexture, m_vecPos);

		if (KB->key_down(KEY_ESCAPE))
		{
			m_onClick();
		}
	}


	const char* Popup::name() noexcept
	{
		return m_strPopupId.c_str();
	}

	Popup::INSTANCE_TYPE Popup::type() noexcept
	{
		return Popup::INSTANCE_GUI;
	}
	
	HUD::HUD()
		: m_health(50), m_network(nullptr), m_texture(nullptr)
	{
		m_network = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");
		XPLICIT_ASSERT(m_network);

		m_texture = IRR->getVideoDriver()->getTexture("health.png");
		XPLICIT_ASSERT(m_texture);
	}

	HUD::~HUD()
	{
		if (m_texture)
			m_texture->drop();
	}

	void HUD::update()
	{
		if (!m_network ||
			!m_texture)
			return;

		auto packet = m_network->get();
		
		for (size_t network_cmd = XPLICIT_LAST_RESERVED_CMD; network_cmd < XPLICIT_NETWORK_CMD_MAX; ++network_cmd)
		{
			if (packet.cmd[network_cmd] == NETWORK_CMD_DAMAGE)
				m_health = packet.health;
		}

		IRR->getVideoDriver()->draw2DImage(m_texture, vector2di(30, 30), recti(0, 0, 326, 28), nullptr, SColor(255, 255, 255, 255), true);
	}

}