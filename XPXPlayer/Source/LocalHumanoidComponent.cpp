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

#include "LocalHumanoidComponent.h"
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

	LocalHumanoidComponent::LocalHumanoidComponent(const int64_t& hash, const bool is_local_player, const char* optional_xid)
		:
		mHash(hash),
		mCam(nullptr), 
		mPacket(),
		mPos(0.f, 0.f, 0.f),
		mState(HUMANOID_STATE::ALIVE)
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
		LocalHumanoidComponent* self = (LocalHumanoidComponent*)class_ptr;

		if (self == nullptr) return;
		if (!IsValidHeapPtr(self)) return;
		if (self->mNetwork == nullptr) return;

		if (!self->mNetwork->read(self->mPacket)) return;

		if ((self->mPacket.channel & XPLICIT_CHANNEL_DATA) &&
			self->mPacket.hash == self->mHash)
		{
			static auto node = CAD->getSceneManager()->getSceneNodeFromName("Camera");

			if (!node)
				node = CAD->getSceneManager()->getSceneNodeFromName("Camera");

			if (node)
			{
				matrix4 pos_translate;

				auto translation = vector3df(self->mPacket.pos[XPLICIT_NETWORK_X],
					self->mPacket.pos[XPLICIT_NETWORK_Y],
					self->mPacket.pos[XPLICIT_NETWORK_Z]);

				auto& pos = const_cast<vector3df&>(node->getPosition());

				pos_translate.transformVect(translation, pos);

				translation = vector3df(self->mPacket.pos_second[XPLICIT_NETWORK_X],
					self->mPacket.pos_second[XPLICIT_NETWORK_Y],
					self->mPacket.pos_second[XPLICIT_NETWORK_Z]);

				pos = const_cast<vector3df&>(node->getRotation());

				pos_translate.transformVect(translation, pos);
			}

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

	Vector<float> LocalHumanoidComponent::get_pos() noexcept { return mPos; }

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

	const int64_t& LocalHumanoidComponent::id() noexcept { return mHash; }

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
