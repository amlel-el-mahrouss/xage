/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: LocalMenuEvent.cpp
 *			Purpose: CoreUI menu.
 *
 * =====================================================================
 */

#include "LocalMenuEvent.h"
#include "Application.h"

namespace Xplicit::Client
{
	LocalMenuEvent::LocalMenuEvent(const int64_t& id)
		: m_network(nullptr), m_enabled(false), m_menu(nullptr), m_timeout(0), m_hash(id)
	{
		m_network = InstanceManager::get_singleton_ptr()->get<NetworkInstance>("NetworkInstance");
		assert(m_network);

		XPLICIT_GET_DATA_DIR(data_dir);
		std::string frame_path = data_dir;
		frame_path += "\\Textures\\leave_menu.png";

		m_menu = IRR->getVideoDriver()->getTexture(frame_path.c_str());
	}

	LocalMenuEvent::~LocalMenuEvent() {}

	const char* LocalMenuEvent::name() noexcept { return ("LocalMenuEvent"); }

	void LocalMenuEvent::enable(const bool enable) noexcept
	{
		m_enabled = enable;
	}

	bool LocalMenuEvent::enabled() noexcept { return m_enabled; }

	static const int XPLICIT_TIMEOUT_MENU = 2000;

	void LocalMenuEvent::operator()()
	{
		if (!m_network)
			return;

		static float tween_start = 8;

		if (KB->key_down(KEY_ESCAPE) && m_timeout < 0)
		{
			tween_start = 8;
			m_enabled = !m_enabled;
			m_timeout = XPLICIT_TIMEOUT_MENU;
		}

		if (m_enabled)
		{
			IRR->getVideoDriver()->draw2DImage(m_menu, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / tween_start));

			if (tween_start > 4)
				tween_start -= 0.01f;

			if (KB->key_down(KEY_KEY_L))
			{
				NetworkPacket pckt{};
				pckt.cmd[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_STOP;
				pckt.hash = m_hash;

				m_network->send(pckt);

				m_enabled = false;
				IRR->closeDevice();
			}
			else if (KB->key_down(KEY_KEY_N))
			{
				m_enabled = false;
			}
		}

		--m_timeout;
	}
}