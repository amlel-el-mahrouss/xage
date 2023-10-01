/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: SceneSystem.cpp
 *			Purpose: Rendering scene system.
 *
 * =====================================================================
 */

#include "SceneSystem.h"

namespace XPX::Renderer
{
	SceneSystem::SceneSystem(const char* pName) noexcept
		: m_driver(RENDERER), 
		  m_system(std::make_unique<ComponentSystem>()), 
		  m_name(pName),
		  f_meshLoader(nullptr)
	{
		
	}

	SceneSystem::~SceneSystem()
	{
		if (f_meshLoader)
			delete f_meshLoader;
	}

	bool SceneSystem::remove_scene_node(const SceneID& scene_id)
	{
		(void)scene_id;

		auto& all_ids = m_system->get_all();

		std::vector<Details::ComponentAccessor>::const_iterator it = std::find_if(all_ids.cbegin(), all_ids.cend(), [&](Details::ComponentAccessor comp) -> bool {
			return comp.as_type<RenderableComponent*>()->f_iSceneId == scene_id;
		});

		return m_system->erase<RenderableComponent>(it);
	}

	const SceneID SceneSystem::add_scene_node(const char* path)
	{
		return 0;
	}

	RenderableComponent* SceneSystem::get_scene_node(const SceneID& sceneId)
	{
		return nullptr;
	}

	void SceneSystem::start_frame() noexcept
	{
		m_driver->begin_scene(1, 0, 0, 0, true, true);

		m_system->update();
	}

	bool SceneSystem::end_frame() noexcept
	{
		return m_driver->end_scene();
	}

	std::shared_ptr<SceneLoaderXSD::SceneData> SceneLoaderXSD::from_disk(const char* path, SceneSystem* scene)
	{
		SceneLoaderXSD::SceneData* pSceneData = new SceneLoaderXSD::SceneData();
		pSceneData->f_Parent = scene;

		if (m_wrapper.exists(path))
		{
			auto scene = m_wrapper.open_reader(path);
			
			return std::shared_ptr<SceneLoaderXSD::SceneData>{ pSceneData };
		}

		return nullptr;
	}
}
