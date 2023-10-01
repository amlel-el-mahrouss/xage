/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: SceneDriver.cpp
 *			Purpose: Abstraction behind the rendering driver.
 *
 * =====================================================================
 */

#include "SceneDriver.h"

namespace XPX::Renderer
{
	SceneDriver::SceneDriver() noexcept
		: m_driver(RENDERER), m_system(std::make_unique<ComponentSystem>())
	{
		
	}

	SceneDriver::~SceneDriver()
	{
		if (f_textureLoader)
			delete f_textureLoader;

		if (f_meshLoader)
			delete f_meshLoader;
	}

	const SceneID SceneDriver::add_texture(const char* path)
	{
		return XPX_INVALID_SCENE_ID;
	}

	const SceneID SceneDriver::add_mesh(const char* path)
	{
		return XPX_INVALID_SCENE_ID;
	}

	RenderableComponent* SceneDriver::get_scene_node(const SceneID& sceneId)
	{
		return nullptr;
	}

	void SceneDriver::start_frame() noexcept
	{
		m_driver->begin_scene(1, 0, 0, 0, true, true);

		m_system->update();
	}

	bool SceneDriver::end_frame() noexcept
	{
		return m_driver->end_scene();
	}
}
