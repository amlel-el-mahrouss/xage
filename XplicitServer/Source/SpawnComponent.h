/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright PlayXPlicit, all rights reserved.
 *
 *			File: SpawnComponent.h
 *			Purpose: Spawn Component
 *
 * =====================================================================
 */

#pragma once

#include "ServerConfig.h"

namespace Xplicit
{
	class SpawnComponent : public Component
	{
	public:
		SpawnComponent() = delete;

	public:
		explicit SpawnComponent(const Vector<float>& spawn);
		~SpawnComponent() override;

	public:
		SpawnComponent& operator=(const SpawnComponent&) = default;
		SpawnComponent(const SpawnComponent&) = default;

		Vector<float>& get() noexcept;

		COMPONENT_TYPE type() noexcept override;
		const char* name() noexcept override;

		bool should_update() noexcept { return true; }
		static void update(void* _this);

		XPXAttribute& get_attribute() noexcept;

	private:
		std::unique_ptr<Lua::CLuaClass> mClass;
		XPXAttribute mAttribute;

	};
}