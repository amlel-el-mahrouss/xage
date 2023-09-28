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

    class NPLICIT_API NpPhysicsEvent : public Event
    {
    public:
        enum
        {
            NP_DYNAMIC,
            NP_STATIC,
        };

    public:
        NpPhysicsEvent() noexcept;
        ~NpPhysicsEvent() noexcept override;

    public:
        NpPhysicsEvent& operator=(const NpPhysicsEvent&) = default;
        NpPhysicsEvent(const NpPhysicsEvent&) = default;

    public:
        const char* name() noexcept override;
        void operator()() override;

    public:
        bool remove_node(NpNodePtr node);
        bool insert_node(NpNodePtr node, int node_kind = NP_DYNAMIC);

    private:
        std::vector<NpNodePtr> mWorldNodes;

    };

    NPLICIT_API bool NplicitAddGround(NpNodePtr node);
}

