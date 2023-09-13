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
#include "RigidBody.h"

#ifdef __XPLICIT_DLL__
#include "ClassComponent.h"
#include "NginCore.h"
#include "Event.h"
#include "Root.h"
#else
#include <ClassComponent.h>
#include <NginCore.h>
#include <Event.h>
#include <Root.h>
#endif // ifdef __XPLICIT_DLL__

namespace XPX
{
    class ClassComponent;

    typedef ClassComponent* NpSceneNode;

    class NPLICIT_API NpMovementSharedEvent : public Event
    {
    public:
        NpMovementSharedEvent() noexcept;
        ~NpMovementSharedEvent() noexcept override;

    public:
        NpMovementSharedEvent& operator=(const NpMovementSharedEvent&) = default;
        NpMovementSharedEvent(const NpMovementSharedEvent&) = default;

    public:
        const char* name() noexcept override;
        void operator()() override;

    public:
        void remove_node(NpSceneNode node);
        void insert_node(NpSceneNode node);

    private:
        std::vector<NpSceneNode> mWorldNodes;

    private:
        NplicitFloat mTimeStamp;
        NplicitFloat mDeltaTime;

    };
}
