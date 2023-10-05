/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: LoadingComponent.cpp
 *			Purpose: Game Loading screen, while the network sends up the needed files.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "LoadingScreenComponent.h"

#include "LocalReplicationComponent.h"
#include "LocalNetworkMonitorEvent.h"
#include "LocalCharacterComponent.h"
#include "LocalCameraComponent.h"
#include "ChatBoxComponent.h"
#include "LocalMenuEvent.h"
#include "ComUI.h"
#include "App.h"

#include <XHTTPManager.h>
#include <Enums.h>
#include <codecvt>

#define XPLICIT_WAIT_FOR std::chrono::milliseconds(500)

namespace XPX
{
    //! connection timeout until client stops trying to ACK.
    constexpr int XPLICIT_TIMEOUT = 100;

    /// <summary>
    /// Component constructor.
    /// </summary>
    LoadingScreenComponent::LoadingScreenComponent() noexcept
        :
        mNetwork(nullptr),
        mTimeout(0)
    {
        LocalCameraComponent* cam = ComponentSystem::get_singleton_ptr()->add<LocalCameraComponent>();
    }

    LoadingScreenComponent::~LoadingScreenComponent() = default;

    bool LoadingScreenComponent::StartLoad = true;

    void LoadingScreenComponent::update(void* class_ptr)
    {
        if (!LoadingScreenComponent::StartLoad)
            return;

        auto self = (LoadingScreenComponent*)class_ptr;

        if (!self ||
            !self->mNetwork) return;

        NetworkPacket packet{};

        if (!self->mNetwork->read(packet))
            return;

        if (packet.cmd[XPLICIT_NETWORK_CMD_BAN] == NETWORK_CMD_BAN)
        {
            ComponentSystem::get_singleton_ptr()->add<PopupComponent>(
                []() {
                    std::exit(60);
            }, POPUP_TYPE::BANNED, "StopPopup");

            StartLoad = false;

            ComponentSystem::get_singleton_ptr()->remove(self->mNetwork);

            return;
        }

        if (packet.cmd[XPLICIT_NETWORK_CMD_ACCEPT] == NETWORK_CMD_ACCEPT &&
            packet.cmd[XPLICIT_NETWORK_CMD_SPAWN] == NETWORK_CMD_SPAWN)
        {
            auto hash = packet.hash;
            auto public_hash = packet.public_hash;

            const auto monitor = EventSystem::get_singleton_ptr()->add<LocalNetworkMonitorEvent>(hash, public_hash);

            monitor->ID = packet.additional_data;

            ComponentSystem::get_singleton_ptr()->add<ChatBoxComponent>(packet.additional_data);
            ComponentSystem::get_singleton_ptr()->add<LocalReplicationComponent>(hash, monitor->ID);

            self->mNetwork->set_hash(hash);

            monitor->Endpoint = XPLICIT_XASSET_ENDPOINT;

            monitor->HTTP = std::make_unique<XHTTPManager>();
            monitor->HTTP->set_endpoint(monitor->Endpoint);

            EventSystem::get_singleton_ptr()->add<LocalHumanoidMoveEvent>(hash);
            EventSystem::get_singleton_ptr()->add<LocalMenuEvent>();

            ComponentSystem::get_singleton_ptr()->add<LocalCharacterComponent>(public_hash, true);
            ComponentSystem::get_singleton_ptr()->add<HUDComponent>(public_hash);

            self->on_join();

            StartLoad = false;
        }
        else
        {
            //! peek after the ++timeout, or retry
            if (self->mTimeout >= XPLICIT_TIMEOUT)
            {
                ComponentSystem::get_singleton_ptr()->add<PopupComponent>(
                    []() {
                        std::exit(30);
                 }, POPUP_TYPE::NETWORK, "TimeoutPopup");

                ComponentSystem::get_singleton_ptr()->remove(self->mNetwork);

                self->on_timeout();

                StartLoad = false;
            }
        }
    }

    void LoadingScreenComponent::connect(Utils::UriParser& ip)
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
                NetworkPacket login_packet{};

                while (StartLoad)
                {
                    if (!mNetwork)
                    {
                        mTimeout = XPLICIT_TIMEOUT;
                        return;
                    }

                    login_packet.cmd[XPLICIT_NETWORK_CMD_BEGIN] = NETWORK_CMD_BEGIN;
                    login_packet.cmd[XPLICIT_NETWORK_CMD_ACK] = NETWORK_CMD_ACK;
                    login_packet.size = sizeof(NetworkPacket);

                    mNetwork->send(login_packet);

                    std::this_thread::sleep_for(XPLICIT_WAIT_FOR);
                    ++this->mTimeout;
                }
                });

            thrd.detach();
        }
    }

    void LoadingScreenComponent::reset() noexcept
    {
        ComponentSystem::get_singleton_ptr()->remove(this->mNetwork);

        ComponentSystem::get_singleton_ptr()->add<PopupComponent>([]()-> void {
            std::exit(25);
        }, POPUP_TYPE::NETWORK);
    }
}

#undef XPLICIT_WAIT_FOR
