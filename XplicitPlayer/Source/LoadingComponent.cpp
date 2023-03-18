/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: LoadingComponent.cpp
 *			Purpose:
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "LocalWatchdogEvent.h"
#include "LoadingComponent.h"
#include "LocalResetEvent.h"
#include "LocalMenuEvent.h"
#include "Application.h"
#include "LocalActor.h"
#include "Camera.h"
#include "CoreUI.h"

namespace Xplicit::Client
{
	constexpr const int XPLICIT_TIMEOUT = (1 * 60 * 60) * 60; // connection timeout

	LoadingComponent::LoadingComponent() 
		: m_run(true), m_network(nullptr), m_texture(nullptr), m_timeout(XPLICIT_TIMEOUT)
	{
		m_texture = IRR->getVideoDriver()->getTexture("logo.png");
	}

	LoadingComponent::~LoadingComponent() 
	{

	}

	void LoadingComponent::update()
	{
		if (!m_network)
			return;

		UDPNetworkPacket packet{};
		m_network->read(packet);

		if (packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] == NETWORK_CMD_ACCEPT)
		{
			ComponentManager::get_singleton_ptr()->add<Xplicit::CoreUI::HUD>();
			auto actor = ComponentManager::get_singleton_ptr()->add<Xplicit::Client::LocalActor>(packet.public_hash);
			XPLICIT_ASSERT(actor);

			if (actor)
			{
				actor->attach(ComponentManager::get_singleton_ptr()->add<Xplicit::Client::CameraComponent>());

				EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalResetEvent>();
				EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalMenuEvent>(packet.hash);
				EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalWatchdogEvent>(packet.hash);
				EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalMoveEvent>(packet.public_hash);

				m_run = false;
			}
			else
			{
				packet.cmd[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_STOP;
				packet.size = sizeof(UDPNetworkPacket);
				packet.hash = packet.hash;

				m_network->send(packet);
				
				ComponentManager::get_singleton_ptr()->add<CoreUI::Popup>([]()-> void {
					IRR->closeDevice();
				}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), CoreUI::POPUP_TYPE::NetworkError);


				m_run = false;
			}
		}
		else
		{
			--m_timeout;

			// peek after the ++timeout, or retry
			if (m_timeout < 0)
			{
				ComponentManager::get_singleton_ptr()->add<CoreUI::Popup>([]()-> void {
					IRR->closeDevice();
					}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), CoreUI::POPUP_TYPE::NetworkError);

				m_run = false; // sprious reponse

			}
			else
			{
				packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] = NETWORK_CMD_BEGIN;
				packet.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;
				packet.size = sizeof(UDPNetworkPacket);

				m_network->send(packet);

				IRR->getVideoDriver()->draw2DImage(m_texture, vector2di(Xplicit::Client::XPLICIT_DIM.Width * 0.02, Xplicit::Client::XPLICIT_DIM.Height * 0.825),
					core::rect<s32>(0, 0, 105, 105), 0,
					video::SColor(255, 255, 255, 255), true);
			}

		}

	}

	void LoadingComponent::connect(const char* ip)
	{
		m_network = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");

		if (!m_network)
		{
			m_network = ComponentManager::get_singleton_ptr()->add<NetworkComponent>();
			assert(m_network);
		}

		if (m_network->connect(ip))
		{
			UDPNetworkPacket spawn{};

			spawn.cmd[XPLICIT_NETWORK_CMD_BEGIN] = NETWORK_CMD_BEGIN;
			spawn.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;
			spawn.size = sizeof(UDPNetworkPacket);
		
			m_network->send(spawn);
		}
	}

	void LoadingComponent::reset() noexcept
	{
		ComponentManager::get_singleton_ptr()->add<CoreUI::Popup>([]()-> void {
			IRR->closeDevice();
			}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), CoreUI::POPUP_TYPE::NetworkError);

	}
}