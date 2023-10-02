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

	namespace Details
	{
		struct TargaHeader
		{
			unsigned char data1[12];
			unsigned short width;
			unsigned short height;
			unsigned char bpp;
			unsigned char data2;
		};

		bool LoadTarga32Bit(const char* filename)
		{
			int error, bpp, imageSize, index, i, j, k;
			FILE* filePtr;
			unsigned int count;
			TargaHeader targaFileHeader;
			unsigned char* targaImage;

			// Open the targa file for reading in binary.
			error = fopen_s(&filePtr, filename, "rb");
			if (error != 0)
			{
				return false;
			}

			// Read in the file header.
			count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
			if (count != 1)
			{
				return false;
			}

			// Check that it is 32 bit and not 24 bit.
			if (bpp != 32)
			{
				return false;
			}

			// Calculate the size of the 32 bit image data.
			imageSize = targaFileHeader.width * targaFileHeader.height * 4;

			// Allocate memory for the targa image data.
			targaImage = new unsigned char[imageSize];

			// Read in the targa image data.
			count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
			if (count != imageSize)
			{
				return false;
			}

			// Close the file.
			error = fclose(filePtr);
			if (error != 0)
			{
				return false;
			}

			// Allocate memory for the targa destination data.
			BYTE* targaData = new unsigned char[imageSize];

			// Initialize the index into the targa destination data array.
			index = 0;

			// Initialize the index into the targa image data.
			k = imageSize - (targaFileHeader.width * 4);

			// Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down and also is not in RGBA order.
			for (j = 0; j < targaFileHeader.height; j++)
			{
				for (i = 0; i < targaFileHeader.width; i++)
				{
					targaData[index + 0] = targaImage[k + 2];  // Red.
					targaData[index + 1] = targaImage[k + 1];  // Green.
					targaData[index + 2] = targaImage[k + 0];  // Blue
					targaData[index + 3] = targaImage[k + 3];  // Alpha

					// Increment the indexes into the targa data.
					k += 4;
					index += 4;
				}

				// Set the targa image data index back to the preceding row at the beginning of the column since its reading it in upside down.
				k -= (targaFileHeader.width * 8);
			}

			// Release the targa image data now that it was copied into the destination array.
			delete[] targaImage;
			targaImage = 0;

			return true;
		}
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
							render->push_vertice(Vector<float>(vert.position.x, vert.position.y, vert.position.z));
							render->push_normal(Vector<float>(vert.normal.x, vert.normal.y, vert.normal.z));
							render->push_texcoord(Vector<float>(vert.textureCoordinate.x, vert.textureCoordinate.y, 0));
						}

						for (auto& indice : wfReader.indices)
						{
							render->push_indice(indice);
						}

						auto substr_wave_mtl = input.substr(input.find(L",") + 1);
						
						hr = wfReader.LoadMTL(substr_wave_mtl.c_str());

						if (SUCCEEDED(hr))
						{
							for (auto& mat : wfReader.materials)
							{
								if (mat.bSpecular)
									render->push_specular(Color<float32>(mat.vSpecular.x, mat.vSpecular.y, mat.vSpecular.z, mat.fAlpha));

								render->push_ambient(Color<float32>(mat.vAmbient.x, mat.vAmbient.y, mat.vAmbient.z, mat.fAlpha));
								render->push_diffuse(Color<float32>(mat.vDiffuse.x, mat.vDiffuse.y, mat.vDiffuse.z, mat.fAlpha));
							}
						}

						render->set_driver(scene->m_driver);
						// render->make_mesh();

						pSceneData->f_Nodes.push_back(render);
					}
				}
			}
			
			return std::shared_ptr<SceneLoaderXSD::SceneData>{ pSceneData };
		}

		return nullptr;
	}
}
