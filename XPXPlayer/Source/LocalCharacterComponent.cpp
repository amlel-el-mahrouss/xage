/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "LocalCharacterComponent.h"
#include "ChatBoxComponent.h"
#include "LocalMenuEvent.h"
#include "App.h"
#include "MenuUI.h"

#include <Enums.h>
#include <CLua.hpp>

namespace XPX
{
	constexpr const short XPLICIT_NETWORK_DELAY = 100;
	constexpr const short XPLICIT_PLAYER_COOLDOWN = 2;

	LocalCharacterComponent::LocalCharacterComponent(const int64_t& hash, const bool is_local_player, const char* optional_xid)
		:
		mHash(hash),
		mCam(nullptr), 
		mPacket(),
		mState(HUMANOID_STATE::ALIVE)
	{
		mNetwork = ComponentSystem::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");

		XPLICIT_ASSERT(mNetwork);

#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("LocalHumanoidComponent::LocalHumanoidComponent");
#endif
	}

	LocalCharacterComponent::~LocalCharacterComponent()
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("LocalHumanoidComponent::~LocalHumanoidComponent");
#endif
	}

	PHYSICS_TYPE LocalCharacterComponent::physics() noexcept { return PHYSICS_COMPLEX; }

	COMPONENT_TYPE LocalCharacterComponent::type() noexcept { return COMPONENT_HUMANOID; }

	const char* LocalCharacterComponent::name() noexcept { return ("LocalHumanoidComponent"); }

	void LocalCharacterComponent::update(void* class_ptr)
	{
		LocalCharacterComponent* self = (LocalCharacterComponent*)class_ptr;

		if (self == nullptr) return;
		if (!IsValidHeapPtr(self)) return;
		if (self->mNetwork == nullptr) return;

		if (!self->mNetwork->read(self->mPacket)) return;

		if ((self->mPacket.channel & XPLICIT_CHANNEL_DATA) &&
			self->mPacket.hash == self->mHash)
		{
			if (KEYBOARD->left_down())
			{
				self->mPacket.cmd[XPLICIT_NETWORK_CMD_LCLICK] = NETWORK_CMD_LCLICK;
				self->mNetwork->send(self->mPacket);
			}

			if (KEYBOARD->right_down())
			{
				self->mPacket.cmd[XPLICIT_NETWORK_CMD_RCLICK] = NETWORK_CMD_RCLICK;
				self->mNetwork->send(self->mPacket);
			}
		}
	}

	LocalHumanoidMoveEvent::LocalHumanoidMoveEvent(const std::int64_t& public_hash)
		: 
		mPacket(), 
		mNetwork(nullptr)
	{
		mNetwork = ComponentSystem::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent");
		XPLICIT_ASSERT(mNetwork);
	}

	LocalHumanoidMoveEvent::~LocalHumanoidMoveEvent() = default;

	const char* LocalHumanoidMoveEvent::name() noexcept { return ("LocalHumanoidMoveEvent"); }

	const int64_t& LocalCharacterComponent::id() noexcept { return mHash; }

	/* LocalPlayer movement logic */
	void LocalHumanoidMoveEvent::operator()()
	{
		if (mNetwork == nullptr)
			return;

		LocalMenuEvent* menu = EventSystem::get_singleton_ptr()->get<LocalMenuEvent>("LocalMenuEvent");
		ChatBoxComponent* chat_box = ComponentSystem::get_singleton_ptr()->get<ChatBoxComponent>("ChatBoxComponent");

		if (menu && menu->enabled() ||
			chat_box->is_typing())
			return;

		auto& traits = Bites::ObjectInputSystem::get_singleton_ptr()->Layout;

		if (KEYBOARD->key_down())
		{
			mPacket = mNetwork->get();

			mPacket.channel = XPLICIT_CHANNEL_DATA;

			if (KEYBOARD->key_down(traits.mForward))
			{
				mPacket.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;

				mPacket.cmd[XPLICIT_NETWORK_CMD_FORWARD] = NETWORK_CMD_FORWARD;

				mPacket.cmd[XPLICIT_NETWORK_CMD_LEFT] = NETWORK_CMD_INVALID;
				mPacket.cmd[XPLICIT_NETWORK_CMD_RIGHT] = NETWORK_CMD_INVALID;
				mPacket.cmd[XPLICIT_NETWORK_CMD_BACKWARD] = NETWORK_CMD_INVALID;

				mNetwork->send(mPacket);

				return;
			}

			if (KEYBOARD->key_down(traits.mBackward))
			{
				mPacket.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;

				mPacket.cmd[XPLICIT_NETWORK_CMD_FORWARD] = NETWORK_CMD_INVALID;

				mPacket.cmd[XPLICIT_NETWORK_CMD_LEFT] = NETWORK_CMD_INVALID;
				mPacket.cmd[XPLICIT_NETWORK_CMD_RIGHT] = NETWORK_CMD_INVALID;
				mPacket.cmd[XPLICIT_NETWORK_CMD_BACKWARD] = NETWORK_CMD_BACKWARD;

				mNetwork->send(mPacket);

				return;
			}

			if (KEYBOARD->key_down(traits.mRight))
			{
				mPacket.cmd[XPLICIT_NETWORK_CMD_BACKWARD] = NETWORK_CMD_INVALID;
				mPacket.cmd[XPLICIT_NETWORK_CMD_FORWARD] = NETWORK_CMD_INVALID;
				mPacket.cmd[XPLICIT_NETWORK_CMD_LEFT] = NETWORK_CMD_INVALID;

				mPacket.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;
				mPacket.cmd[XPLICIT_NETWORK_CMD_RIGHT] = NETWORK_CMD_RIGHT;

				mNetwork->send(mPacket);

				return;
			}

			if (KEYBOARD->key_down(traits.mLeft))
			{
				mPacket.cmd[XPLICIT_NETWORK_CMD_BACKWARD] = NETWORK_CMD_INVALID;
				mPacket.cmd[XPLICIT_NETWORK_CMD_FORWARD] = NETWORK_CMD_INVALID;
				mPacket.cmd[XPLICIT_NETWORK_CMD_RIGHT] = NETWORK_CMD_INVALID;

				mPacket.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;
				mPacket.cmd[XPLICIT_NETWORK_CMD_LEFT] = NETWORK_CMD_LEFT;

				mNetwork->send(mPacket);

				return;
			}

			if (KEYBOARD->key_down(traits.mJump))
			{
				mPacket.cmd[XPLICIT_NETWORK_CMD_BACKWARD] = NETWORK_CMD_INVALID;
				mPacket.cmd[XPLICIT_NETWORK_CMD_FORWARD] = NETWORK_CMD_INVALID;
				mPacket.cmd[XPLICIT_NETWORK_CMD_RIGHT] = NETWORK_CMD_INVALID;
				mPacket.cmd[XPLICIT_NETWORK_CMD_LEFT] = NETWORK_CMD_INVALID;

				mPacket.cmd[XPLICIT_NETWORK_CMD_POS] = NETWORK_CMD_POS;
				mPacket.cmd[XPLICIT_NETWORK_CMD_JUMP] = NETWORK_CMD_JUMP;

				mNetwork->send(mPacket);

				return;
			}

		}
	}
}
