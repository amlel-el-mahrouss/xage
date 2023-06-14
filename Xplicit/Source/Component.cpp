/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: Component.h
 *			Purpose: Xplicit's Component System
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "Component.h"

namespace Xplicit 
{
	void ComponentManager::update()
	{
		for (size_t i = 0; i < m_instances.size(); i++)
		{
			XPLICIT_ASSERT(m_instances[i]);

			if (m_instances[i]->should_update())
				m_instances[i]->update();
		}
	}

	ComponentManager* ComponentManager::get_singleton_ptr()
	{
		static ComponentManager* ptr = nullptr;

		if (!ptr)
			ptr = new ComponentManager();

		return ptr;
	}

	void Component::update() {}

	bool Component::can_collide() noexcept { return false; }
	bool Component::has_physics() noexcept { return false; }

	bool Component::should_update() noexcept { return true; }

	const char* Component::name() noexcept { return ("Component"); }

	Component::PHYSICS_TYPE Component::physics() noexcept { return PHYSICS_SIMPLE; }
	Component::INSTANCE_TYPE Component::type() noexcept { return INSTANCE_LOGIC; }
}
