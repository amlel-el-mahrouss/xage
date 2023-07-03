/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 *			File: LoadingComponent.cpp
 *			Purpose: 
 *
 * =====================================================================
 */

 /**
 @file
 */


#include "LocalReplicationComponent.h"
#include "LocalNetworkMonitorEvent.h"
#include "LocalHumanoidComponent.h"
#include "LocalCameraComponent.h"
#include "LoadingComponent.h"
#include "LocalHTTPManager.h"
#include "LocalMenuEvent.h"
#include "Application.h"

#include <CommonEngine.h>
#include <lua/lua.hpp>

extern ILightSceneNode* XPLICIT_LIGHT;

namespace Xplicit::Player
{
	// connection timeout, then client quits.
	constexpr int XPLICIT_TIMEOUT = 28000;
	static Gwk::Controls::Canvas* XPLICIT_CANVAS;

	LoadingComponent::LoadingComponent() 
		:
		mTexture(nullptr),
		mNetwork(nullptr), 
		mTimeout(XPLICIT_TIMEOUT),
		mEnabled(true)
	{
		mTexture = RENDER->getVideoDriver()->getTexture("bkg.png");
	}

	LoadingComponent::~LoadingComponent() 
	{
		if (mTexture)
			(void)mTexture->drop();
	}

	void LoadingComponent::update()
	{
		if (!mNetwork) return;

		NetworkPacket packet;
		mNetwork->read(packet);

		if (packet.cmd[XPLICIT_NETWORK_CMD_BAN] == NETWORK_CMD_BAN)
		{
			ComponentManager::get_singleton_ptr()->add<Player::PopupComponent>(
				[]() { 
					if (KB->key_down(KEY_RETURN))
						RENDER->closeDevice();
				},
				vector2di(XPLICIT_DIM.X / 2.8,
					XPLICIT_DIM.Y / 2.8),
				POPUP_TYPE::BANNED, "StopPopup");

			mEnabled = false;

			ComponentManager::get_singleton_ptr()->remove(mNetwork);

			return;
		}

		if (packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] == NETWORK_CMD_ACCEPT)
		{
			auto public_hash = packet.public_hash;
			auto hash = packet.hash;

			packet.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;

			ComponentManager::get_singleton_ptr()->add<LocalReplicationComponent>(hash);
			ComponentManager::get_singleton_ptr()->add<LocalHudComponent>(public_hash);
			
			const auto cam = ComponentManager::get_singleton_ptr()->add<LocalCameraComponent>();
			
			const auto ply = ComponentManager::get_singleton_ptr()->add<LocalHumanoidComponent>(public_hash);

			ply->attach(cam);
			mNetwork->set_hash(hash);

			const auto monitor = EventManager::get_singleton_ptr()->add<LocalNetworkMonitorEvent>(hash, public_hash);

			monitor->ID = packet.buffer;
			monitor->Endpoint = XPLICIT_XASSET_ENDPOINT;
			monitor->HTTP = std::make_unique<LocalHTTPManager>();

			EventManager::get_singleton_ptr()->add<LocalHumanoidMoveEvent>(public_hash);
			EventManager::get_singleton_ptr()->add<LocalMenuEvent>(XPLICIT_CANVAS);

			XPLICIT_LIGHT = RENDER->getSceneManager()->addLightSceneNode(nullptr, vector3df(0, 0, 0), SColorf(1.0, 1.0, 1.0, 1.0), 1000.0);

			RENDER->setWindowCaption(L"Xplicit [ InGame ]");

			XPLICIT_INFO("Engine:LocalSpawn [EVENT]");
			Lua::XLuaStateManager::get_singleton_ptr()->run_string("Engine:LocalSpawn()");

			mEnabled = false;
			return;
		}
		else
		{
			--mTimeout;

			// peek after the ++timeout, or retry
			if (mTimeout < 0)
			{
				ComponentManager::get_singleton_ptr()->add<PopupComponent>(
					[]() {
						if (KB->key_down(KEY_RETURN))
							RENDER->closeDevice(); 
					}, 
						vector2di(XPLICIT_DIM.X / 2.8,
						XPLICIT_DIM.Y / 2.8),
						POPUP_TYPE::NETWORK, "StopPopup");

				ComponentManager::get_singleton_ptr()->remove(mNetwork);
				mEnabled = false;

				return;
			}
			else
			{
				RENDER->getVideoDriver()->draw2DImage(mTexture, 
					vector2di(0, 0),
					recti(0, 0, 1280, 720), 
					nullptr,
					SColor(255, 255, 255, 255), 
					true);
			}
		}
	}

	void LoadingComponent::set_data(Gwk::Controls::Canvas* pCanvas) noexcept
	{
		XPLICIT_CANVAS = pCanvas;
	}

	void LoadingComponent::connect(Utils::UriParser& ip)
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

		if (mNetwork->connect(ip.get().c_str(), ip.port().c_str()))
		{
			Thread thrd([&]() {
				while (mEnabled)
				{
					if (!mNetwork)
						return;

					XPLICIT_INFO("[XCONNECT] Connecting...");

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

	void LoadingComponent::reset() noexcept
	{
		ComponentManager::get_singleton_ptr()->add<Player::PopupComponent>([]()-> void {
			if (KB->key_down(KEY_RETURN))
				RENDER->closeDevice();
			}, vector2di(XPLICIT_DIM.X / 3.45,
				XPLICIT_DIM.Y / 4),
				POPUP_TYPE::NETWORK);
	}
}