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

#include <WaveFrontReader.h>

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
			auto xsd = m_wrapper.open_reader<wchar_t>(path);

			PString input;
			bool xage_begin = true;

			while (std::getline(xsd, input))
			{
				if (input.find(L"#pragma") != String::npos)
				{
					if (input.find(L"begin"))
						xage_begin = true;

					if (input.find(L"end"))
						xage_begin = false;

					continue;
				}

				if (xage_begin)
				{
					if (const auto pos = input.find(L"#wavefront");
						pos != String::npos)
					{
						auto substr_wave = input.substr(pos + 1, input.find(L","));
						XPLICIT_INFO(fmt::format("Loading OBJ {}", substr_wave));

						WaveFrontReader<wchar_t> wfReader;

						HRESULT hr = wfReader.Load(substr_wave.c_str());
						DX11::Details::ThrowIfFailed(hr);

						auto render = scene->m_system->add<RenderableComponent>();
						
						for (auto& vert : wfReader.vertices)
						{
							render->push(Vector<float>(vert.position.x, vert.position.y, vert.position.z));
						}


						for (auto& indice : wfReader.indices)
						{
							render->push(indice);
						}

						auto substr_wave_mtl = input.substr(input.find(L","));
						XPLICIT_INFO(fmt::format("Loading MTL {}", substr_wave_mtl));


						hr = wfReader.LoadMTL(substr_wave_mtl.c_str());

						if (SUCCEEDED(hr))
						{
							for (auto& mat : wfReader.materials)
							{
								render->push_ambient(Color<float32>(mat.vAmbient.x, mat.vAmbient.y, mat.vAmbient.z));
								render->push_diffuse(Color<float32>(mat.vDiffuse.x, mat.vDiffuse.y, mat.vDiffuse.z));
								render->push_specular(Color<float32>(mat.vSpecular.x, mat.vSpecular.y, mat.vSpecular.z));
								render->push_normal(Color<float32>(0.5, 0.5, 0.5));
							}
						}

						render->set_driver(scene->m_driver);
						render->make_mesh();
					}
				}
			}
			
			return std::shared_ptr<SceneLoaderXSD::SceneData>{ pSceneData };
		}

		return nullptr;
	}
}
