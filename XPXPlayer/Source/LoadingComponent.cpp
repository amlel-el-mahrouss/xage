/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: LoadingComponent.cpp
 *			Purpose: XPXPlayer Loading Screen, show something while the player waits.
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

#include <RemoteEventStorage.h>
#include <XHTTPManager.h>
#include <CommonEngine.h>
#include <CLua.hpp>
#include <RoXML.h>
#include <codecvt>

namespace XPX
{
	// connection timeout, then client quits.
	constexpr int XPLICIT_TIMEOUT = 1000;

	LoadingComponent::LoadingComponent() 
		:
		mNetwork(nullptr), 
		mTimeout(XPLICIT_TIMEOUT),
		mLoadingTexture(nullptr)
	{
		if (auto node = CAD->getSceneManager()->getSceneNodeFromName("LoadingCube");
			node)
			mLoadingTextureNode = (decltype(mLoadingTextureNode))node;

		mLoadingTexture = CAD->getVideoDriver()->getTexture("xpxplayer.png");

		auto mesh = CAD->getSceneManager()->getGeometryCreator()->createCubeMesh();
		mLoadingTextureNode = CAD->getSceneManager()->addMeshSceneNode(mesh);
		mLoadingTextureNode->setName("LoadingCube");

		for (size_t i = 0; i < mLoadingTextureNode->getMaterialCount(); i++)
		{
			mLoadingTextureNode->setMaterialTexture(i, mLoadingTexture);
		}

		mLoadingTextureNode->setMaterialFlag(EMF_LIGHTING, false);
		mLoadingTextureNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);

		// this needs to match the scene.
		mLoadingTextureNode->getMaterial(0).DiffuseColor.setAlpha(0x40);

		auto cam = ComponentSystem::get_singleton_ptr()->add<XPX::LocalCameraComponent>();

        CAD->getSceneManager()->setActiveCamera(cam->get());
		CAD->getSceneManager()->getActiveCamera()->setTarget(mLoadingTextureNode->getPosition());
    }

	LoadingComponent::~LoadingComponent() = default;

	bool LoadingComponent::mEnabled = true;

	void LoadingComponent::update(void* class_ptr)
	{
		auto* self = (LoadingComponent*)class_ptr;

		auto rot = self->mLoadingTextureNode->getRotation();

		rot.Y += 1;
		rot.Z += 1;

		self->mLoadingTextureNode->setRotation(rot);

		if (!self ||
            !self->mNetwork) return;

		NetworkPacket packet{};
		self->mNetwork->read(packet);

		if (packet.cmd[XPLICIT_NETWORK_CMD_BAN] == NETWORK_CMD_BAN)
		{
			ComponentSystem::get_singleton_ptr()->add<PopupComponent>(
				[]() { 
					CAD->closeDevice();
				}, POPUP_TYPE::BANNED, "StopPopup");

			mEnabled = false;

			ComponentSystem::get_singleton_ptr()->remove(self->mNetwork);

			return;
		}

		if (packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] == NETWORK_CMD_ACCEPT)
		{
			ImGUI::UIFont::get_label_font()->draw("Acknowledge done, jumping in...", irr::core::recti(10, 10, 10, 10), ImGUI::ImColor(255, 255, 255, 255));

			packet.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;

			self->mLoadingTextureNode->setVisible(false);

			ComponentSystem::get_singleton_ptr()->add<XPX::RemoteEventStorage>(self->mNetwork);

			auto& hash = packet.hash;

			auto& public_hash = packet.public_hash;

			EventSystem::get_singleton_ptr()->add<LocalHumanoidMoveEvent>(public_hash);
			EventSystem::get_singleton_ptr()->add<LocalMenuEvent>();

			auto replicator = ComponentSystem::get_singleton_ptr()->add<LocalReplicationComponent>(hash);

			LocalReplicationComponent::update((void*)replicator);

			ComponentSystem::get_singleton_ptr()->add<HUDComponent>(public_hash);
			
			const auto cam = ComponentSystem::get_singleton_ptr()->get<LocalCameraComponent>("LocalCameraComponent");
			const auto local_player = ComponentSystem::get_singleton_ptr()->add<LocalHumanoidComponent>(public_hash, true);

			const auto monitor = EventSystem::get_singleton_ptr()->add<LocalNetworkMonitorEvent>(hash, public_hash);

			local_player->attach(cam);
			self->mNetwork->set_hash(hash);

			monitor->ID = packet.buffer;

			local_player->get_class()->insert("Id", fmt::format("'{}'", monitor->ID).c_str());

			XPLICIT_INFO(monitor->ID);

			monitor->Endpoint = XPLICIT_XASSET_ENDPOINT;

			monitor->HTTP = std::make_unique<XHTTPManager>();
			monitor->HTTP->set_endpoint(monitor->Endpoint);

			mEnabled = false;

			return;
		}
		else
		{
			--self->mTimeout;

			// peek after the ++timeout, or retry
			if (self->mTimeout < 0)
			{
				self->mLoadingTextureNode->setVisible(false);

				ComponentSystem::get_singleton_ptr()->add<PopupComponent>(
					[]() {
						CAD->closeDevice();
					}, POPUP_TYPE::NETWORK, "StopPopup");

				ComponentSystem::get_singleton_ptr()->remove(self->mNetwork);

				mEnabled = false;

				return;
			}
			else
			{
				ImGUI::UIFont::get_label_font()->draw("Connecting to server...", irr::core::recti(10, 10, 10, 10), ImGUI::ImColor(255, 255, 255, 255));

			}

            std::this_thread::sleep_for(std::chrono::microseconds(250));
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
		ComponentSystem::get_singleton_ptr()->add<PopupComponent>([]()-> void {
			CAD->closeDevice();
			}, POPUP_TYPE::NETWORK);
	}
}