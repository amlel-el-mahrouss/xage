/*
 * =====================================================================
 *
 *			XPXServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: SpawnComponent.h
 *			Purpose: Spawn point component.
 *
 * =====================================================================
 */

#pragma once

#include "ServerConfig.h"

namespace XPX
{
	class SpawnComponent : public Component
	{
	public:
		SpawnComponent() = delete;

	public:
		explicit SpawnComponent(const Vector<NetworkFloat>& spawn);
		~SpawnComponent() override;

	public:
		SpawnComponent& operator=(const SpawnComponent&) = default;
		SpawnComponent(const SpawnComponent&) = default;

		Vector<NetworkFloat>& get() noexcept;

		COMPONENT_TYPE type() noexcept override;
		const char* name() noexcept override;

		static bool should_update() noexcept { return false; }
		static void update(void* self);

	private:
		Vector<NetworkFloat> mPos;

	};
}