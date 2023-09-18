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
#include "Application.h"
#include "MenuUI.h"

#include <RemoteEventStorage.h>
#include <XHTTPManager.h>
#include <Enums.h>
#include <CLua.hpp>
#include <RoXML.h>
#include <codecvt>

namespace XPX
{
    // connection timeout until client stops trying to ACK.
    constexpr int XPLICIT_TIMEOUT = 100;

    LoadingComponent::LoadingComponent()
        :
        mNetwork(nullptr),
        mTimeout(0),
        mLoadingFrame()
    {
        mLoadingFrame.BackgroundColor.setRed(45);
        mLoadingFrame.BackgroundColor.setGreen(45);
        mLoadingFrame.BackgroundColor.setBlue(45);
        mLoadingFrame.BackgroundColor.setAlpha(255);

        auto cam = ComponentSystem::get_singleton_ptr()->add<XPX::LocalCameraComponent>();
        CAD->getSceneManager()->setActiveCamera(cam->get());
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

        self->mLoadingFrame.update(self->mLoadingFrame.BackgroundColor);

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

            ComponentSystem::get_singleton_ptr()->add<RemoteEventStorage>(self->mNetwork);

            auto hash = packet.hash;
            auto public_hash = packet.public_hash;

            EventSystem::get_singleton_ptr()->add<LocalHumanoidMoveEvent>(hash);
            EventSystem::get_singleton_ptr()->add<LocalMenuEvent>();

            ComponentSystem::get_singleton_ptr()->add<LocalReplicationComponent>(hash);
            ComponentSystem::get_singleton_ptr()->add<HUDComponent>(public_hash);

            const auto cam = ComponentSystem::get_singleton_ptr()->get<LocalCameraComponent>("LocalCameraComponent");
            const auto local_player = ComponentSystem::get_singleton_ptr()->add<LocalHumanoidComponent>(public_hash, true);

            const auto monitor = EventSystem::get_singleton_ptr()->add<LocalNetworkMonitorEvent>(hash, public_hash);

            local_player->attach(cam);
            self->mNetwork->set_hash(hash);

            ComponentSystem::get_singleton_ptr()->add<ChatBoxComponent>(packet.additional_data);
            monitor->ID = packet.additional_data;

            local_player->get_class()->insert("PlayerId", fmt::format("'{}'", monitor->ID).c_str());

            XPLICIT_INFO(monitor->ID);

            monitor->Endpoint = XPLICIT_XASSET_ENDPOINT;

            monitor->HTTP = std::make_unique<XHTTPManager>();
            monitor->HTTP->set_endpoint(monitor->Endpoint);

            ImGUI::UIFontHelper::get_label_font()->draw("Acknowledged by server, jumping in...", irr::core::recti(10, 10, 10, 10), ImGUI::ImColor(255, 255, 255, 255));

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
                String fmt("Dialing to server... (");
                fmt += std::to_string(self->mTimeout);
                fmt += ")";

                ImGUI::UIFontHelper::get_label_font()->draw(fmt.c_str(), irr::core::recti(10, 10, 10, 10), ImGUI::ImColor(255, 255, 255, 255));
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
            mLoadingFrame.W = CAD->getVideoDriver()->getScreenSize().Width;
            mLoadingFrame.H = CAD->getVideoDriver()->getScreenSize().Height;

            mLoadingFrame.X = 0;
            mLoadingFrame.Y = 0;

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
        ComponentSystem::get_singleton_ptr()->add<PopupComponent>([]()-> void {
            CAD->closeDevice();
            }, POPUP_TYPE::NETWORK);
    }
}