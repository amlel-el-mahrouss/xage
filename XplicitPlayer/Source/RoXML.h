/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

/* RoXML format */

// external
#include <rapidxml/rapidxml_utils.hpp>
#include <rapidxml/rapidxml.hpp>
#include <DataValue.h>
#include <lua/lua.hpp>
#include <Root.h>
#include <Util.h>

// internal
#include "RegisteredEventsList.h"

namespace Xplicit::SceneManager
{
	constexpr int STUD_WIDTH = 4;
	constexpr int STUD_HEIGHT = 1;
	constexpr int STUD_TALL = 2;

	using namespace rapidxml;

	class RoXMLDocumentParser final
	{
	public:
		RoXMLDocumentParser() = default;
		~RoXMLDocumentParser() = default;

	public:
		XPLICIT_COPY_DEFAULT(RoXMLDocumentParser);

	public:
		void load_scene(String& path) noexcept
		{
			if (path.empty() ||
				!std::filesystem::exists(path))
				return;

			Thread stream_job([](String _path) {
				file<> xml_file(_path.c_str()); // Default template is char
				xml_document<> doc;
				
				doc.parse<0>(xml_file.data());

				xml_node<>* root_node = doc.first_node();

				while (root_node)
				{
					xml_node<>* node = root_node->first_node();

					String node_name = node->name();

					if (node_name == "Stud")
					{
						if (strncmp(node->first_attribute()->name(), "Id", 2) == 0)
						{
							auto node_id = node->first_attribute()->value();

							auto brick_mesh = RENDER->getSceneManager()->getGeometryCreator()->createCubeMesh(vector3df(STUD_WIDTH, STUD_HEIGHT, STUD_TALL));
							auto brick = RENDER->getSceneManager()->addMeshSceneNode(brick_mesh);

							brick->setName(node_id);
							brick->setPosition(vector3df(0.f, 0.f, 0.0f));

							brick->setMaterialTexture(0, RENDER->getVideoDriver()->getTexture("no_texture.png"));
						}
					}

					while (node)
					{
						node_name = node->name();
						std::cout << node->name() << std::endl;

						if (node_name == "Color")
						{
							String attr_who = node->first_attribute()->name();
							String attr_mat = node->first_attribute()->next_attribute()->name();

							if (attr_who == "Who" &&
								attr_mat == "Mat")
							{
								const auto mat_id = node->first_attribute()->next_attribute()->value();

								if (mat_id)
								{
									const auto scene_node = RENDER->getSceneManager()->getSceneNodeFromName(node->first_attribute()->value());

									if (scene_node)
									{
										try
										{
											scene_node->getMaterial(std::atoi(mat_id)).AmbientColor.set(std::atoi(node->value()));
										}
										catch (...)
										{
											XPLICIT_INFO("Invalid Color requested, ignoring...");
										}
									}
								}

							}
						}

						if (node_name == "Vector3")
						{
							String attr_x = node->first_attribute()->name();
							String attr_y = node->first_attribute()->next_attribute()->name();
							String attr_z = node->first_attribute()->next_attribute()->next_attribute()->name();

							if (attr_x == "X" &&
								attr_y == "Y" &&
								attr_z == "Z")
							{
								String id = node->value();

								String x = node->first_attribute()->value();
								String y = node->first_attribute()->next_attribute()->value();
								String z = node->first_attribute()->next_attribute()->next_attribute()->value();

								const auto scene_node = RENDER->getSceneManager()->getSceneNodeFromName(id.c_str());

								try
								{
									scene_node->setPosition(vector3df(std::atof(x.c_str()), std::atof(y.c_str()), std::atof(z.c_str())));
								}
								catch (...)
								{
									XPLICIT_INFO("Invalid Position requested, ignoring...");
								}
							}
						}

						if (node_name == "Event")
						{
							String attr = node->first_attribute()->name();

							if (attr == "Name")
							{
								String name_value = node->first_attribute()->value();

								for (std::size_t event_idx = 0; event_idx < XPLICIT_EVENT_MAX; ++event_idx)
								{
									if (strncmp(name_value.c_str(), XPLICIT_EVENTS[event_idx], strlen(XPLICIT_EVENTS[event_idx])))
									{
										String func_name = "methodXplicit";
										func_name += std::to_string(xplicit_get_epoch());

										String event_code = "local func ";
										event_code += func_name;
										event_code += "()\n";

										event_code += node->value();

										event_code += "\nend\n";
										event_code += "Engine:Connect(";
										event_code += "\"";
										event_code += name_value;
										event_code += "\"";
										event_code += ",";
										event_code += func_name;
										event_code += ");";

										Lua::XLuaStateManager::get_singleton_ptr()->run_string(event_code.c_str());

										break;
									}
								}
							}
						}

						node = node->next_sibling();
					}

					root_node = root_node->next_sibling();
				}
			}, path);

			stream_job.detach();
		}

	};
}