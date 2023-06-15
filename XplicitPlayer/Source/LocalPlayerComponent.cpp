/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: LocalActor.cpp
 *			Purpose: Client-side Player Components
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "LocalPlayerComponent.h"
#include "Application.h"
#include "CoreUI.h"

#include <Common.h>

namespace Xplicit::Player
{
	constexpr const short XPLICIT_PLAYER_COOLDOWN = 2;

	LocalPlayerComponent::LocalPlayerComponent(const int64_t& public_hash)
		: 
		Component(), 
		IMeshable("Character.dae"), 
		mPacket(), 
		mCam(nullptr), 
		mPublicHash(public_hash),
		mEvent(EventManager::get_singleton_ptr()->add<LocalPlayerMoveEvent>(public_hash))
	{
		mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");

		XPLICIT_ASSERT(mNetwork);

#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("LocalActor::LocalActor");
#endif
	}

	LocalPlayerComponent::~LocalPlayerComponent()
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("LocalActor::~LocalActor");
#endif

		if (mEvent)
			EventManager::get_singleton_ptr()->remove(mEvent);
	}

	LocalPlayerComponent::INSTANCE_TYPE LocalPlayerComponent::type() noexcept { return INSTANCE_ACTOR; }

	const char* LocalPlayerComponent::name() noexcept { return ("LocalPlayerComponent"); }

	void LocalPlayerComponent::update()
	{
		if (!mNetwork || !mNode || mPublicHash == -1) return;

		std::mutex mutex;

		mutex.lock();

		mPacket = mNetwork->get();

		if (mPacket.public_hash == mPublicHash)
		{
			if (mPacket.cmd[XPLICIT_NETWORK_CMD_POS] == NETWORK_CMD_POS &&
				mPacket.cmd[XPLICIT_NETWORK_CMD_ACCEPT] == NETWORK_CMD_ACCEPT)
			{
				auto pos = mNode->getAbsolutePosition();

				static f32 then = IRR->getTimer()->getTime();
				f32 delta = (IRR->getTimer()->getTime() - then) / XPLICIT_DELTA_TIME;

				auto xSpeed = mPacket.speed[XPLICIT_NETWORK_X] * delta;
				auto zSpeed = mPacket.speed[XPLICIT_NETWORK_Z] * delta;

				if (mPacket.cmd[XPLICIT_NETWORK_CMD_FORWARD] == NETWORK_CMD_FORWARD)
					pos.Z -= zSpeed;

				if (mPacket.cmd[XPLICIT_NETWORK_CMD_BACKWARD] == NETWORK_CMD_BACKWARD)
					pos.Z += zSpeed;

				if (mPacket.cmd[XPLICIT_NETWORK_CMD_LEFT] == NETWORK_CMD_LEFT)
					pos.X += xSpeed;

				if (mPacket.cmd[XPLICIT_NETWORK_CMD_RIGHT] == NETWORK_CMD_RIGHT)
					pos.X -= xSpeed;

				mNode->setPosition(pos);

				mPacket.cmd[XPLICIT_NETWORK_CMD_LEFT] = NETWORK_CMD_INVALID;
				mPacket.cmd[XPLICIT_NETWORK_CMD_RIGHT] = NETWORK_CMD_INVALID;
				mPacket.cmd[XPLICIT_NETWORK_CMD_BACKWARD] = NETWORK_CMD_INVALID;
				mPacket.cmd[XPLICIT_NETWORK_CMD_FORWARD] = NETWORK_CMD_INVALID;
				mPacket.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_INVALID;

				mNetwork->send(mPacket);
			}
		}

		mutex.unlock();
	}

	void LocalPlayerComponent::attach(CameraComponent* cam) noexcept
	{ 
		if (cam)
			mCam = cam; 
	}

	void LocalPlayerComponent::set_pos(const vector3df& newPos) noexcept { mNode->setPosition(newPos); }
	vector3df LocalPlayerComponent::get_pos() noexcept { return mNode->getAbsolutePosition(); }

	LocalPlayerMoveEvent::LocalPlayerMoveEvent(const std::int64_t& public_hash)
		: 
		mPacket(), 
		mNetwork(nullptr),
		mPublicHash(public_hash)
	{
		mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");
		XPLICIT_ASSERT(mNetwork);
	}

	LocalPlayerMoveEvent::~LocalPlayerMoveEvent() {}

	const char* LocalPlayerMoveEvent::name() noexcept { return ("LocalMoveEvent"); }

	const int64_t& LocalPlayerComponent::id() noexcept { return mPublicHash; }

	/* LocalPlayer movement logic */
	void LocalPlayerMoveEvent::operator()()
	{
		if (mNetwork == nullptr || 
			mPublicHash == -1)
			return;

		auto traits = ApplicationContext::get_singleton_ptr()->Keyboard->get_layout();

		if (KB->key_down(traits.mForward))
		{
			mPacket.public_hash = mPublicHash;

			mPacket.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;

			mPacket.cmd[XPLICIT_NETWORK_CMD_FORWARD] = NETWORK_CMD_FORWARD;

			mPacket.cmd[XPLICIT_NETWORK_CMD_LEFT] = NETWORK_CMD_INVALID;
			mPacket.cmd[XPLICIT_NETWORK_CMD_RIGHT] = NETWORK_CMD_INVALID;

			mPacket.cmd[XPLICIT_NETWORK_CMD_BACKWARD] = NETWORK_CMD_INVALID;

			mNetwork->send(mPacket);

			return;
		}

		if (KB->key_down(traits.mBackward))
		{
			mPacket.public_hash = mPublicHash;

			mPacket.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;

			mPacket.cmd[XPLICIT_NETWORK_CMD_FORWARD] = NETWORK_CMD_INVALID;
			
			mPacket.cmd[XPLICIT_NETWORK_CMD_LEFT] = NETWORK_CMD_INVALID;
			mPacket.cmd[XPLICIT_NETWORK_CMD_RIGHT] = NETWORK_CMD_INVALID;

			mPacket.cmd[XPLICIT_NETWORK_CMD_BACKWARD] = NETWORK_CMD_BACKWARD;

			mNetwork->send(mPacket);

			return;
		}

		if (KB->key_down(traits.mRight))
		{
			mPacket.public_hash = mPublicHash;

			mPacket.cmd[XPLICIT_NETWORK_CMD_BACKWARD] = NETWORK_CMD_INVALID;
			mPacket.cmd[XPLICIT_NETWORK_CMD_FORWARD] = NETWORK_CMD_INVALID;
			mPacket.cmd[XPLICIT_NETWORK_CMD_LEFT] = NETWORK_CMD_INVALID;

			mPacket.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;
			mPacket.cmd[XPLICIT_NETWORK_CMD_RIGHT] = NETWORK_CMD_RIGHT;

			mNetwork->send(mPacket);

			return;
		}

		if (KB->key_down(traits.mLeft))
		{
			mPacket.public_hash = mPublicHash;

			mPacket.cmd[XPLICIT_NETWORK_CMD_BACKWARD] = NETWORK_CMD_INVALID;
			mPacket.cmd[XPLICIT_NETWORK_CMD_FORWARD] = NETWORK_CMD_INVALID;
			mPacket.cmd[XPLICIT_NETWORK_CMD_RIGHT] = NETWORK_CMD_INVALID;

			mPacket.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;
			mPacket.cmd[XPLICIT_NETWORK_CMD_LEFT] = NETWORK_CMD_LEFT;

			mNetwork->send(mPacket);

			return;
		}
	}
}
