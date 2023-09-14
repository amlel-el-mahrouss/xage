/*
 * =====================================================================
 *
 *			XPXServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: SpawnComponent.h
 *			Purpose: Spawn Component
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

		static bool should_update() noexcept { return true; }
		static void update(void* self);

		XPXAttribute& get_attribute() noexcept;

	private:
		std::unique_ptr<Lua::CLuaClass> mClass;
		XPXAttribute mAttribute;

	};
}