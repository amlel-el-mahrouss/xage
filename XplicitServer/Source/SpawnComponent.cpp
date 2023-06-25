/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "SpawnComponent.h"
#include "HumanoidComponent.h"

namespace Xplicit
{
	SpawnComponent::SpawnComponent(const Quaternion<float>& vec) : Component(), mOrigin(vec) {}
	SpawnComponent::~SpawnComponent() = default;

	Quaternion<float>& SpawnComponent::get() noexcept { return mOrigin; }

	bool SpawnComponent::should_update() noexcept { return false; }

	void SpawnComponent::update() {}
}