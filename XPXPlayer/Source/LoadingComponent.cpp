/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: LoadingComponent.cpp
 *			Purpose: XPXPlayer Loading Screen, show something while the player waits
 *          and manages the connection process.
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
#include "ChatBoxComponent.h"
#include "LoadingComponent.h"
#include "LocalMenuEvent.h"
#include "ClientUtils.h"
#include "MenuUI.h"
#include "App.h"

#include <XHTTPManager.h>
#include <Enums.h>
#include <CLua.hpp>
#include <RoXML.h>
#include <codecvt>

namespace XPX
{
    // connection timeout until client stops trying to ACK.
    constexpr int XPLICIT_TIMEOUT = 100;

    /// <summary>
    /// Loading Component constructor.
    /// </summary>
    LoadingComponent::LoadingComponent() noexcept
        :
        mLoadingTexture(nullptr),
        mNetwork(nullptr),
        mTimeout(0)
    {
        mLoadingTexture = CAD->getVideoDriver()->getTexture("UIBadgeVerified.png");
        LocalCameraComponent* cam = ComponentSystem::get_singleton_ptr()->add<LocalCameraComponent>();
    }

    LoadingComponent::~LoadingComponent() = default;

    bool LoadingComponent::StartLoad = true;

    void LoadingComponent::update(void* class_ptr)
    {
        if (!LoadingComponent::StartLoad)
            return;

        auto* self = (LoadingComponent*)class_ptr;

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

            StartLoad = false;

            ComponentSystem::get_singleton_ptr()->remove(self->mNetwork);

            return;
        }

        if (packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] == NETWORK_CMD_ACCEPT)
        {
            packet.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;

            auto hash = packet.hash;
            auto public_hash = packet.public_hash;

            const auto monitor = EventSystem::get_singleton_ptr()->add<LocalNetworkMonitorEvent>(hash, public_hash);

            self->mNetwork->read(packet);

            ComponentSystem::get_singleton_ptr()->add<ChatBoxComponent>(packet.additional_data);
            monitor->ID = packet.additional_data;

            XPLICIT_INFO("XPX_ID:" + monitor->ID);

            EventSystem::get_singleton_ptr()->add<LocalHumanoidMoveEvent>(hash);
            EventSystem::get_singleton_ptr()->add<LocalMenuEvent>();

            ComponentSystem::get_singleton_ptr()->add<LocalReplicationComponent>(hash, monitor->ID);
            ComponentSystem::get_singleton_ptr()->add<HUDComponent>(public_hash);

            ComponentSystem::get_singleton_ptr()->add<LocalHumanoidComponent>(public_hash, true);

            self->mNetwork->set_hash(hash);

            monitor->Endpoint = XPLICIT_XASSET_ENDPOINT;

            monitor->HTTP = std::make_unique<XHTTPManager>();
            monitor->HTTP->set_endpoint(monitor->Endpoint);

            StartLoad = false;
        }
        else
        {
            // peek after the ++timeout, or retry
            if (self->mTimeout >= XPLICIT_TIMEOUT)
            {
                ComponentSystem::get_singleton_ptr()->add<PopupComponent>(
                    []() {
                        CAD->closeDevice();
                    }, POPUP_TYPE::NETWORK, "TimeoutPopup");

                ComponentSystem::get_singleton_ptr()->remove(self->mNetwork);

                StartLoad = false;
            }
            else
            {
                CAD->getVideoDriver()->draw2DImage(self->mLoadingTexture,
                    recti(vector2d(10, 10), dimension2di(self->mLoadingTexture->getSize().Width,
                        self->mLoadingTexture->getSize().Height)),
                    recti(), nullptr, nullptr, true);
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
                while (StartLoad)
                {
                    if (!mNetwork)
                    {
                        mTimeout = XPLICIT_TIMEOUT;
                        return;
                    }

                    NetworkPacket spawn{};

                    spawn.cmd[XPLICIT_NETWORK_CMD_BEGIN] = NETWORK_CMD_BEGIN;
                    spawn.size = sizeof(NetworkPacket);

                    mNetwork->send(spawn);

                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    ++this->mTimeout;
                }
                });

            thrd.detach();
        }
    }

    void LoadingComponent::reset() noexcept
    {
        ComponentSystem::get_singleton_ptr()->remove(this->mNetwork);

        ComponentSystem::get_singleton_ptr()->add<PopupComponent>([]()-> void {
            CAD->closeDevice();
            }, POPUP_TYPE::NETWORK);
    }
}