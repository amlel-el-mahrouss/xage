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
#include <Targa.h>

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

			if (id)
			{
				std::vector<SceneID> ids;
				for (auto node : id->f_Nodes)
				{
					node->f_iSceneId = m_scene_counter;
					++m_scene_counter;

					ids.push_back(node->f_iSceneId);
				}

				return ids;
			}
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

			PString input = L"";
			bool xage_begin = true;

			PString working_dir = L"";

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
					if (const auto pos = input.find(L"#workingdir");
						pos != String::npos)
					{
						working_dir = input.substr(pos + strlen("#workingdir "));
					}

					if (const auto pos = input.find(L"#wavefront");
						pos != String::npos)
					{
						auto substr_wave = input.substr(pos, input.find(L","));
						substr_wave.erase(substr_wave.find(L"#wavefront"), strlen("#wavefront"));
						substr_wave.erase(substr_wave.find(L" "), strlen(" "));

						WaveFrontReader<wchar_t> wfReader;

						PString full_path = working_dir;
						full_path += substr_wave.c_str();

						if (!std::filesystem::exists(std::filesystem::path(full_path)))
							continue;

						HRESULT hr = wfReader.Load(full_path.c_str());
						DX11::Details::ThrowIfFailed(hr);

						auto render = scene->m_system->add<RenderableComponent>();
						
						for (auto& vert : wfReader.vertices)
						{
							render->push_vertice(Vector<float>(vert.position.x, vert.position.y, vert.position.z));
							render->push_normal(Vector<float>(vert.normal.x, vert.normal.y, vert.normal.z));
							render->push_texture_coord(Vector<float>(vert.textureCoordinate.x, vert.textureCoordinate.y, 0));
						}

						for (auto& indice : wfReader.indices)
						{
							render->push_indice(indice);
						}

						auto substr_wave_mtl = input.substr(input.find(L",") + 1);
						

						PString full_mtl_path = working_dir;
						full_mtl_path += substr_wave_mtl;

						hr = wfReader.LoadMTL(full_mtl_path.c_str());

						std::vector<ImageDataParams> params;

						if (SUCCEEDED(hr))
						{
							for (auto& mat : wfReader.materials)
							{
								if (mat.bSpecular)
								{
									render->push_ambient(Color<float32>
										(mat.vSpecular.x, 
										mat.vSpecular.y, 
										mat.vSpecular.z, 
										mat.fAlpha));
								}

								render->push_ambient(Color<float32>(mat.vAmbient.x, mat.vAmbient.y, mat.vAmbient.z, mat.fAlpha));
								render->push_diffuse(Color<float32>(mat.vDiffuse.x, mat.vDiffuse.y, mat.vDiffuse.z, mat.fAlpha));

								char* tex = nullptr;

								try
								{
									const PChar* paths[4] = { mat.strEmissiveTexture, mat.strNormalTexture , mat.strSpecularTexture, mat.strTexture };

									for (std::size_t index = 0UL; index < 4; ++index)
									{
										if (*paths[index] == 0)
											continue;

										tex = new char[260];
										memset(tex, 0, wcslen(paths[index]));

										if (!tex)
											throw EngineError("Out of memory. (TGA)");

										wcstombs(tex, paths[index], 260);

										char path_tga[255];
										memset(path_tga, 0, 255);

										auto sz = wcstombs(path_tga, working_dir.c_str(), working_dir.size());

										if (sz != working_dir.size())
											continue;

										String full_path_tga = path_tga;
										
										for (auto i = 0UL; i < strlen(tex); ++i)
										{
											full_path_tga += tex[i];
										}

										auto header = LoadTarga32(full_path_tga.c_str());

										if (header)
										{
											ImageDataParams params_image{};
											params_image.iHeight = header.f_sHeader.height;
											params_image.iWidth = header.f_sHeader.width;
											params_image.iStride = 4;
											params_image.pImage = header.f_pImage;

											params.push_back(params_image);
										}

										delete[] tex;
									}
								}
								catch (...)
								{
									if (tex)
									{
										fmt::print("Can't load non Targa file: {}", tex);
										delete[] tex;
									}
									else
									{
										fmt::print("Something else happened! We don't know though.");
									}
								}
							}
						}

						render->set_driver(scene->m_driver);
						render->make_mesh(params);

						pSceneData->f_Nodes.push_back(render);

						continue;
					}
				}
			}
			
			return std::shared_ptr<SceneLoaderXSD::SceneData>{ pSceneData };
		}

		delete pSceneData;
		return nullptr;
	}
}
