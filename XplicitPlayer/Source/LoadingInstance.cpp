/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Common.cpp
 *			Purpose: Common components
 *
 * =====================================================================
 */

#include "LocalWatchdogEvent.h"
#include "LoadingInstance.h"
#include "LocalResetEvent.h"
#include "LocalMenuEvent.h"
#include "Application.h"
#include "LocalActor.h"
#include "Camera.h"
#include "CoreUI.h"

namespace Xplicit::Client
{
	constexpr const int XPLICIT_TIMEOUT = 1000; // connection timeout

	LoadingInstance::LoadingInstance() 
		: m_run(true), m_network(nullptr), m_logo_tex(nullptr), m_timeout(XPLICIT_TIMEOUT)
	{
		XPLICIT_GET_DATA_DIR(data_dir);

		std::string health_path = data_dir;
		health_path += "\\Textures\\logo.png";

		m_logo_tex = IRR->getVideoDriver()->getTexture(health_path.c_str());
	}

	LoadingInstance::~LoadingInstance() {}

	void LoadingInstance::update()
	{
		if (!m_run)
			return;
		
		NetworkPacket packet{};
		m_network->read(packet);

		if (packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] == NETWORK_CMD_ACCEPT)
		{
			InstanceManager::get_singleton_ptr()->add<Xplicit::CoreUI::HUD>();
			auto actor = InstanceManager::get_singleton_ptr()->add<Xplicit::Client::LocalActor>(packet.hash);
			XPLICIT_ASSERT(actor);

			actor->attach(InstanceManager::get_singleton_ptr()->add<Xplicit::Client::CameraInstance>());

			EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalMenuEvent>(packet.hash);
			EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalMoveEvent>(packet.hash);
			EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalResetEvent>();
			EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalWatchdogEvent>(packet.hash);

			m_run = false;

			return;
		}

		IRR->getVideoDriver()->draw2DImage(m_logo_tex, vector2di(Xplicit::Client::XPLICIT_DIM.Width * 0.02, Xplicit::Client::XPLICIT_DIM.Height * 0.825),
			core::rect<s32>(0, 0, 105, 105), 0,
			video::SColor(255, 255, 255, 255), true);

		--m_timeout;

		// peek after the ++timeout
		if (m_timeout < 0)
		{
			InstanceManager::get_singleton_ptr()->add<CoreUI::ErrorMessage>([]()-> void {
				IRR->closeDevice();
			}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), CoreUI::ERROR_TYPE::NetworkError);

			m_run = false; // sprious reponse
		}

		// retry...
		packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] = NETWORK_CMD_BEGIN;
		packet.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;

		m_network->send(packet);

		XPLICIT_SLEEP(100);
	}

	void LoadingInstance::connect(const char* ip)
	{
		m_network = InstanceManager::get_singleton_ptr()->get<NetworkInstance>("NetworkInstance");

		if (!m_network)
		{
			m_network = InstanceManager::get_singleton_ptr()->add<NetworkInstance>();
			assert(m_network);
		}

		if (m_network->connect(ip))
		{
			NetworkPacket spawn{};

			spawn.cmd[XPLICIT_NETWORK_CMD_BEGIN] = NETWORK_CMD_BEGIN;
			spawn.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;
		
			m_network->send(spawn);
		}
	}

	void LoadingInstance::reset() noexcept
	{
		InstanceManager::get_singleton_ptr()->add<CoreUI::ErrorMessage>([]()-> void {
			IRR->closeDevice();
			}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), CoreUI::ERROR_TYPE::NetworkError);

	}
}