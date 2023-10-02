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
#include <DirectXTex.h>

namespace XPX::Renderer
{
	SceneSystem::SceneSystem(const char* pName) noexcept
		: m_driver(RENDERER), 
		  m_system(std::make_unique<ComponentSystem>()), 
		  m_name(pName),
		  f_meshLoader(nullptr),
		  m_scene_counter(0)
	{
		
	}

	SceneSystem::~SceneSystem()
	{
		if (f_meshLoader)
			delete f_meshLoader;
	}

	bool SceneSystem::remove_scene_node(const SceneID& scene_id)
	{
		auto& all_ids = m_system->get_all();

		std::vector<Details::ComponentAccessor>::const_iterator it = std::find_if(all_ids.cbegin(), all_ids.cend(), [&](Details::ComponentAccessor comp) -> bool {
			return comp.as_type<RenderableComponent*>()->f_iSceneId == scene_id;
		});

		if (m_system->erase<RenderableComponent>(it))
		{
			--m_scene_counter;
			return true;
		}

		return false;
	}

	const std::vector<SceneID> SceneSystem::add_scene_node(const char* path)
	{
		if (f_meshLoader)
		{
			auto id = f_meshLoader->from_disk(path, this);

			std::vector<SceneID> ids;
			for (auto node : id->f_Nodes)
			{
				node->f_iSceneId = m_scene_counter;
				++m_scene_counter;

				ids.push_back(node->f_iSceneId);
			}

			return ids;
		}

		return {};
	}

	RenderableComponent* SceneSystem::get_scene_node(const SceneID& scene_id)
	{
		auto& all_ids = m_system->get_all();

		std::vector<Details::ComponentAccessor>::const_iterator it = std::find_if(all_ids.cbegin(), all_ids.cend(), [&](Details::ComponentAccessor comp) -> bool {
			return comp.as_type<RenderableComponent*>()->f_iSceneId == scene_id;
			});

		if (it != all_ids.cend())
			return it->as_type<RenderableComponent*>();

		return nullptr;
	}

	void SceneSystem::start_frame() noexcept
	{
		m_system->update();
	}

	bool SceneSystem::end_frame() noexcept
	{
		return true;
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
					if (input.find(L"begin") != String::npos)
						xage_begin = true;

					if (input.find(L"end") != String::npos)
						xage_begin = false;

					continue;
				}

				if (xage_begin)
				{
					if (const auto pos = input.find(L"#wavefront");
						pos != String::npos)
					{
						auto substr_wave = input.substr(pos, input.find(L","));
						substr_wave.erase(substr_wave.find(L"#wavefront"), strlen("#wavefront"));
						substr_wave.erase(substr_wave.find(L" "), strlen(" "));

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
						
						hr = wfReader.LoadMTL(substr_wave_mtl.c_str());

						if (SUCCEEDED(hr))
						{
							for (auto& mat : wfReader.materials)
							{
								render->push_ambient(Color<float32>(mat.vAmbient.x, mat.vAmbient.y, mat.vAmbient.z));
								render->push_diffuse(Color<float32>(mat.vDiffuse.x, mat.vDiffuse.y, mat.vDiffuse.z));
								render->push_specular(Color<float32>(mat.vSpecular.x, mat.vSpecular.y, mat.vSpecular.z));
							}
						}

						render->set_driver(scene->m_driver);
						render->make_mesh();

						pSceneData->f_Nodes.push_back(render);
					}
				}
			}
			
			return std::shared_ptr<SceneLoaderXSD::SceneData>{ pSceneData };
		}

		return nullptr;
	}
}
