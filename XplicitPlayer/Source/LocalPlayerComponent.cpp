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
#include "GameMenuUI.h"

#include <CommonEngine.h>

namespace Xplicit::Player
{
	constexpr const short XPLICIT_NETWORK_DELAY = 100;
	constexpr const short XPLICIT_PLAYER_COOLDOWN = 2;

	LocalPlayerComponent::LocalPlayerComponent(const int64_t& public_hash)
		:
		StaticMesh("xplicit-player.dae"),
		mPublicHash(public_hash),
		mCam(nullptr), 
		mPacket(),
		mPos(0.f, 0.f, 0.f)
	{
		mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");

		XPLICIT_ASSERT(mNetwork);

#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("LocalPlayerComponent::LocalPlayerComponent");
#endif
	}

	LocalPlayerComponent::~LocalPlayerComponent()
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("LocalPlayerComponent::~LocalPlayerComponent");
#endif
	}

	COMPONENT_TYPE LocalPlayerComponent::type() noexcept { return COMPONENT_ACTOR; }

	const char* LocalPlayerComponent::name() noexcept { return ("LocalPlayerComponent"); }

	void LocalPlayerComponent::update()
	{
		if (mNetwork == nullptr) return;
		if (!mNetwork->read(mPacket)) return;
		
		if (mPublicHash == 0)
			mPublicHash = mPacket.public_hash;

		if (mPacket.public_hash == mPublicHash)
		{
			if (mPacket.cmd[XPLICIT_NETWORK_CMD_POS] == NETWORK_CMD_POS &&
				mPacket.cmd[XPLICIT_NETWORK_CMD_ACCEPT] == NETWORK_CMD_ACCEPT)
			{
				const float delta = mPacket.pos[XPLICIT_NETWORK_DELTA];
				const float xSpeed = mPacket.pos[XPLICIT_NETWORK_X] * delta;
				const float zSpeed = mPacket.pos[XPLICIT_NETWORK_Z] * delta;
				const float ySpeed = mPacket.pos[XPLICIT_NETWORK_Y] * delta;

				mPos.Z = zSpeed;
				mPos.X = xSpeed;
				mPos.Y = ySpeed;

				std::cout << delta << std::endl;
				std::cout << xSpeed << " " << ySpeed << " " << zSpeed << std::endl;

				this->node()->setPosition(mPos);
				this->mCam->get()->setPosition(mPos);
			}
		}
	}

	void LocalPlayerComponent::attach(LocalCameraComponent* cam) noexcept
	{ 
		if (cam)
			mCam = cam; 
	}

	vector3df LocalPlayerComponent::get_pos() noexcept { return mPos; }

	LocalPlayerMoveEvent::LocalPlayerMoveEvent(const std::int64_t& public_hash)
		: 
		mPacket(), 
		mNetwork(nullptr),
		mPublicHash(public_hash)
	{
		mNetwork = ComponentManager::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");
		XPLICIT_ASSERT(mNetwork);
	}

	LocalPlayerMoveEvent::~LocalPlayerMoveEvent() = default;

	const char* LocalPlayerMoveEvent::name() noexcept { return ("LocalPlayerMoveEvent"); }

	const int64_t& LocalPlayerComponent::id() noexcept { return mPublicHash; }

	/* LocalPlayer movement logic */
	void LocalPlayerMoveEvent::operator()()
	{
		if (mNetwork == nullptr)
			return;

		auto traits = Root::get_singleton_ptr()->Keyboard->get_layout();

		if (KB->key_down(traits.mForward))
		{
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
