/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "Component.h"

namespace Xplicit
{
	void ComponentSystem::update() noexcept
	{
		if (mComponents.empty())
			return;

		const std::size_t sz = mComponents.size();

		for (std::size_t i = 0; i < sz; i++)
		{
			XPLICIT_ASSERT(mComponents[i]._Pointee);

			if (mComponents[i]._Eval())
				mComponents[i]._Update(mComponents[i]._Pointee);
		}
	}

	ComponentSystem* ComponentSystem::get_singleton_ptr() noexcept
	{
		static ComponentSystem* ptr = nullptr;

		if (!ptr)
			ptr = new ComponentSystem();

		return ptr;
	}

	Component::Component() = default;

	Component::~Component() = default;

	bool Component::can_collide() noexcept { return false; }
	bool Component::has_physics() noexcept { return false; }

	const char* Component::name() noexcept { return ("Component"); }

	PHYSICS_TYPE Component::physics() noexcept { return PHYSICS_SIMPLE; }

	COMPONENT_TYPE Component::type() noexcept { return COMPONENT_LOGIC; }
}