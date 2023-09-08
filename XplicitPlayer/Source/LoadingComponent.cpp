/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright PlayXPlicit, all rights reserved.
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
#include "MenuUI.h"
#include "Application.h"

#include <XHTTPManager.h>
#include <CommonEngine.h>
#include <CLua/CLua.hpp>
#include <RoXML.h>
#include <codecvt>

namespace Xplicit::Player
{
	// connection timeout, then client quits.
	constexpr int XPLICIT_TIMEOUT = 18000;

	LoadingComponent::LoadingComponent() 
		:
		mNetwork(nullptr), 
		mTimeout(XPLICIT_TIMEOUT),
		mLoadingFrame()
	{
		ComponentSystem::get_singleton_ptr()->add<Xplicit::Player::LocalCameraComponent>();
		
		mLoadingFrame.X = 0;
		mLoadingFrame.Y = 0;

		mLoadingFrame.W = RENDER->getVideoDriver()->getScreenSize().Width;
		mLoadingFrame.H = RENDER->getVideoDriver()->getScreenSize().Height;
	}

	LoadingComponent::~LoadingComponent() = default;

	bool LoadingComponent::mEnabled = true;

	void LoadingComponent::update(void* class_ptr)
	{
		LoadingComponent* self = (LoadingComponent*)class_ptr;

		if (!self->mNetwork) return;

		NetworkPacket packet;
		self->mNetwork->read(packet);

		if (packet.cmd[XPLICIT_NETWORK_CMD_BAN] == NETWORK_CMD_BAN)
		{
			ComponentSystem::get_singleton_ptr()->add<Player::PopupComponent>(
				[]() { 
					RENDER->closeDevice();
				}, POPUP_TYPE::BANNED, "StopPopup");

			mEnabled = false;

			ComponentSystem::get_singleton_ptr()->remove(self->mNetwork);

			return;
		}

		if (packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] == NETWORK_CMD_ACCEPT)
		{
			auto public_hash = packet.public_hash;
			auto hash = packet.hash;

			packet.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;

			ComponentSystem::get_singleton_ptr()->add<LocalReplicationComponent>(hash);
			ComponentSystem::get_singleton_ptr()->add<HUDComponent>(public_hash);
			
			const auto cam = ComponentSystem::get_singleton_ptr()->get<LocalCameraComponent>("LocalCameraComponent");
			const auto local_player = ComponentSystem::get_singleton_ptr()->add<LocalHumanoidComponent>(public_hash, true);

			const auto monitor = EventSystem::get_singleton_ptr()->add<LocalNetworkMonitorEvent>(hash, public_hash);

			local_player->attach(cam);
			self->mNetwork->set_hash(hash);

			monitor->ID = packet.buffer;

			local_player->get_class()->insert("Id", (String("'") + monitor->ID + "'").c_str());

			XPLICIT_INFO(monitor->ID);

			monitor->Endpoint = XPLICIT_XASSET_ENDPOINT;
			monitor->HTTP = std::make_unique<XHTTPManager>();
			monitor->HTTP->set_endpoint(monitor->Endpoint);

			EventSystem::get_singleton_ptr()->add<LocalHumanoidMoveEvent>(public_hash);
			EventSystem::get_singleton_ptr()->add<LocalMenuEvent>();

			mEnabled = false;

			return;
		}
		else
		{
			--self->mTimeout;

			// peek after the ++timeout, or retry
			if (self->mTimeout < 0)
			{
				ComponentSystem::get_singleton_ptr()->add<PopupComponent>(
					[]() {
						RENDER->closeDevice();
					}, POPUP_TYPE::NETWORK, "StopPopup");

				ComponentSystem::get_singleton_ptr()->remove(self->mNetwork);
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
			RENDER->closeDevice();
			}, POPUP_TYPE::NETWORK);
	}
}