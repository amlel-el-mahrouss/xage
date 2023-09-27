/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "RXML.h"
#include <NpPhysicsEngine.h>

namespace XPX::RXML
{
	void RXMLDocument::parse(RXMLDocumentParams& params) noexcept
	{
		if (!params.Inline)
		{
			if (params.Path.empty() ||
				!std::filesystem::exists(params.Path))
				return;
		}

		Thread stream_job([&]() {
			std::unique_ptr<xml_document<>> doc = std::make_unique<xml_document<>>();

			// open xml file
			file<char> xml_file(params.Path.c_str());

			// check inline
			if (!params.Inline)
				doc->parse<0>(xml_file.data());
			else
				doc->parse<0>(params.Path.data());

			// get first node
			xml_node<>* root_node = doc->first_node();
			xml_node<>* node = root_node->first_node();

			while (node)
			{
				const String node_name = node->name();
				RoXMLNodeDescription world_node;

				if (node_name == "Item")
				{
					if (node->first_attribute())
					{
						auto first_attr = node->first_attribute();
						String klass_to_instantiate = first_attr->name();

						if (first_attr &&
							klass_to_instantiate == "Class")
							klass_to_instantiate = first_attr->value();

						if (strcmp(first_attr->next_attribute()->name(), "Referent") == 0)
						{
							auto node_id = first_attr->next_attribute()->value();

							world_node.Name = node_name;
							world_node.ID = node_id;

							void* object = nullptr;

							if (klass_to_instantiate == "Mesh")
							{
								auto val = node->value();

								for (size_t i = 0; i < strlen(val); i++)
								{
									if (isalnum(val[i]) ||
										val[i] == '.' ||
										val[i] == '/' ||
										val[i] == '\\' ||
										val[i] == '-' ||
										val[i] == ':' ||
										val[i] == '_')
									{
										world_node.Value += val[i];
									}
								}
								
								auto node_mesh = RENDERER->getSceneManager()->addMeshSceneNode(RENDERER->getSceneManager()->getMesh(world_node.Value.c_str()));

								if (node_mesh)
									object = node_mesh;
							}
						}
					}
				}

				if (node_name == "Color3")
				{
					if (node->first_attribute())
					{
						String attr_who = node->first_attribute()->name();
						String attr_mat = node->first_attribute()->next_attribute()->name();

						if (attr_who == "Referent" &&
							attr_mat == "Material")
						{
							const auto mat_id = node->first_attribute()->next_attribute()->value();
							const UInt32 mat_id_cast = std::atoi(node->value());

							if (params.Has3D)
							{
								if (mat_id)
								{
									try
									{
										const auto scene_node = RENDERER->getSceneManager()->getSceneNodeFromName(node->first_attribute()->value());

										if (scene_node)
										{
											scene_node->getMaterial(0).AmbientColor.color = mat_id_cast;
											scene_node->getMaterial(0).DiffuseColor.color = mat_id_cast;
										}
									}
									catch (...)
									{
										XPLICIT_INFO("No such entity had been found!");
									}
								}
							}
						}
					}
				}

				if (node_name == "Target3")
				{
					if (!node->first_attribute() ||
						!node->first_attribute()->next_attribute() ||
						!node->first_attribute()->next_attribute()->next_attribute())
					{
						XPLICIT_CRITICAL("SceneLoader: Bad Target3!");
						break;
					}

					String attr_x = node->first_attribute()->name();
					String attr_y = node->first_attribute()->next_attribute()->name();
					String attr_z = node->first_attribute()->next_attribute()->next_attribute()->name();
					String attr_referent = node->first_attribute()->next_attribute()->next_attribute()->next_attribute()->name();

					if (attr_x == "X" &&
						attr_y == "Y" &&
						attr_z == "Z" &&
						attr_referent == "Referent")
					{
						String id = node->first_attribute()->next_attribute()->next_attribute()->next_attribute()->value();

						String x = node->first_attribute()->value();
						String y = node->first_attribute()->next_attribute()->value();
						String z = node->first_attribute()->next_attribute()->next_attribute()->value();

						world_node.Rotation.X = std::atof(x.c_str());
						world_node.Rotation.Y = std::atof(y.c_str());
						world_node.Rotation.Z = std::atof(z.c_str());

						if (params.Has3D)
						{
							irr::scene::ICameraSceneNode* scene_node = (irr::scene::ICameraSceneNode*)RENDERER->getSceneManager()->getSceneNodeFromName(id.c_str());

							const auto pos = irr::core::vector3df(std::atof(x.c_str()), std::atof(y.c_str()), std::atof(z.c_str()));

							if (scene_node)
								scene_node->setTarget(pos);
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
						XPLICIT_CRITICAL("SceneLoader: Bad Rotate3!");
						break;
					}

					String attr_x = node->first_attribute()->name();
					String attr_y = node->first_attribute()->next_attribute()->name();
					String attr_z = node->first_attribute()->next_attribute()->next_attribute()->name();
					String attr_referent = node->first_attribute()->next_attribute()->next_attribute()->next_attribute()->name();

					if (attr_x == "X" &&
						attr_y == "Y" &&
						attr_z == "Z" &&
						attr_referent == "Referent")
					{
						String id = node->first_attribute()->next_attribute()->next_attribute()->next_attribute()->value();

						String x = node->first_attribute()->value();
						String y = node->first_attribute()->next_attribute()->value();
						String z = node->first_attribute()->next_attribute()->next_attribute()->value();

						world_node.Rotation.X = std::atof(x.c_str());
						world_node.Rotation.Y = std::atof(y.c_str());
						world_node.Rotation.Z = std::atof(z.c_str());

						if (params.Has3D)
						{
							auto scene_node = (irr::scene::ICameraSceneNode*)RENDERER->getSceneManager()->getSceneNodeFromName(id.c_str());

							const auto pos = irr::core::vector3df(std::atof(x.c_str()), std::atof(y.c_str()), std::atof(z.c_str()));

							if (scene_node)
							{
								scene_node->setRotation(pos);
							}
						}
					}
				}

				if (node_name == "DataValue")
				{
					if (node->first_attribute() &&
						node->first_attribute()->next_attribute())
					{
						String name = node->first_attribute()->next_attribute()->name();

						if (name == "Kind")
						{
							String first_attr_value = node->first_attribute()->value();
							DataValue data;

							data.Name = node->first_attribute()->name();

							if (auto ptr = node->first_attribute()->value();
								ptr)
							{
								data.Name += ".";
								data.Name += ptr;
							}

							data.Kind = DATA_VALUE_TYPE::NIL;

							if ("Boolean" == first_attr_value)
							{
								data.Kind = DATA_VALUE_TYPE::BOOLEAN;
							}

							if ("StringUTF8" == first_attr_value)
							{
								data.Kind = DATA_VALUE_TYPE::STRING_UTF8;
							}

							if ("Integer32" == first_attr_value)
							{
								data.Kind = DATA_VALUE_TYPE::INTEGER_32;
							}

							if ("IEE754" == first_attr_value)
							{
								data.Kind = DATA_VALUE_TYPE::IEE754;
							}

							String value = node->value();
							String tmp;

							for (size_t i = 0; i < value.size(); i++)
							{
								if (value[i] != ',' &&
									isalnum(value[i]))
								{
									tmp += value[i];
								}
								else if (value[i] == ',')
								{
									data.Values.push_back(tmp);
									tmp.clear();
								}
							}

							params.DataValues.push_back(data);
						}
					}
				}

				if (node_name == "Position3")
				{
					if (node->first_attribute() &&
						node->first_attribute()->next_attribute() &&
						node->first_attribute()->next_attribute()->next_attribute() &&
						node->first_attribute()->next_attribute()->next_attribute()->next_attribute())
					{
						String attr_x = node->first_attribute()->name();
						String attr_y = node->first_attribute()->next_attribute()->name();
						String attr_z = node->first_attribute()->next_attribute()->next_attribute()->name();
						String attr_referent = node->first_attribute()->next_attribute()->next_attribute()->next_attribute()->name();

						if (attr_x == "X" &&
							attr_y == "Y" &&
							attr_z == "Z" &&
							attr_referent == "Referent")
						{
							String id = node->first_attribute()->next_attribute()->next_attribute()->next_attribute()->value();

							String x = node->first_attribute()->value();
							String y = node->first_attribute()->next_attribute()->value();
							String z = node->first_attribute()->next_attribute()->next_attribute()->value();

							world_node.Position.X = std::atof(x.c_str());
							world_node.Position.Y = std::atof(y.c_str());
							world_node.Position.Z = std::atof(z.c_str());

							if (params.Has3D)
							{
								const auto scene_node = RENDERER->getSceneManager()->getSceneNodeFromName(id.c_str());

								if (scene_node)
								{
									auto pos = irr::core::vector3df(world_node.Position.X, world_node.Position.Y, world_node.Position.Z);
									scene_node->setPosition(pos);
								}
							}

						}
					}
				}

				if (node_name == "Scale3")
				{
					if (node->first_attribute() &&
						node->first_attribute()->next_attribute() &&
						node->first_attribute()->next_attribute()->next_attribute())
					{
						String attr_x = node->first_attribute()->name();
						String attr_y = node->first_attribute()->next_attribute()->name();
						String attr_z = node->first_attribute()->next_attribute()->next_attribute()->name();
						String attr_referent = node->first_attribute()->next_attribute()->next_attribute()->next_attribute()->name();

						if (attr_x == "X" &&
							attr_y == "Y" &&
							attr_z == "Z" &&
							attr_referent == "Referent")
							{
								String id = node->first_attribute()->next_attribute()->next_attribute()->next_attribute()->value();

							String x = node->first_attribute()->value();
							String y = node->first_attribute()->next_attribute()->value();
							String z = node->first_attribute()->next_attribute()->next_attribute()->value();

							world_node.Size.X = std::atof(x.c_str());
							world_node.Size.Y = std::atof(y.c_str());
							world_node.Size.Z = std::atof(z.c_str());

							if (params.Has3D)
							{
								const auto scene_node = RENDERER->getSceneManager()->getSceneNodeFromName(id.c_str());
								
								if (scene_node)
									scene_node->setScale(irr::core::vector3df(std::atof(x.c_str()), std::atof(y.c_str()), std::atof(z.c_str())));
							}
						}
					}
				}

				if (node_name == "Sound")
				{
					if (node->first_attribute() &&
						node->first_attribute()->next_attribute())
					{
						if (strcmp(node->first_attribute()->name(), "Name") == 0)
							world_node.ID = node->first_attribute()->value();

						if (strcmp(node->first_attribute()->next_attribute()->name(), "Value") == 0)
							world_node.Value = node->first_attribute()->next_attribute()->value();
					}
				}

				world_node.Name = node_name;

				if (!world_node.Name.empty())
					params.WorldNodes.push_back(world_node);

				node = node->next_sibling();
			}

			params.Done = true;
			});

		if (!params.WaitFor)
		{
			stream_job.detach();
		}
		else
		{
			stream_job.join();
		}
	}
}

void DVFromRXML(XPX::RXML::RXMLDocumentParams& params) noexcept
{
	XPX::Thread data_values_job([](XPX::RXML::RXMLDocumentParams params) {
		XPX::String fmt;

		if (params.DataValues.size() > 0)
		{
			for (size_t i = 0; i < params.DataValues.size(); i++)
			{
				XPX::String final_string;

				for (size_t y = 0; y < params.DataValues[i].Values.size(); y++)
				{
					final_string += params.DataValues[i].Values[y];
					final_string += ",";
				}

				fmt = params.DataValues[i].Name.c_str();
				fmt += "=";
				fmt += fmt::format("{}{}{}", "{", final_string, "}");

				XPX::Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

				fmt.clear();
			}
		}
		}, params);

	data_values_job.detach();
}