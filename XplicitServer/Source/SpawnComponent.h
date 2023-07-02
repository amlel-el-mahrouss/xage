/*
 * =====================================================================
 *
 *			XplicitServer
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
		explicit SpawnComponent(const Vector<float>& spawn);
		virtual ~SpawnComponent();

	public:
		SpawnComponent& operator=(const SpawnComponent&) = default;
		SpawnComponent(const SpawnComponent&) = default;

		Vector<float>& get() noexcept;

		virtual COMPONENT_TYPE type() noexcept override { return (COMPONENT_LOGIC); }
		virtual const char* name() noexcept override { return ("SpawnComponent"); }

		virtual bool should_update() noexcept override;
		virtual void update() override;

		XAttribute& get_attribute() noexcept;

	private:
		XAttribute mAttribute;

	};
}