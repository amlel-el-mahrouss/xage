/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 *			File: LoadingComponent.cpp
 *			Purpose: XplicitPlayer Loading Screen, show something while the player waits.
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
#include "LocalMenuEvent.h"
#include "Application.h"

#include <XHTTPManager.h>
#include <CommonEngine.h>
#include <lua/CLua.hpp>
#include <codecvt>

namespace Xplicit::Player
{
	// connection timeout, then client quits.
	constexpr int XPLICIT_TIMEOUT = 18000;

	LoadingComponent::LoadingComponent() 
		:
		mNetwork(nullptr), 
		mTimeout(XPLICIT_TIMEOUT),
		mEnabled(true)
	{
		ComponentSystem::get_singleton_ptr()->add<Xplicit::Player::LocalCameraComponent>("Camera");
	}

	LoadingComponent::~LoadingComponent() = default;

	void LoadingComponent::update()
	{
		if (!mNetwork) return;

		NetworkPacket packet;
		mNetwork->read(packet);

		if (packet.cmd[XPLICIT_NETWORK_CMD_BAN] == NETWORK_CMD_BAN)
		{
			ComponentSystem::get_singleton_ptr()->add<Player::PopupComponent>(
				[]() { 
					if (Bites::ObjectInputSystem::get_singleton_ptr()->key_down(VK_RETURN))
						std::exit(0);
				},
				Ogre::Vector2(XPLICIT_DIM.X / 2.8,
					XPLICIT_DIM.Y / 2.8),
				POPUP_TYPE::BANNED, "StopPopup");

			mEnabled = false;
			ComponentSystem::get_singleton_ptr()->remove(mNetwork);

			return;
		}

		if (packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] == NETWORK_CMD_ACCEPT)
		{
			auto public_hash = packet.public_hash;
			auto hash = packet.hash;

			packet.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;

			ComponentSystem::get_singleton_ptr()->add<LocalReplicationComponent>(hash);
			ComponentSystem::get_singleton_ptr()->add<LocalHudComponent>(public_hash);
			
			const auto cam = ComponentSystem::get_singleton_ptr()->get<LocalCameraComponent>("LocalCameraComponent");
			const auto ply = ComponentSystem::get_singleton_ptr()->add<LocalHumanoidComponent>(public_hash);

			ply->attach(cam);
			mNetwork->set_hash(hash);

			const auto monitor = EventSystem::get_singleton_ptr()->add<LocalNetworkMonitorEvent>(hash, public_hash);

			monitor->ID = packet.buffer;
			monitor->Endpoint = XPLICIT_XASSET_ENDPOINT;
			monitor->HTTP = std::make_unique<XHTTPManager>();
			monitor->HTTP->set_endpoint(monitor->Endpoint);

			EventSystem::get_singleton_ptr()->add<LocalHumanoidMoveEvent>(public_hash);
			EventSystem::get_singleton_ptr()->add<LocalMenuEvent>();

			Root::get_singleton_ptr()->set_title("Xplicit [ Place1 ]");

			XPLICIT_INFO("Game:LocalSpawn [EVENT]");
			Lua::CLuaStateManager::get_singleton_ptr()->run_string("Game:LocalSpawn()");

			mEnabled = false;
			return;
		}
		else
		{
			--mTimeout;

			// peek after the ++timeout, or retry
			if (mTimeout < 0)
			{
				ComponentSystem::get_singleton_ptr()->add<PopupComponent>(
					[]() {
						if (Bites::ObjectInputSystem::get_singleton_ptr()->key_down(VK_RETURN))
							std::exit(0);
					}, 
						Ogre::Vector2(XPLICIT_DIM.X / 2.8,
						XPLICIT_DIM.Y / 2.8),
						POPUP_TYPE::NETWORK, "StopPopup");

				ComponentSystem::get_singleton_ptr()->remove(mNetwork);
				mEnabled = false;

				return;
			}
		}
	}

	void LoadingComponent::connect(Utils::UriParser& ip)
	{
		mNetwork = ComponentSystem::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");

		if (mNetwork == nullptr)
		{
			mNetwork = ComponentSystem::get_singleton_ptr()->add<NetworkComponent>();
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

					NetworkPacket spawn{};

					spawn.cmd[XPLICIT_NETWORK_CMD_BEGIN] = NETWORK_CMD_BEGIN;
					spawn.size = sizeof(NetworkPacket);

					mNetwork->send(spawn);

					std::this_thread::sleep_for(std::chrono::milliseconds(500));
				}
			});

			thrd.detach();
		}
	}

	void LoadingComponent::reset() noexcept
	{
		ComponentSystem::get_singleton_ptr()->add<Player::PopupComponent>([]()-> void {
			if (Bites::ObjectInputSystem::get_singleton_ptr()->key_down(VK_RETURN))
				std::exit(0);
			}, Ogre::Vector2(XPLICIT_DIM.X / 3.45,
				XPLICIT_DIM.Y / 4),
				POPUP_TYPE::NETWORK);
	}
}