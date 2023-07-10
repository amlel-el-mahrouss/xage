/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <Util.h>
#include <Component.h>
#include <CommonEngine.h>
#include <ClassComponent.h>

namespace Xplicit
{
	class GearComponent final : public ClassComponent
	{
	public:
		GearComponent() = delete;
		~GearComponent() override;

	public:
		XPLICIT_COPY_DEFAULT(GearComponent);

	public:
		const char* name() noexcept override;
		COMPONENT_TYPE type() noexcept override;
		PHYSICS_TYPE physics() noexcept override;
	
	public:
		//! Slots
		void click() noexcept;
		void activate() noexcept;
		void deactivate() noexcept;

	public:
		static bool should_update() noexcept;
		static void update(ClassPtr _self);

	public:
		explicit GearComponent(const char* name, const char* parent = "World") noexcept;

	private:
		String mParent;
		String mName;

	};
}