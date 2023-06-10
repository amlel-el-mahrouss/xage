/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: LocalMenuEvent.cpp
 *			Purpose: Client menu
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "LocalMenuEvent.h"
#include "Application.h"

#define LOCAL_MENU_TWEEN_START (8)
#define LOCAL_MENU_TWEEN_END (2)
#define LOCAL_MENU_TWEENING (0.1f)

namespace Xplicit::Client
{
	LocalMenuEvent::LocalMenuEvent(const int64_t& hash)
		: m_network(nullptr), m_enabled(false), m_menu(nullptr), m_timeout(0), m_hash(hash)
	{
		m_network = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");
		XPLICIT_ASSERT(m_network);

		XPLICIT_GET_DATA_DIR(data_dir);
		std::string frame_path = data_dir;
		frame_path += "\\Textures\\Leave.png";

		m_menu = IRR->getVideoDriver()->getTexture(frame_path.c_str());
	}

	LocalMenuEvent::~LocalMenuEvent() 
	{
		if (m_menu)
			m_menu->drop();
	}

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

		static float tween_start = LOCAL_MENU_TWEEN_START;

		if (KB->key_down(KEY_ESCAPE) && m_timeout < 0)
		{
			tween_start = LOCAL_MENU_TWEEN_START;
			m_enabled = !m_enabled;
			m_timeout = XPLICIT_TIMEOUT_MENU;
		}

		if (m_enabled)
		{
			if (tween_start > LOCAL_MENU_TWEEN_END)
				tween_start -= LOCAL_MENU_TWEENING;

			if (KB->key_down(KEY_KEY_Y))
			{
				NetworkPacket stop_packet{};

				stop_packet.cmd[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_STOP;
				stop_packet.hash = m_hash;
				stop_packet.size = sizeof(NetworkPacket);

				m_network->send(stop_packet);
				m_enabled = false;

				XPLICIT_SLEEP(200);
				IRR->closeDevice();
			}
			else if (KB->key_down(KEY_KEY_N))
			{
				m_enabled = false;
			}
		}
		else
		{
			if (tween_start < LOCAL_MENU_TWEEN_START)
				tween_start += LOCAL_MENU_TWEENING;
		}

		if (tween_start < LOCAL_MENU_TWEEN_START)
		{
			IRR->getVideoDriver()->draw2DImage(m_menu, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 2.8,
				Xplicit::Client::XPLICIT_DIM.Height / tween_start));
		}

		--m_timeout;
	}
}