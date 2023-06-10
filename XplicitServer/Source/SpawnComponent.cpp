/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: SpawnComponent.cpp
 *			Purpose: Actor spawn component
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "SpawnComponent.h"
#include "PlayerComponent.h"

namespace Xplicit
{
	SpawnComponent::SpawnComponent(const Quaternion<float>& vec) : Component(), m_origin(vec) {}
	SpawnComponent::~SpawnComponent() = default;

	Quaternion<float>& SpawnComponent::get() noexcept
	{
		return m_origin;
	}

	bool SpawnComponent::should_update() noexcept { return false; }

	void SpawnComponent::update() {}
}