/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "LocalHumanoidComponent.h"
#include "LocalMenuEvent.h"
#include "Application.h"
#include "GameMenuUI.h"

#include <CommonEngine.h>
#include <lua/CLua.hpp>

namespace Xplicit::Player
{
	constexpr const short XPLICIT_NETWORK_DELAY = 100;
	constexpr const short XPLICIT_PLAYER_COOLDOWN = 2;

	LocalHumanoidComponent::LocalHumanoidComponent(const int64_t& public_hash)
		:
		StaticBundleMesh("Character.roxml"), // relative to contents dir
		mPublicHash(public_hash),
		mCam(nullptr), 
		mPacket(),
		mPos(0.f, 2.f, 0.f)
	{
		mNetwork = ComponentSystem::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");

		XPLICIT_ASSERT(mNetwork);

#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("LocalHumanoidComponent::LocalHumanoidComponent");
#endif


		XPLICIT_INFO("Game:Spawn [EVENT]");
		Lua::CLuaStateManager::get_singleton_ptr()->run_string("Game:Spawn()");
	}

	LocalHumanoidComponent::~LocalHumanoidComponent()
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("LocalHumanoidComponent::~LocalHumanoidComponent");
#endif
	}

	PHYSICS_TYPE LocalHumanoidComponent::physics() noexcept { return PHYSICS_COMPLEX; }

	COMPONENT_TYPE LocalHumanoidComponent::type() noexcept { return COMPONENT_HUMANOID; }

	const char* LocalHumanoidComponent::name() noexcept { return ("LocalHumanoidComponent"); }

	void LocalHumanoidComponent::update()
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

				for (size_t i = 0; i < XPLICIT_BUNDLE_MAX; ++i)
				{
					if (!this->node(i))
						continue;

					auto pos = this->node(i)->getPosition();

					pos.Z += mPos.Z;
					pos.X += mPos.X;
					pos.Y += mPos.Y;

					this->node(i)->setPosition(pos);
				}

				XPLICIT_INFO("Game:Move [EVENT]");
				Lua::CLuaStateManager::get_singleton_ptr()->run_string("Game:Move()");
			}
		}
	}

	void LocalHumanoidComponent::attach(LocalCameraComponent* cam) noexcept
	{ 
		if (cam)
		{
			mCam = cam;
			this->node(XPLICIT_BUNDLE_HEAD)->addChild(mCam->get());
		}
	}

	Vector<float> LocalHumanoidComponent::get_pos() noexcept { return mPos; }

	LocalHumanoidMoveEvent::LocalHumanoidMoveEvent(const std::int64_t& public_hash)
		: 
		mPacket(), 
		mNetwork(nullptr),
		mPublicHash(public_hash)
	{
		mNetwork = ComponentSystem::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");
		XPLICIT_ASSERT(mNetwork);
	}

	LocalHumanoidMoveEvent::~LocalHumanoidMoveEvent() = default;

	const char* LocalHumanoidMoveEvent::name() noexcept { return ("LocalHumanoidMoveEvent"); }

	const int64_t& LocalHumanoidComponent::id() noexcept { return mPublicHash; }

	/* LocalPlayer movement logic */
	void LocalHumanoidMoveEvent::operator()()
	{
		if (mNetwork == nullptr)
			return;

		LocalMenuEvent* menu = EventSystem::get_singleton_ptr()->get<LocalMenuEvent>("LocalMenuEvent");

		if (menu && menu->enabled())
			return;

		auto& traits = Bites::ObjectInputSystem::get_singleton_ptr()->Layout;

		if (Bites::ObjectInputSystem::get_singleton_ptr()->key_down(traits.mForward))
		{
			mPacket.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;

			mPacket.cmd[XPLICIT_NETWORK_CMD_FORWARD] = NETWORK_CMD_FORWARD;

			mPacket.cmd[XPLICIT_NETWORK_CMD_LEFT] = NETWORK_CMD_INVALID;
			mPacket.cmd[XPLICIT_NETWORK_CMD_RIGHT] = NETWORK_CMD_INVALID;
			mPacket.cmd[XPLICIT_NETWORK_CMD_BACKWARD] = NETWORK_CMD_INVALID;

			mNetwork->send(mPacket);

			return;
		}

		if (Bites::ObjectInputSystem::get_singleton_ptr()->key_down(traits.mBackward))
		{
			mPacket.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;

			mPacket.cmd[XPLICIT_NETWORK_CMD_FORWARD] = NETWORK_CMD_INVALID;
			
			mPacket.cmd[XPLICIT_NETWORK_CMD_LEFT] = NETWORK_CMD_INVALID;
			mPacket.cmd[XPLICIT_NETWORK_CMD_RIGHT] = NETWORK_CMD_INVALID;
			mPacket.cmd[XPLICIT_NETWORK_CMD_BACKWARD] = NETWORK_CMD_BACKWARD;

			mNetwork->send(mPacket);

			return;
		}

		if (Bites::ObjectInputSystem::get_singleton_ptr()->key_down(traits.mRight))
		{
			mPacket.cmd[XPLICIT_NETWORK_CMD_BACKWARD] = NETWORK_CMD_INVALID;
			mPacket.cmd[XPLICIT_NETWORK_CMD_FORWARD] = NETWORK_CMD_INVALID;
			mPacket.cmd[XPLICIT_NETWORK_CMD_LEFT] = NETWORK_CMD_INVALID;

			mPacket.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;
			mPacket.cmd[XPLICIT_NETWORK_CMD_RIGHT] = NETWORK_CMD_RIGHT;

			mNetwork->send(mPacket);

			return;
		}

		if (Bites::ObjectInputSystem::get_singleton_ptr()->key_down(traits.mLeft))
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
