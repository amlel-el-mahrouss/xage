/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: CoreUI.cpp
 *			Purpose: Core Client UI
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
	Popup::Popup(const std::function<void()>& on_click, const vector2di pos, const POPUP_TYPE shutdown_type) noexcept
		: m_on_click(on_click), m_pos(pos)
	{
		XPLICIT_ASSERT(on_click);

		std::string path;

		switch (shutdown_type)
		{
		case POPUP_TYPE::Kicked:
			path += "network_kicked.png";
			break;
		case POPUP_TYPE::NetworkError:
			path += "network_error.png";
			break;
		case POPUP_TYPE::Shutdown:
			path += "network_shutdown.png";

			break;
		}

		m_texture = IRR->getVideoDriver()->getTexture(path.c_str());
		XPLICIT_ASSERT(m_texture);

		if (!m_texture)
			throw EngineError();
	}

	Popup::~Popup()
	{
		if (m_texture)
			m_texture->drop();
	}

	void Popup::update()
	{
		IRR->getVideoDriver()->draw2DImage(m_texture, m_pos);

		if (KB->key_down(KEY_ESCAPE))
		{
			m_on_click();
		}
	}


	const char* Popup::name() noexcept
	{
		return "Popup";
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

		// TODO: draw health
		IRR->getVideoDriver()->draw2DImage(m_texture, vector2di(30, 30), recti(0, 0, 326, 28), nullptr, SColor(255, 255, 255, 255), true);
	}

}