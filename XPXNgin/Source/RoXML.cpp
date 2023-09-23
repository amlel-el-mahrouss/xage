/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "RoXML.h"

namespace XPX::RoXML
{
	void RoXMLDocumentParser::parse(RoXMLDocumentParameters& params) noexcept
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

							const char* script_id = "";
							const char* parent_id = XPLICIT_CLASS_NAMESPACE;

							
							if (first_attr->next_attribute()->next_attribute() &&
								strcmp(first_attr->next_attribute()->next_attribute()->name(), "Script") == 0)
							{
								script_id = first_attr->next_attribute()->next_attribute()->value();
							}
							
							if (first_attr->next_attribute()->next_attribute()->next_attribute() &&
								strcmp(first_attr->next_attribute()->next_attribute()->next_attribute()->name(), "Parent") == 0)
							{
								parent_id = first_attr->next_attribute()->next_attribute()->next_attribute()->value();

								if (strlen(parent_id) < 1)
									parent_id = XPLICIT_LUA_NAMESPACE;
							}

							void* object = nullptr;

							// assign a part component to the said id
							// so you can use it.

							if (klass_to_instantiate == "Part")
							{
								auto node_mesh = CAD->getSceneManager()->addCubeSceneNode();

								if (node_mesh)
								{
									if (auto parent = CAD->getSceneManager()->getSceneNodeFromName(parent_id);
										parent)
										node_mesh->setParent(parent);

									node_mesh->setName(node_id);
									object = node_mesh;
								}
							}

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

								String url = world_node.Value;

								std::unique_ptr<XHTTPManager> HTTP = std::make_unique<XHTTPManager>();

								if (HTTP &&
									url.find("xasset://") != String::npos)
								{
									String substr = url.erase(url.find("xasset://"), strlen("xasset://"));

									url.clear();
									url = "/";
									url += substr;

									HTTP->set_endpoint(XPLICIT_XASSET_ENDPOINT);

									XPLICIT_GET_DATA_DIR(full_path);

									auto tmp = uuids::to_string(XPX::UUIDFactory::version<4>()) + "-DAE";
									tmp += XPLICIT_OBJ_FORMAT;

									if (HTTP->download(url, tmp))
									{
										XPLICIT_GET_DATA_DIR(full_path);

										String full_download_path = full_path;
										full_download_path += "Contents/";
										full_download_path += tmp;

										auto node_mesh = CAD->getSceneManager()->addMeshSceneNode(CAD->getSceneManager()->getMesh(full_download_path.c_str()));

										if (node_mesh)
										{
											if (auto parent = CAD->getSceneManager()->getSceneNodeFromName(parent_id);
												parent)
												node_mesh->setParent(parent);

											node_mesh->setName(node_id);
											object = node_mesh;
										}
									}
								}
							}

							//! here you can see that Has3D is strictly reserved for headless implementations of XPX tech.
							//! however, the classcomponent asks for it the mesh/part specs.
							//! that's fine just ask for it, cad still loads it with np.
							if (object &&
								!params.Has3D)
							{
								irr::scene::ISceneNode* node = nullptr;

								if (node = CAD->getSceneManager()->getSceneNodeFromName(parent_id);
									node)
									node->addChild(node);

								auto scene_node = (ISceneNode*)object;

								// finally create an replicatable instance of the newly created node.
								ClassComponent* component = ComponentSystem::get_singleton_ptr()->add<ClassComponent>(
									Vector<NetworkFloat>(scene_node->getPosition().X,
										scene_node->getPosition().Y,
										scene_node->getPosition().Z),
									Vector<NetworkFloat>(scene_node->getScale().X,
										scene_node->getScale().Y,
										scene_node->getScale().Z),
									Color<NetworkFloat>(255, 255, 255, 255), script_id, parent_id, node_id);

								XPLICIT_ASSERT(component);
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
										const auto scene_node = CAD->getSceneManager()->getSceneNodeFromName(node->first_attribute()->value());

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
							irr::scene::ICameraSceneNode* scene_node = (irr::scene::ICameraSceneNode*)CAD->getSceneManager()->getSceneNodeFromName(id.c_str());

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
							auto scene_node = (irr::scene::ICameraSceneNode*)CAD->getSceneManager()->getSceneNodeFromName(id.c_str());

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
								const auto scene_node = CAD->getSceneManager()->getSceneNodeFromName(id.c_str());

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
								const auto scene_node = CAD->getSceneManager()->getSceneNodeFromName(id.c_str());
								
								if (scene_node)
									scene_node->setScale(irr::core::vector3df(std::atof(x.c_str()), std::atof(y.c_str()), std::atof(z.c_str())));
							}
						}
					}
				}

				if (node_name == "Script") // and it is really what we think it is.
				{
					// go on and include that!
					world_node.ID = node_name;

					if (node->first_attribute() &&
						strcmp(node->first_attribute()->name(), "Path") == 0)
					{
						auto val = node->first_attribute()->value();

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

						String url = world_node.Value;

						std::unique_ptr<XHTTPManager> HTTP = std::make_unique<XHTTPManager>();

						if (HTTP &&
							url.find("xasset://") != String::npos)
						{
							String substr = url.erase(url.find("xasset://"), strlen("xasset://"));

							url.clear();
							url = "/";
							url += substr;

							HTTP->set_endpoint(XPLICIT_XASSET_ENDPOINT);

							XPLICIT_GET_DATA_DIR(full_path);

							auto tmp = uuids::to_string(XPX::UUIDFactory::version<4>()) + "-LUA";

							if (HTTP->download(url, tmp))
							{
								XPLICIT_GET_DATA_DIR(full_path);

								String full_download_path = full_path;
								full_download_path += "Contents/";
								full_download_path += tmp;

								auto script = ComponentSystem::get_singleton_ptr()->add<LuaScriptComponent>(full_download_path.c_str());
								
								if (script)
									script->run_script();

								if (script &&
									script->status() == LuaScriptComponent::LUA_STOP)
								{
									std::remove(full_download_path.c_str());
									ComponentSystem::get_singleton_ptr()->remove(script);
								}
							}
						}
					}
					else
					{
						auto tmp = uuids::to_string(XPX::UUIDFactory::version<4>()) + "-LUA";

						XPLICIT_GET_DATA_DIR(full_path);

						String full_write_path = full_path;
						full_write_path += "Contents/";
						full_write_path += tmp;

						std::ofstream file(full_write_path);

						file << node->value();

						auto script = ComponentSystem::get_singleton_ptr()->add<LuaScriptComponent>(full_write_path.c_str());

						if (script)
							script->run_script();

						if (script &&
							script->status() == LuaScriptComponent::LUA_STOP)
						{
							std::remove(full_write_path.c_str());
							ComponentSystem::get_singleton_ptr()->remove(script);
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

void DVFromRoXML(XPX::RoXML::RoXMLDocumentParameters& params) noexcept
{
	XPX::Thread data_values_job([](XPX::RoXML::RoXMLDocumentParameters params) {
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