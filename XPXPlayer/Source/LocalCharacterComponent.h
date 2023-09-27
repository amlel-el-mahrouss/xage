/*
 * =====================================================================
 *
 *			XPXPlayer
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: LocalHumanoidComponent.h
 *			Purpose: Client-side Player
 *
 * =====================================================================
 */

#pragma once

#include "LocalCameraComponent.h"

#include <NetworkComponent.h>
#include <XplicitID.h>
#include <GameVar.h>

namespace XPX
{
	enum class HUMANOID_STATE
	{
		ALIVE,
		DEAD,
		INVALID,
	};

	class LocalHumanoidMoveEvent;

	//! Replicated player component
	/** LocalHumanoidComponent is the player's logic */
	class LocalCharacterComponent final : public Component
	{
	public:
		LocalCharacterComponent(const int64_t& hash, const bool is_local_player = false, const char* optional_xplicit_id = XPLICIT_ID_DUMMY);
		~LocalCharacterComponent() override;

		LocalCharacterComponent& operator=(const LocalCharacterComponent&) = default;
		LocalCharacterComponent(const LocalCharacterComponent&) = default;

		COMPONENT_TYPE type() noexcept override;
		const char* name() noexcept override;
		PHYSICS_TYPE physics() noexcept override;

	public:
		static void update(void* class_ptr);
		static bool should_update() noexcept { return true; }

		const int64_t& id() noexcept;

	private:
		NetworkComponent* mNetwork;
		LocalCameraComponent* mCam;

	private:
		std::int64_t mHash;
		HUMANOID_STATE mState;
		NetworkPacket mPacket;

	};

	class LocalHumanoidMoveEvent : public Event
	{
	public:
		LocalHumanoidMoveEvent() = delete;

	public:
		explicit LocalHumanoidMoveEvent(const std::int64_t& public_hash);
		virtual ~LocalHumanoidMoveEvent();

	public:
		LocalHumanoidMoveEvent& operator=(const LocalHumanoidMoveEvent&) = default;
		LocalHumanoidMoveEvent(const LocalHumanoidMoveEvent&) = default;

	public:
		virtual void operator()() override;
		const char* name() noexcept;

	private:
		NetworkComponent* mNetwork;
		NetworkPacket mPacket;

	};
}
