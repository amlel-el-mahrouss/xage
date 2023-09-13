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

#include "BtMovementSharedEvent.h"
#include "Root.h"

#ifdef XPLICIT_BULLET

namespace XPX
{
    BtMovementSharedEvent::BtMovementSharedEvent() noexcept
        : mCollisionConfiguration(new btDefaultCollisionConfiguration()),
        mBroadphase(new btAxisSweep3(btVector3(-1000, -1000, -1000), btVector3(1000, 1000, 1000))),
        mDispatcher(new btCollisionDispatcher(mCollisionConfiguration)),
        mSolver(new btSequentialImpulseConstraintSolver()),
        mWorld(new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfiguration)),
        mWorldObjs(),
        mTimeStamp(CAD->getTimer()->getTime()),
        mDeltaTime(0.0)
    {

    }

    BtMovementSharedEvent::~BtMovementSharedEvent()
    {
        delete mSolver;
        delete mDispatcher;
        delete mBroadphase;
        delete mCollisionConfiguration;

        for (auto& object : mWorldObjs)
        {
            ISceneNode* node = static_cast<irr::scene::ISceneNode*>(object->getUserPointer());
            node->remove();

            // Remove the object from the world
            mWorld->removeRigidBody(object);

            // Free memory
            delete object->getMotionState();
            delete object->getCollisionShape();
            delete object;
        }

        delete mWorld;
    }

    const char* BtMovementSharedEvent::name() noexcept { return "BtMovementSharedEvent"; }

    void BtMovementSharedEvent::operator()()
    {
        mDeltaTime = CAD->getTimer()->getTime() - mTimeStamp;
        mTimeStamp = CAD->getTimer()->getTime();

        mWorld->stepSimulation(mDeltaTime, XPX_DELTA);

        for (auto* objs : mWorldObjs)
        {
            irr::scene::ISceneNode* node = static_cast<irr::scene::ISceneNode*>(objs->getUserPointer());

            btVector3 Point = objs->getCenterOfMassPosition();
            node->setPosition(irr::core::vector3df(static_cast<irr::f32>(Point[0]),
                static_cast<irr::f32>(Point[1]),
                static_cast<irr::f32>(Point[2])));

            irr::core::vector3df Euler;

            const btQuaternion& tQuat = objs->getOrientation();
            irr::core::quaternion q(tQuat.getX(), tQuat.getY(), tQuat.getZ(), tQuat.getW());
            q.toEuler(Euler);

            Euler *= irr::core::RADTODEG;

            node->setRotation(Euler);
        }
    }

    void BtMovementSharedEvent::insert_node(BtSceneNode node, double mass)
    {
        btVector3 tPosition(node->getPosition().X, node->getPosition().Y, node->getPosition().Z);
        btVector3 tScale(node->getScale().X, node->getScale().Y, node->getScale().Z);

        btTransform transform;

        transform.setIdentity();
        transform.setOrigin(tPosition);

        btDefaultMotionState* MotionState = new btDefaultMotionState(transform);

        // Shape of the rigid body
        btVector3 HalfExtents(tScale.x() * 0.5f, tScale.y() * 0.5f, tScale.z() * 0.5f);
        btCollisionShape* Shape = new btBoxShape(HalfExtents);

        // Specify the mass, use it to calculate the local inertia
        btVector3 LocalInertia;
        Shape->calculateLocalInertia(mass, LocalInertia);

        btRigidBody::btRigidBodyConstructionInfo info(mass, MotionState, Shape, LocalInertia);

        // Create the rigid body object
        btRigidBody* rigidBody = new btRigidBody(info);

        // Store a pointer to the irrlicht node so we can update it later
        rigidBody->setUserPointer((void*)(node));

        // Add it to the world
        mWorld->addRigidBody(rigidBody);
        mWorldObjs.push_back(rigidBody);
    }
}

#endif