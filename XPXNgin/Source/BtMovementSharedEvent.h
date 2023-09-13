/*
 * =====================================================================
 *
 *			XPXServer
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
#include "Event.h"
#include "Root.h"

#ifdef XPLICIT_BULLET

namespace XPX
{
    typedef ISceneNode* BtSceneNode;

    /// <summary>
    /// Bullet event type.
    /// </summary>

    class XPLICIT_API BtMovementSharedEvent : public Event
    {
    public:
        BtMovementSharedEvent() noexcept;
        ~BtMovementSharedEvent() override;

    public:
        BtMovementSharedEvent& operator=(const BtMovementSharedEvent&) = default;
        BtMovementSharedEvent(const BtMovementSharedEvent&) = default;

    public:
        const char* name() noexcept override;
        void operator()() override;

    public:
        void insert_node(BtSceneNode node, double mass);

    private:
        btDefaultCollisionConfiguration* mCollisionConfiguration;
        btSequentialImpulseConstraintSolver* mSolver;
        btBroadphaseInterface* mBroadphase;
        btCollisionDispatcher* mDispatcher;
        std::vector<btRigidBody*> mWorldObjs;
        btDiscreteDynamicsWorld* mWorld;

    private:
        double mTimeStamp;
        double mDeltaTime;

    };
}

#endif