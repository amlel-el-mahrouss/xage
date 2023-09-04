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
#include <CLua/CLua.hpp>

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
		mPos(0.f, 2.f, 0.f),
		mState(HUMANOID_STATE::DEAD)
	{
		mNetwork = ComponentSystem::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");

		XPLICIT_ASSERT(mNetwork);

#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("LocalHumanoidComponent::LocalHumanoidComponent");
#endif
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

	void LocalHumanoidComponent::update(void* class_ptr)
	{
		LocalHumanoidComponent* _this = (LocalHumanoidComponent*)class_ptr;

		if (_this->mNetwork == nullptr) return;
		if (!_this->mNetwork->read(_this->mPacket)) return;

		if (_this->mPublicHash == 0)
			_this->mPublicHash = _this->mPacket.public_hash;

		if (_this->mPacket.public_hash == _this->mPublicHash)
		{
			if (_this->mPacket.cmd[XPLICIT_NETWORK_CMD_POS] == NETWORK_CMD_POS &&
				_this->mPacket.cmd[XPLICIT_NETWORK_CMD_ACCEPT] == NETWORK_CMD_ACCEPT)
			{
				const float delta = _this->mPacket.pos[XPLICIT_NETWORK_DELTA];

				const float xSpeed = _this->mPacket.pos[XPLICIT_NETWORK_X] * delta;
				const float zSpeed = _this->mPacket.pos[XPLICIT_NETWORK_Z] * delta;
				const float ySpeed = _this->mPacket.pos[XPLICIT_NETWORK_Y] * delta;

				_this->mPos.Z = zSpeed;
				_this->mPos.X = xSpeed;
				_this->mPos.Y = ySpeed;

				XPLICIT_INFO("World:LocalMove [EVENT]");

				String fmt = std::format("World:LocalMove({},{},{})", std::to_string(_this->mPos.X), std::to_string(_this->mPos.Y), std::to_string(_this->mPos.Z));

				Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
			}
		}

		if (KB->left_down())
		{
			_this->mPacket.cmd[XPLICIT_NETWORK_CMD_LCLICK] = NETWORK_CMD_LCLICK;
			_this->mNetwork->send(_this->mPacket);

			return;
		}

		if (KB->right_down())
		{
			_this->mPacket.cmd[XPLICIT_NETWORK_CMD_RCLICK] = NETWORK_CMD_RCLICK;
			_this->mNetwork->send(_this->mPacket);

			return;
		}

		if (_this->mPacket.health > 0 &&
			_this->mState == HUMANOID_STATE::DEAD)
		{
			XPLICIT_INFO("World:LocalSpawn [EVENT]");
			Lua::CLuaStateManager::get_singleton_ptr()->run_string("World:LocalSpawn()");

			_this->mState = HUMANOID_STATE::ALIVE;
		}
		else if (_this->mPacket.health <= 0)
		{
			_this->mState = HUMANOID_STATE::DEAD;
		}

		if (_this->count_parts() < 1)
			return;

		_this->node_at(XPLICIT_BUNDLE_HEAD)->setRotation(vector3df(
			_this->mCam->get()->getRotation().X,
			_this->mCam->get()->getRotation().Y > 0 ? _this->mCam->get()->getRotation().Y : 0,
			_this->mCam->get()->getRotation().Z));
	}

	void LocalHumanoidComponent::attach(LocalCameraComponent* cam) noexcept
	{ 
		if (cam)
			mCam = cam;
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
