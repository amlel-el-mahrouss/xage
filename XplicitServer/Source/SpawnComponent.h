/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: SpawnComponent.h
 *			Purpose: Spawn Component
 *
 * =====================================================================
 */

#pragma once

#include "CommonInclude.h"

namespace Xplicit
{
	class SpawnComponent : public Component
	{
	public:
		SpawnComponent() = delete;

	public:
		SpawnComponent(const Quaternion<float>& spawn);

		virtual ~SpawnComponent();

		SpawnComponent& operator=(const SpawnComponent&) = default;
		SpawnComponent(const SpawnComponent&) = default;

		Quaternion<float>& get() noexcept;

		virtual COMPONENT_TYPE type() noexcept override { return (COMPONENT_LOGIC); }
		virtual const char* name() noexcept override { return ("SpawnComponent"); }

		virtual bool should_update() noexcept override;
		virtual void update() override;

	private:
		Quaternion<float> mOrigin;

	};
}