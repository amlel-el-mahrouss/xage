/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: LoadingComponent.cpp
 *			Purpose: Loading Component
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "SplashScreenComponent.h"

#include "LocalNetworkMonitorEvent.h"
#include "LocalPlayerComponent.h"
#include "CameraComponent.h"
#include "LocalMenuEvent.h"
#include "Application.h"

namespace Xplicit::Player
{
	constexpr const int XPLICIT_TIMEOUT = ((1 * 60) * 10); // connection timeout

	SplashScreenComponent::SplashScreenComponent() 
		: mEnable(true), mNetwork(nullptr), mTexture(nullptr), mTimeout(XPLICIT_TIMEOUT)
	{
		mTexture = IRR->getVideoDriver()->getTexture("NgineLogo.png");
	}

	SplashScreenComponent::~SplashScreenComponent() 
	{
		if (mTexture)
			mTexture->drop();
	}

	void SplashScreenComponent::update()
	{
		if (!mNetwork)
			return;

		NetworkPacket packet{};
		mNetwork->read(packet);

		/* command accepted, let's download files... */
		if (packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] == NETWORK_CMD_ACCEPT)
		{
			ComponentManager::get_singleton_ptr()->add<Xplicit::Player::HUDComponent>();
			ComponentManager::get_singleton_ptr()->add<Xplicit::Player::CameraComponent>();

			ComponentManager::get_singleton_ptr()->add<Xplicit::Player::LocalPlayerComponent>(packet.public_hash);

			EventManager::get_singleton_ptr()->add<Xplicit::Player::LocalNetworkMonitorEvent>(packet.hash);
			EventManager::get_singleton_ptr()->add<Xplicit::Player::LocalMenuEvent>(packet.hash);

			mEnable = false;
		}
		else
		{
			--mTimeout;

			// peek after the ++timeout, or retry
			if (mTimeout < 0)
			{
				packet.cmd[XPLICIT_NETWORK_CMD_STOP] = NETWORK_CMD_STOP;
				packet.size = sizeof(NetworkPacket);

				mNetwork->send(packet);

				ComponentManager::get_singleton_ptr()->add<Player::PopupComponent>(
					[]() { IRR->closeDevice(); }, 
						vector2di(Xplicit::Player::XPLICIT_DIM.Width / 2.8,
						Xplicit::Player::XPLICIT_DIM.Height / 2.8),
						Player::POPUP_TYPE::NETWORK_ERROR, "StopPopup");

				mEnable = false;
			}
			else
			{
				packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] = NETWORK_CMD_BEGIN;
				packet.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;

				packet.size = sizeof(NetworkPacket);

				mNetwork->send(packet);

				IRR->getVideoDriver()->draw2DImage(mTexture, 
					vector2di(Xplicit::Player::XPLICIT_DIM.Width * 0.02, Xplicit::Player::XPLICIT_DIM.Height * 0.625),
					core::rect<s32>(0, 0, 255, 255), 0,
					video::SColor(255, 255, 255, 255), true);
			}

		}

	}

	void SplashScreenComponent::connect(const char* ip)
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

	void SplashScreenComponent::reset() noexcept
	{
		ComponentManager::get_singleton_ptr()->add<Player::PopupComponent>([]()-> void {
			IRR->closeDevice();
			}, vector2di(Xplicit::Player::XPLICIT_DIM.Width / 3.45,
				Xplicit::Player::XPLICIT_DIM.Height / 4),
				Player::POPUP_TYPE::NETWORK_ERROR);
	}
}