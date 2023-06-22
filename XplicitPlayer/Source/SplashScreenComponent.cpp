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


#include "LocalReplicationComponent.h"
#include "LocalNetworkMonitorEvent.h"
#include "SplashScreenComponent.h"
#include "LocalPlayerComponent.h"
#include "LocalCameraComponent.h"
#include "LocalMenuEvent.h"
#include "Application.h"

#include <CommonEngine.h>

namespace Xplicit::Player
{
	constexpr int XPLICIT_TIMEOUT = ((1 * 60) * 300); // connection timeout

	SplashScreenComponent::SplashScreenComponent() 
		:
		mTexture(nullptr),
		mNetwork(nullptr), 
		mTimeout(XPLICIT_TIMEOUT),
		mEnabled(true)
	{
		mTexture = IRR->getVideoDriver()->getTexture("bkg.png");
	}

	SplashScreenComponent::~SplashScreenComponent() 
	{
		if (mTexture)
			(void)mTexture->drop();
	}

	void SplashScreenComponent::update()
	{
		if (!mNetwork) return;
		if (!mEnabled) return;

		NetworkPacket packet;
		mNetwork->read(packet);

		if (packet.cmd[XPLICIT_NETWORK_CMD_BAN] == NETWORK_CMD_BAN)
		{
			ComponentManager::get_singleton_ptr()->add<Player::PopupComponent>(
				[]() { IRR->closeDevice(); },
				vector2di(XPLICIT_DIM.Width / 2.8,
					XPLICIT_DIM.Height / 2.8),
				POPUP_TYPE::BANNED, "StopPopup");

		}

		if (packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] == NETWORK_CMD_ACCEPT &&
			packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] == NETWORK_CMD_SPAWN)
		{
			auto publicHash = packet.public_hash;
			auto hash = packet.hash;

			packet.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;

			ComponentManager::get_singleton_ptr()->add<LocalReplicationComponent>(hash);
			ComponentManager::get_singleton_ptr()->add<LocalHudComponent>(publicHash);
			
			const auto cam = ComponentManager::get_singleton_ptr()->add<LocalCameraComponent>();
			const auto ply = ComponentManager::get_singleton_ptr()->add<LocalPlayerComponent>(publicHash);

			XPLICIT_ASSERT(ply);

			if (ply)
				ply->attach(cam);

			mNetwork->set_hash(hash);

			EventManager::get_singleton_ptr()->add<LocalNetworkMonitorEvent>(hash);
			EventManager::get_singleton_ptr()->add<LocalPlayerMoveEvent>(publicHash);

			EventManager::get_singleton_ptr()->add<LocalMenuEvent>(hash);

			mEnabled = false;
		}
		else
		{
			--mTimeout;

			// peek after the ++timeout, or retry
			if (mTimeout < 0)
			{
				ComponentManager::get_singleton_ptr()->add<PopupComponent>(
					[]() { IRR->closeDevice(); }, 
						vector2di(XPLICIT_DIM.Width / 2.8,
						XPLICIT_DIM.Height / 2.8),
						POPUP_TYPE::NETWORK, "StopPopup");

				mEnabled = false;
			}
			else
			{
				IRR->getVideoDriver()->draw2DImage(mTexture, 
					vector2di(0, 0),
					recti(0, 0, 1280, 720), 
					nullptr,
					SColor(255, 255, 255, 255), 
					true);
			}
		}
	}

	void SplashScreenComponent::connect(const char* ip)
	{
		mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");

		if (mNetwork == nullptr)
		{
			mNetwork = ComponentManager::get_singleton_ptr()->add<NetworkComponent>();
			XPLICIT_ASSERT(mNetwork);

#ifdef XPLICIT_DEBUG
			if (mNetwork == nullptr)
				throw std::runtime_error("Unrecoverable state! Aborting should be done.");
#endif // ifndef XPLICIT_DEBUG
		}

		if (mNetwork->connect(ip))
		{
			Thread thrd([&]() {
				while (mEnabled)
				{
					XPLICIT_INFO("Trying to connect to peer...");

					NetworkPacket spawn{};

					spawn.cmd[XPLICIT_NETWORK_CMD_BEGIN] = NETWORK_CMD_BEGIN;
					spawn.size = sizeof(NetworkPacket);

					mNetwork->send(spawn);
					
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
			});

			thrd.detach();
		}
	}

	void SplashScreenComponent::reset() noexcept
	{
		ComponentManager::get_singleton_ptr()->add<Player::PopupComponent>([]()-> void {
			IRR->closeDevice();
			}, vector2di(XPLICIT_DIM.Width / 3.45,
				XPLICIT_DIM.Height / 4),
				POPUP_TYPE::NETWORK);
	}
}