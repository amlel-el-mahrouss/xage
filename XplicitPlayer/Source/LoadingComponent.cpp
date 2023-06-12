/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: LoadingComponent.cpp
 *			Purpose: Loading Component
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "LoadingComponent.h"

#include "LocalPlayerComponent.h"
#include "LocalResetEvent.h"
#include "CameraComponent.h"
#include "LocalMenuEvent.h"
#include "Application.h"

namespace Xplicit::Client
{
	constexpr const int XPLICIT_TIMEOUT = ((1 * 60) * 60 * 60); // connection timeout

	LoadingComponent::LoadingComponent() 
		: mEnable(true), mNetwork(nullptr), m_texture(nullptr), m_timeout(XPLICIT_TIMEOUT)
	{
		m_texture = IRR->getVideoDriver()->getTexture("xpx.png");
		ComponentManager::get_singleton_ptr()->add<CoreUI::Notification>("Downloading", "Download.png");
	}

	LoadingComponent::~LoadingComponent() 
	{
		if (m_texture)
			m_texture->drop();
	}

	void LoadingComponent::update()
	{
		if (!mNetwork)
			return;

		NetworkPacket packet{};
		mNetwork->read(packet);

		/* command accepted, let's download files... */
		if (packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] == NETWORK_CMD_ACCEPT)
		{
			auto ply = ComponentManager::get_singleton_ptr()->add<Xplicit::Client::LocalPlayerComponent>(packet.public_hash);
			XPLICIT_ASSERT(ply);

			if (ply)
			{
				ply->attach(ComponentManager::get_singleton_ptr()->add<Xplicit::Client::CameraComponent>());

				ComponentManager::get_singleton_ptr()->add<Xplicit::CoreUI::HUD>();

				EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalResetEvent>(packet.hash);
				EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalMenuEvent>(packet.hash);
				EventDispatcher::get_singleton_ptr()->add<Xplicit::Client::LocalMoveEvent>(packet.public_hash);

				mEnable = false;
			}
			else
			{
				packet.cmd[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_STOP;
				packet.size = sizeof(NetworkPacket);

				mNetwork->send(packet);
				
				ComponentManager::get_singleton_ptr()->add<CoreUI::Popup>([]()-> void {
					IRR->closeDevice();
				}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45,
					Xplicit::Client::XPLICIT_DIM.Height / 4),
					CoreUI::POPUP_TYPE::NETWORK_ERROR);

				mEnable = false;
			}
		}
		else /* download files ... */
		{
			--m_timeout;

			// peek after the ++timeout, or retry
			if (m_timeout < 0)
			{
				packet.cmd[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_STOP;
				packet.size = sizeof(NetworkPacket);

				mNetwork->send(packet);

				ComponentManager::get_singleton_ptr()->add<CoreUI::Popup>([]()-> void {
					IRR->closeDevice();
					}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, Xplicit::Client::XPLICIT_DIM.Height / 4), CoreUI::POPUP_TYPE::NETWORK_ERROR, "TimeoutPopup");

				mEnable = false;
			}
			else
			{
				packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] = NETWORK_CMD_BEGIN;
				packet.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;

				packet.size = sizeof(NetworkPacket);

				mNetwork->send(packet);

				IRR->getVideoDriver()->draw2DImage(m_texture, 
					vector2di(Xplicit::Client::XPLICIT_DIM.Width * 0.02, Xplicit::Client::XPLICIT_DIM.Height * 0.625),
					core::rect<s32>(0, 0, 255, 255), 0,
					video::SColor(255, 255, 255, 255), true);
			}

		}

	}

	void LoadingComponent::connect(const char* ip)
	{
		mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");

		if (!mNetwork)
		{
			mNetwork = ComponentManager::get_singleton_ptr()->add<NetworkComponent>();
			assert(mNetwork);
		}

		if (mNetwork->connect(ip))
		{
			NetworkPacket spawn{};

			spawn.cmd[XPLICIT_NETWORK_CMD_BEGIN] = NETWORK_CMD_BEGIN;
			spawn.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;

			spawn.size = sizeof(NetworkPacket);
		
			mNetwork->send(spawn);
		}
	}

	void LoadingComponent::reset() noexcept
	{
		ComponentManager::get_singleton_ptr()->add<CoreUI::Popup>([]()-> void {
			IRR->closeDevice();
			}, vector2di(Xplicit::Client::XPLICIT_DIM.Width / 3.45, 
				Xplicit::Client::XPLICIT_DIM.Height / 4), 
				CoreUI::POPUP_TYPE::NETWORK_ERROR);

	}
}