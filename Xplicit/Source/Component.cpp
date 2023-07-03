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
	void ComponentManager::update() noexcept
	{
		if (mComponents.empty())
			return;

		const std::size_t sz = mComponents.size();

		for (std::size_t i = 0; i < sz; i++)
		{
#ifdef XPLICIT_DEBUG
			XPLICIT_ASSERT(mComponents[i]);
#endif // ifdef XPLICIT_DEBUG

			if (mComponents[i]->should_update())
				mComponents[i]->update();
		}
	}

	ComponentManager* ComponentManager::get_singleton_ptr() noexcept
	{
		static ComponentManager* ptr = nullptr;

		if (!ptr)
			ptr = new ComponentManager();

		return ptr;
	}

	Component::Component() = default;
	Component::~Component() = default;

	void Component::update() {}

	bool Component::can_collide() noexcept { return false; }
	bool Component::has_physics() noexcept { return false; }

	bool Component::should_update() noexcept { return true; }

	const char* Component::name() noexcept { return ("Component"); }

	PHYSICS_TYPE Component::physics() noexcept { return PHYSICS_SIMPLE; }

	COMPONENT_TYPE Component::type() noexcept { return COMPONENT_LOGIC; }
}
