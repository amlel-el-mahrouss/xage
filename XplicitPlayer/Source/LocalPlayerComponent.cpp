/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: LocalActor.cpp
 *			Purpose: Client-side Player
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

namespace Xplicit::Client
{
	constexpr const short XPLICIT_PLAYER_COOLDOWN = 2;

	LocalPlayerComponent::LocalPlayerComponent(const int64_t& public_hash)
		: Component(), 
		MeshComponentHelper("Character.dae"), 
		mPacket(), 
		mCam(nullptr), 
		mPublicHash(public_hash)
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

				//			   XPLICIT_DEFAULT_VEL
				auto x_speed = mPacket.speed[XPLICIT_NETWORK_X] * delta;
				auto z_speed = mPacket.speed[XPLICIT_NETWORK_Z] * delta;

				if (pos.X == x_speed ||
					pos.Z == z_speed)
					return;

				if (pos.X > x_speed ||
					pos.X < x_speed)
					pos.X = x_speed;

				if (pos.Z > z_speed ||
					pos.Z < z_speed)
					pos.Z = z_speed;

				mNode->setPosition(pos);
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

	/* main movement logic */
	void LocalMoveEvent::operator()()
	{
		if (mNetwork == nullptr || 
			mPublicHash == -1)
			return;

		static auto node = ComponentManager::get_singleton_ptr()->get<LocalPlayerComponent>("LocalPlayerComponent");
		if (!node) return;

		mPacket.cmd[XPLICIT_NETWORK_CMD_FORWARD] = NETWORK_CMD_INVALID;
		mPacket.cmd[XPLICIT_NETWORK_CMD_BACKWARD] = NETWORK_CMD_INVALID;
		mPacket.cmd[XPLICIT_NETWORK_CMD_RIGHT] = NETWORK_CMD_INVALID;
		mPacket.cmd[XPLICIT_NETWORK_CMD_LEFT] = NETWORK_CMD_INVALID;

		auto speed = XPLICIT_DEFAULT_VEL;
		auto pos = node->get_pos();

		static f32 then = IRR->getTimer()->getTime();
		f32 delta = (IRR->getTimer()->getTime() - then) / XPLICIT_DELTA_TIME;

		if (KB->key_down(Details::KEY_KEY_W))
		{
			mPacket.public_hash = mPublicHash;
			mPacket.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;
			mPacket.cmd[XPLICIT_NETWORK_CMD_FORWARD] = NETWORK_CMD_FORWARD;

			pos.Z -= speed * then;

			mNetwork->send(mPacket);
		}

		if (KB->key_down(Details::KEY_KEY_S))
		{
			mPacket.public_hash = mPublicHash;
			mPacket.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;
			mPacket.cmd[XPLICIT_NETWORK_CMD_BACKWARD] = NETWORK_CMD_BACKWARD;

			pos.Z += speed * then;

			mNetwork->send(mPacket);
		}

		if (KB->key_down(Details::KEY_KEY_D))
		{
			mPacket.public_hash = mPublicHash;
			mPacket.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;
			mPacket.cmd[XPLICIT_NETWORK_CMD_RIGHT] = NETWORK_CMD_RIGHT;

			pos.X -= speed * then;
			
			mNetwork->send(mPacket);
		}

		if (KB->key_down(Details::KEY_KEY_A))
		{
			mPacket.public_hash = mPublicHash;
			mPacket.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;
			mPacket.cmd[XPLICIT_NETWORK_CMD_LEFT] = NETWORK_CMD_LEFT;

			pos.X += speed * then;

			mNetwork->send(mPacket);
		}

		node->set_pos(pos);
	}
}
