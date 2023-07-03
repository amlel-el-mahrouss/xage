/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

/* RoXML format */
/* This format describes scene-nodes and stuff like that. */

// XML
#include <rapidxml/rapidxml_utils.hpp>
#include <rapidxml/rapidxml.hpp>

// Engine
#include "DataValue.h"
#include "Root.h"
#include "Util.h"

// CLua
#include <lua/lua.hpp>

namespace Xplicit::RoXML
{
	using namespace rapidxml;

	struct RoXMLNodeDescription final
	{
		explicit RoXMLNodeDescription()
			: 
			Color(0.0f, 0.0f, 0.0f),
			Size(0.0f, 0.0f, 0.0f),
			Position(0.0f, 0.0f, 0.0f),
			Value(""),
			Name(""),
			ID("")
		{}

		String ID;
		String Name;
		String Value;
		Color<float> Color;
		Vector<float> Size;
		Vector<float> Position;

	};

	struct RoXMLDocumentParameters
	{
		String Path{ "" };

		bool Has3D{ false };
		bool LuaOnly{ false };
		bool NoLua{ false };

		std::vector<RoXMLNodeDescription> WorldNodes;

	};

	class RoXMLDocumentParser final
	{
	public:
		RoXMLDocumentParser() = default;
		~RoXMLDocumentParser() = default;

	public:
		XPLICIT_COPY_DEFAULT(RoXMLDocumentParser);

	public:
		void load(RoXMLDocumentParameters& params) noexcept
		{
			if (params.Path.empty() ||
				!std::filesystem::exists(params.Path))
				return;

			Thread stream_job([&]() {
				file<> xml_file(params.Path.c_str()); // Default template is char
				xml_document<> doc;

				doc.parse<0>(xml_file.data());

				xml_node<>* root_node = doc.first_node();

				while (root_node)
				{
					auto node = root_node;

					while (node)
					{
						String node_name = node->name();

						RoXMLNodeDescription world_node;

						if (node_name == "Stud")
						{
							if (node->first_attribute())
							{
								if (strncmp(node->first_attribute()->name(), "Id", 2) == 0)
								{
									auto node_id = node->first_attribute()->value();

									if (params.Has3D)
									{
										auto brick_mesh = RENDER->getSceneManager()->getGeometryCreator()->createCubeMesh();
										auto brick = RENDER->getSceneManager()->addMeshSceneNode(brick_mesh);

										brick->setName(node_id);
										brick->setPosition(vector3df(0.f, 0.f, 0.0f));

										brick->setMaterialTexture(0, RENDER->getVideoDriver()->getTexture("no_texture.png"));
									}

									world_node.Name = "Stud";
									world_node.ID = node_id;
								}
							}
						}

						if (node_name == "Ball")
						{
							if (node->first_attribute())
							{
								if (strncmp(node->first_attribute()->name(), "Id", 2) == 0)
								{
									auto node_id = node->first_attribute()->value();

									if (params.Has3D)
									{
										auto brick_mesh = RENDER->getSceneManager()->getGeometryCreator()->createSphereMesh();
										auto brick = RENDER->getSceneManager()->addMeshSceneNode(brick_mesh);

										brick->setName(node_id);
										brick->setPosition(vector3df(0.f, 0.f, 0.0f));

										brick->setMaterialTexture(0, RENDER->getVideoDriver()->getTexture("no_texture.png"));
									}

									world_node.Name = "Ball";
									world_node.ID = node_id;
								}
							}
						}

						if (node_name == "Color")
						{
							if (!node->first_attribute())
								break;

							String attr_who = node->first_attribute()->name();
							String attr_mat = node->first_attribute()->next_attribute()->name();

							if (attr_who == "Who" &&
								attr_mat == "Mat")
							{
								const auto mat_id = node->first_attribute()->next_attribute()->value();
								const auto mat_id_cast = std::atoi(node->value());

								world_node.Name = "Color";

								world_node.Color.R = mat_id_cast;
								world_node.Color.G = mat_id_cast << 8;
								world_node.Color.B = mat_id_cast << 16;
								world_node.Color.A = 0xFF;


								if (mat_id)
								{
									const auto scene_node = RENDER->getSceneManager()->getSceneNodeFromName(node->first_attribute()->value());

									if (scene_node)
									{
										try
										{
											scene_node->getMaterial(std::atoi(mat_id)).AmbientColor.set(mat_id_cast);
										}
										catch (...)
										{
											XPLICIT_INFO("Invalid Color requested, ignoring...");
										}
									}
								}

							}
						}

						if (node_name == "Position3")
						{
							String attr_x = node->first_attribute()->name();
							String attr_y = node->first_attribute()->next_attribute()->name();
							String attr_z = node->first_attribute()->next_attribute()->next_attribute()->name();

							if (attr_x == "X" &&
								attr_y == "Y" &&
								attr_z == "Z")
							{
								String id;

								for (std::size_t i = 0; i < strlen(node->value()); i++)
								{
									if (isalnum(node->value()[i]))
									{
										id += node->value()[i];
									}
								}

								world_node.Name = "Position3";

								String x = node->first_attribute()->value();
								String y = node->first_attribute()->next_attribute()->value();
								String z = node->first_attribute()->next_attribute()->next_attribute()->value();

								world_node.Position.X = std::atof(x.c_str());
								world_node.Position.Y = std::atof(y.c_str());
								world_node.Position.Z = std::atof(z.c_str());

								const auto scene_node = RENDER->getSceneManager()->getSceneNodeFromName(id.c_str());

								try
								{
									if (!scene_node)
										throw std::runtime_error("Invalid position requested, ignoring...");

									scene_node->setPosition(vector3df(std::atof(x.c_str()), std::atof(y.c_str()), std::atof(z.c_str())));
								}
								catch (const std::runtime_error& err)
								{
									XPLICIT_INFO(err.what());
								}
							}
						}

						if (node_name == "Size3")
						{
							String attr_x = node->first_attribute()->name();
							String attr_y = node->first_attribute()->next_attribute()->name();
							String attr_z = node->first_attribute()->next_attribute()->next_attribute()->name();

							if (attr_x == "X" &&
								attr_y == "Y" &&
								attr_z == "Z")
							{
								String id;

								for (std::size_t i = 0; i < strlen(node->value()); i++)
								{
									if (isalnum(node->value()[i]))
									{
										id += node->value()[i];
									}
								}

								world_node.Name = "Size3";

								String x = node->first_attribute()->value();
								String y = node->first_attribute()->next_attribute()->value();
								String z = node->first_attribute()->next_attribute()->next_attribute()->value();

								world_node.Size.X = std::atof(x.c_str());
								world_node.Size.Y = std::atof(y.c_str());
								world_node.Size.Z = std::atof(z.c_str());

								const auto scene_node = RENDER->getSceneManager()->getSceneNodeFromName(id.c_str());

								try
								{
									scene_node->setPosition(vector3df(std::atof(x.c_str()), std::atof(y.c_str()), std::atof(z.c_str())));
								}
								catch (...)
								{
									XPLICIT_INFO("Invalid Size requested, ignoring...");
								}
							}
						}

						// equivalent to Engine:Connect but in RoXML
						if (node_name == "Connect")
						{
							String attr = node->first_attribute()->name();

							if (attr == "Name")
							{
								if (node->first_attribute()->next_attribute())
								{
									String attr_id = node->first_attribute()->next_attribute()->name();
									String id = node->first_attribute()->next_attribute()->value();

									world_node.Name = "Connect";

									String name_value = node->first_attribute()->value();

									world_node.ID = name_value;

									String func_name = "fnXplicit";
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
									event_code += "\"";
									event_code += id;
									event_code += "\"";
									event_code += ",";
									event_code += func_name;
									event_code += ");";

									Lua::XLuaStateManager::get_singleton_ptr()->run_string(event_code.c_str());
								}
							}
						}

						if (node_name == "Instance" && node->first_attribute() && strcmp(node->first_attribute()->name(), "Name") == 0)
						{
							world_node.Name = node_name;
							world_node.ID = node->first_attribute()->value();
							world_node.Value = node->value();
						}

						params.WorldNodes.push_back(world_node);

						node = node->next_sibling();

						params.WorldNodes.push_back(world_node);
					}

					root_node = root_node->next_sibling();
				}
			});

			(!params.Has3D) ? stream_job.join() : stream_job.detach();
		}

	};
}