/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: SpawnComponent.h
 *			Purpose: Actor spawn component
 *
 * =====================================================================
 */

#pragma once

#include "SDK.h"

namespace Xplicit
{
	class SpawnComponent : public Component
	{
	public:
		SpawnComponent() = delete;

	public:
		SpawnComponent(Nplicit::Quaternion<float>& spawn);

		virtual ~SpawnComponent();

		SpawnComponent& operator=(const SpawnComponent&) = default;
		SpawnComponent(const SpawnComponent&) = default;

		Nplicit::Quaternion<float>& get() noexcept;

		virtual INSTANCE_TYPE type() noexcept override { return (INSTANCE_LOGIC); }
		virtual const char* name() noexcept override { return ("SpawnComponent"); }

		virtual bool should_update() noexcept override;
		virtual void update() override;

	private:
		Nplicit::Quaternion<float> m_origin;

	};
}