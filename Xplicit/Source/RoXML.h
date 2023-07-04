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
			Rotation(0.0f, 0.0f, 0.0f, 0.0f),
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
		Quaternion<float> Rotation;

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

						if (node_name == "Mesh" &&
							!params.LuaOnly &&
							params.Has3D)
						{
							if (node->first_attribute())
							{
								if (strncmp(node->first_attribute()->name(), "Name", 2) == 0)
								{
									auto node_id = node->first_attribute()->value();
									auto brick_mesh = RENDER->_getCurrentSceneManager()->createEntity(node_id);

									world_node.Name = "Mesh";
									world_node.ID = node_id;
								}
							}
						}

						if (node_name == "Color3")
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

								world_node.Name = "Color3";

								world_node.Color.R = mat_id_cast;
								world_node.Color.G = mat_id_cast << 8;
								world_node.Color.B = mat_id_cast << 16;
								world_node.Color.A = mat_id_cast << 22;


								if (mat_id)
								{
									const auto scene_node = RENDER->_getCurrentSceneManager()->getEntity(node->first_attribute()->value());

									if (scene_node)
										scene_node->getSubEntity(0)->getMaterial()->setDiffuse(world_node.Color.R, world_node.Color.G, world_node.Color.B, world_node.Color.A);
								}

							}
						}

						if (node_name == "Rotate3")
						{
							if (!node->first_attribute() ||
								!node->first_attribute()->next_attribute() ||
								!node->first_attribute()->next_attribute()->next_attribute() ||
								!node->first_attribute()->next_attribute()->next_attribute()->next_attribute())
							{
								XPLICIT_CRITICAL("RoXML: Bad Rotate3!");
								break;
							}

							String attr_x = node->first_attribute()->name();
							String attr_y = node->first_attribute()->next_attribute()->name();
							String attr_z = node->first_attribute()->next_attribute()->next_attribute()->name();
							String attr_w = node->first_attribute()->next_attribute()->next_attribute()->next_attribute()->name();

							if (attr_x == "X" &&
								attr_y == "Y" &&
								attr_z == "Z" &&
								attr_w == "W")
							{
								String id;

								for (std::size_t i = 0; i < strlen(node->value()); i++)
								{
									if (isalnum(node->value()[i]))
									{
										id += node->value()[i];
									}
								}

								world_node.Name = "Rotate3";

								String x = node->first_attribute()->value();
								String y = node->first_attribute()->next_attribute()->value();
								String z = node->first_attribute()->next_attribute()->next_attribute()->value();
								String w = node->first_attribute()->next_attribute()->next_attribute()->next_attribute()->value();

								world_node.Rotation.X = std::atof(x.c_str());
								world_node.Rotation.Y = std::atof(y.c_str());
								world_node.Rotation.Z = std::atof(z.c_str());
								world_node.Rotation.W = std::atof(w.c_str());

								const auto scene_node = RENDER->_getCurrentSceneManager()->getEntity(id);

								if (scene_node)
									scene_node->getParentSceneNode()->rotate(Ogre::Quaternion(std::atof(x.c_str()), std::atof(y.c_str()), std::atof(z.c_str()), std::atof(w.c_str())));
							}
						}

						if (node_name == "Position3")
						{
							if (!node->first_attribute() ||
								!node->first_attribute()->next_attribute() ||
								!node->first_attribute()->next_attribute()->next_attribute())
							{
								XPLICIT_CRITICAL("RoXML: Bad Position3!");
								break;
							}

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

								const auto scene_node = RENDER->_getCurrentSceneManager()->getEntity(id);

								if (scene_node)
									scene_node->getParentSceneNode()->setPosition(Ogre::Vector3f(std::atof(x.c_str()), std::atof(y.c_str()), std::atof(z.c_str())));
							}
						}

						if (node_name == "Scale3")
						{
							if (!node->first_attribute() ||
								!node->first_attribute()->next_attribute() ||
								!node->first_attribute()->next_attribute()->next_attribute())
							{
								XPLICIT_CRITICAL("RoXML: Bad Scale3!");
								break;
							}

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

								world_node.Name = "Scale3";

								String x = node->first_attribute()->value();
								String y = node->first_attribute()->next_attribute()->value();
								String z = node->first_attribute()->next_attribute()->next_attribute()->value();

								world_node.Size.X = std::atof(x.c_str());
								world_node.Size.Y = std::atof(y.c_str());
								world_node.Size.Z = std::atof(z.c_str());

								const auto scene_node = RENDER->_getCurrentSceneManager()->getEntity(id.c_str());

								try
								{
									scene_node->getParentSceneNode()->setScale(Ogre::Vector3f(std::atof(x.c_str()), std::atof(y.c_str()), std::atof(z.c_str())));
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

						if (!params.NoLua)
						{
							// if it is a CLua snippet
							if (node_name == "CLua" &&
								node->first_attribute() && // and if it has a name attribute
								strcmp(node->first_attribute()->name(), "Name") == 0) // and it is really what we think it is.
							{
								// go on and include that!
								world_node.Name = node_name;
								world_node.ID = node->first_attribute()->value();
								world_node.Value = node->value();
							}
						}

						params.WorldNodes.push_back(world_node);

						node = node->next_sibling();
					}

					root_node = root_node->next_sibling();
				}
			});

			(!params.Has3D) ? stream_job.join() : stream_job.detach();
		}

	};
}