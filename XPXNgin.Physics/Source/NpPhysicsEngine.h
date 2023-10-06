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

#   include <Util.h>
#   include <NginCore.h>
#   include <Event.h>
#   include <Root.h>

#endif // ifdef __XPLICIT_DLL__

namespace XPX
{
    typedef XPXAttribute* NpNodePtr;
    typedef XPXAttribute NpNodeType;

    class NPLICIT_API NpPhysicsEvent : public Event
    {
    public:
        explicit NpPhysicsEvent() noexcept;
        ~NpPhysicsEvent() noexcept override;

    public:
        NpPhysicsEvent& operator=(const NpPhysicsEvent&) = default;
        NpPhysicsEvent(const NpPhysicsEvent&) = default;

    public:
        const char* name() noexcept override;
        void operator()() override;

    public:
        bool insert_node(NpNodePtr node);
        bool remove_node(NpNodePtr node);

    private:
        std::vector<NpNodePtr> mWorldNodes;
        bool mNetworkBased;

    };

    NPLICIT_API bool NplicitAddGround(NpNodePtr node);
}

