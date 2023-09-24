/*
 * =====================================================================
 *
 *			XPXNgin.Physics
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

 // bug count = 0

 /**
 * @file
 */

#pragma once

#include "Config.h"

#ifndef __XPLICIT_DLL__
#include <ClassComponent.h>
#include <NginCore.h>
#include <Event.h>
#include <Root.h>
#endif // ifdef __XPLICIT_DLL__

namespace XPX
{
    class ClassComponent;

    typedef ClassComponent* NpSceneNode;

    class NPLICIT_API NpMovementServerEvent : public Event
    {
    public:
        enum
        {
            NP_DYNAMIC,
            NP_STATIC,
        };

    public:
        NpMovementServerEvent() noexcept;
        ~NpMovementServerEvent() noexcept override;

    public:
        NpMovementServerEvent& operator=(const NpMovementServerEvent&) = default;
        NpMovementServerEvent(const NpMovementServerEvent&) = default;

    public:
        const char* name() noexcept override;
        void operator()() override;

    public:
        bool remove_node(NpSceneNode node);
        bool insert_node(NpSceneNode node, int node_kind = NP_DYNAMIC);

    private:
        std::vector<NpSceneNode> mWorldNodes;

    };
}

