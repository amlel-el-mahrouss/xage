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
#include "Application.h"
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
		mState(HUMANOID_STATE::ALIVE),
		mIsLocalPlayer(is_local_player),
		mClass(nullptr),
		mCharacter(nullptr)
	{
		String path = "Contents/Humanoid.dae";

		if (is_local_player)
		{
			mClass = new Lua::CLuaClass("world.Players.LocalPlayer");
			mCharacter = XPX::ComponentSystem::get_singleton_ptr()->add<MeshComponent>(path.c_str(), "Players.LocalPlayer.RootPart", "world");
		}
		else
		{
			// make sure that the server passes an xplicitid.
			XPLICIT_ASSERT(optional_xid);

			String path_xid = "Players.";
			path_xid += optional_xid;

			// ugly ass hack, as always.
			Lua::CLuaStateManager::get_singleton_ptr()->run_string("world." + path_xid + " = {}");

			path_xid += ".RootPart";

			mCharacter = XPX::ComponentSystem::get_singleton_ptr()->add<MeshComponent>(path.c_str(), path_xid.c_str(), "world");
		}

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

		if (self->mNetwork == nullptr) return;
		if (!self->mNetwork->read(self->mPacket)) return;

		if (self->mPacket.hash == self->mHash)
		{
			if (self->mPacket.cmd[XPLICIT_NETWORK_CMD_POS] == NETWORK_CMD_POS &&
				self->mPacket.cmd[XPLICIT_NETWORK_CMD_ACCEPT] == NETWORK_CMD_ACCEPT)
			{
				const NetworkFloat delta = self->mPacket.pos[XPLICIT_NETWORK_DELTA];

				const NetworkFloat xSpeed = self->mPacket.pos[XPLICIT_NETWORK_X];
				const NetworkFloat zSpeed = self->mPacket.pos[XPLICIT_NETWORK_Z];
				const NetworkFloat ySpeed = self->mPacket.pos[XPLICIT_NETWORK_Y];

				self->mPos.Z += zSpeed * delta;
				self->mPos.X += xSpeed * delta;
				self->mPos.Y += ySpeed * delta;

				self->mCharacter->node()->setPosition(vector3df(self->mPos.X, self->mPos.Y, self->mPos.Z));

				self->mCharacter->node()->setRotation(
					vector3df(self->mCam->get()->getPosition().X, 
					0, 
					self->mCam->get()->getPosition().Z));

				XPLICIT_INFO("world:LocalMove [EVENT]");

				String fmt = fmt::format("world:LocalMove({},{},{})", std::to_string(self->mPos.X), std::to_string(self->mPos.Y), std::to_string(self->mPos.Z));

				Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());
			}

			if (self->mIsLocalPlayer)
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

				if (self->mPacket.health > 0 &&
					self->mState == HUMANOID_STATE::DEAD)
				{
					XPLICIT_INFO("world:LocalSpawn [EVENT]");

					Lua::CLuaStateManager::get_singleton_ptr()->run_string("world:LocalSpawn()");

					self->mState = HUMANOID_STATE::ALIVE;
				}
				else if (self->mPacket.health <= 0)
				{
					self->mState = HUMANOID_STATE::DEAD;
				}
			}
		}
	}

	void LocalHumanoidComponent::attach(LocalCameraComponent* cam) noexcept
	{ 
		if (cam)
		{
			mCam = cam;
			mCam->get()->setTarget(mCharacter->node()->getPosition());
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

			auto prev_channel = mPacket.channel;

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

			mPacket.channel = prev_channel;
			return;
		}
	}
}
