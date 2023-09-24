/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "SpawnComponent.h"
#include "HumanoidComponent.h"

#include <CLua.hpp>

namespace XPX
{
	SpawnComponent::SpawnComponent(const Vector<NetworkFloat>& vec)
		: mPos(vec)
	{

	}

	SpawnComponent::~SpawnComponent() = default;

	Vector<NetworkFloat>& SpawnComponent::get() noexcept { return mPos; }

	void SpawnComponent::update(ClassPtr class_ptr) { }

	COMPONENT_TYPE SpawnComponent::type() noexcept { return (COMPONENT_LOGIC); }
	const char* SpawnComponent::name() noexcept { return ("SpawnComponent"); }
}