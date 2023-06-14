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
		mEvent(EventDispatcher::get_singleton_ptr()->add<LocalMoveEvent>(public_hash))
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
			EventDispatcher::get_singleton_ptr()->remove(mEvent);
	}

	LocalPlayerComponent::INSTANCE_TYPE LocalPlayerComponent::type() noexcept { return INSTANCE_ACTOR; }

	const char* LocalPlayerComponent::name() noexcept { return ("LocalPlayerComponent"); }

	void LocalPlayerComponent::update()
	{
		if (!mNetwork ||
			!mNode)
			return;

		if (mPublicHash == -1)
			return;

		mPacket = mNetwork->get();

		if (mPacket.public_hash == mPublicHash)
		{
			if (mPacket.cmd[XPLICIT_NETWORK_CMD_POS] == NETWORK_CMD_POS &&
				mPacket.cmd[XPLICIT_NETWORK_CMD_ACCEPT] == NETWORK_CMD_ACCEPT)
			{
				auto pos = mNode->getAbsolutePosition();

				f32 delta = mPacket.speed[XPLICIT_NETWORK_DELTA];

				auto x_speed = mPacket.speed[XPLICIT_NETWORK_X] * delta;
				auto z_speed = mPacket.speed[XPLICIT_NETWORK_Z] * delta;



				mNode->setPosition(pos);
				mNetwork->send(mPacket);
			}
		}
	}

	void LocalPlayerComponent::attach(CameraComponent* cam) noexcept { mCam = cam; }

	void LocalPlayerComponent::set_pos(const vector3df& newPos) noexcept { mNode->setPosition(newPos); }
	vector3df LocalPlayerComponent::get_pos() noexcept { return mNode->getAbsolutePosition(); }

	LocalMoveEvent::LocalMoveEvent(const std::int64_t& public_hash)
		: 
		mPacket(), 
		mNetwork(nullptr),
		mPublicHash(public_hash),
		mCooldown(0)
	{
		mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");
		XPLICIT_ASSERT(mNetwork);
	}

	LocalMoveEvent::~LocalMoveEvent() {}

	const char* LocalMoveEvent::name() noexcept { return ("LocalMoveEvent"); }

	const int64_t& LocalPlayerComponent::id() noexcept { return mPublicHash; }

	/* LocalPlayer movement logic */
	void LocalMoveEvent::operator()()
	{
		if (mNetwork == nullptr || 
			mPublicHash == -1)
			return;

		static f32 then = IRR->getTimer()->getTime();
		f32 delta = (IRR->getTimer()->getTime() - then) / XPLICIT_DELTA_TIME;

		if (KB->key_down(Details::KEY_KEY_W))
		{
			mPacket.public_hash = mPublicHash;

			mPacket.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;
			mPacket.cmd[XPLICIT_NETWORK_CMD_FORWARD] = NETWORK_CMD_FORWARD;

			mNetwork->send(mPacket);
		}

		if (KB->key_down(Details::KEY_KEY_S))
		{
			mPacket.public_hash = mPublicHash;

			mPacket.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;
			mPacket.cmd[XPLICIT_NETWORK_CMD_BACKWARD] = NETWORK_CMD_BACKWARD;

			mNetwork->send(mPacket);
		}

		if (KB->key_down(Details::KEY_KEY_D))
		{
			mPacket.public_hash = mPublicHash;

			mPacket.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;
			mPacket.cmd[XPLICIT_NETWORK_CMD_RIGHT] = NETWORK_CMD_RIGHT;

			mNetwork->send(mPacket);
		}

		if (KB->key_down(Details::KEY_KEY_A))
		{
			mPacket.public_hash = mPublicHash;

			mPacket.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;
			mPacket.cmd[XPLICIT_NETWORK_CMD_LEFT] = NETWORK_CMD_LEFT;

			mNetwork->send(mPacket);
		}
	}
}
