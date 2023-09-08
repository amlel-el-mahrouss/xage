/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright PlayXPlicit, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

/* RoXML format */
/* This format describes scene-nodes and stuff like that. */

// XML
#include <rapidxml/rapidxml_utils.hpp>
#include <rapidxml/rapidxml.hpp>

// Common includes
#include "LuaScriptComponent.h"
#include "ClassComponent.h"
#include "MeshComponent.h"
#include "PartComponent.h"
#include "XHTTPManager.h"
#include "DataValue.h"
#include "Root.h"
#include "Util.h"

// CLua
#include <CLua/CLua.hpp>

#ifndef XPLICIT_XASSET_ENDPOINT
#	define XPLICIT_XASSET_ENDPOINT "play-xplicit.com"
#endif // ifndef XPLICIT_XASSET_ENDPOINT

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

		bool Inline{ false };
		bool Has3D{ false };
		bool LuaOnly{ false };
		bool NoLua{ false };
		bool WaitFor{ false };

		std::vector<DataValue> DataValues;
		std::vector<RoXMLNodeDescription> WorldNodes;

	};

	class XPLICIT_API RoXMLDocumentParser final
	{
	public:
		RoXMLDocumentParser() = default;
		~RoXMLDocumentParser() = default;

	public:
		XPLICIT_COPY_DEFAULT(RoXMLDocumentParser);

	public:
		void parse(RoXMLDocumentParameters& params) noexcept
		{
			if (!params.Inline)
			{
				if (params.Path.empty() ||
					!std::filesystem::exists(params.Path))
					return;
			}

			Thread stream_job([&]() {
				std::unique_ptr<xml_document<>> doc = std::make_unique<xml_document<>>();

				//
				file<char> xml_file(params.Path.c_str());

				if (!params.Inline)
					doc->parse<0>(xml_file.data());
				else
					doc->parse<0>(params.Path.data());

				xml_node<>* root_node = doc->first_node();
				xml_node<>* node = root_node->first_node();

				while (node)
				{
					const String node_name = node->name();
					RoXMLNodeDescription world_node;

					if (node_name == "Class")
					{
						if (node->first_attribute())
						{
							auto klass = node->last_attribute();
							String klass_to_instanciate = klass->name();

							if (klass &&
								klass_to_instanciate == "Object")
								klass_to_instanciate = klass->value();

							// Here is the list of run-time components { "Light", "Mesh", "Sound", "Particle" };

							if (strcmp(node->first_attribute()->name(), "Name") == 0)
							{
								auto node_id = node->first_attribute()->value();

								world_node.Name = node_name;
								world_node.ID = node_id;

								const char* script_id = "";
								const char* parent_id = XPLICIT_CLASS_NAMESPACE;

								// For for a lua attribute!
								if (node->first_attribute()->next_attribute() &&
									strcmp(node->first_attribute()->next_attribute()->name(), "Lua") == 0)
									script_id = node->first_attribute()->next_attribute()->value();

								// now check for a parent!
								if (node->first_attribute()->next_attribute() &&
									strcmp(node->first_attribute()->next_attribute()->name(), "Parent") == 0)
									parent_id = node->first_attribute()->next_attribute()->value();

								if (params.Has3D)
								{
									void* object = nullptr;

									if (klass_to_instanciate == "Light")
									{
										irr::scene::ILightSceneNode* light = nullptr;
										object = light = RENDER->getSceneManager()->addLightSceneNode();
										
										light->setName(node_id);
									}

									// assign a part component to the said id
									// so you can use it.

									if (klass_to_instanciate == "Part")
									{
										PartComponent* part = ComponentSystem::get_singleton_ptr()->add<PartComponent>(node_id, parent_id);
									}

									if (klass_to_instanciate == "Mesh")
									{
										XPLICIT_GET_DATA_DIR(mesh_path);

										for (size_t i = 0; i < strlen(node->value()); i++)
										{
											if (node->value()[i] == '\r' ||
												node->value()[i] == '\n' ||
												node->value()[i] == '\t' ||
												node->value()[i] == ' ')
												continue;

											mesh_path += node->value()[i];
										}

										MeshComponent* _mesh = ComponentSystem::get_singleton_ptr()->add<MeshComponent>(mesh_path.c_str(), node_id, parent_id);

										if (_mesh)
											object = _mesh;
									}

									if (klass_to_instanciate == "Skydome")
									{
										irr::scene::ISceneNode* skydome = nullptr;
										skydome = RENDER->getSceneManager()->addSkyDomeSceneNode(RENDER->getVideoDriver()->getTexture(node->value()));
										object = skydome;

										if (skydome)
										{
											skydome->setMaterialFlag(irr::video::EMF_LIGHTING, false);
											skydome->setName(node_id);
										}
									}

									if (klass_to_instanciate == "Particle")
									{
										auto particle_scene_node = RENDER->getSceneManager()->addParticleSystemSceneNode(true);
										particle_scene_node->setName(node_id);

										particle_scene_node->setMaterialTexture(0, RENDER->getVideoDriver()->getTexture(node->value()));
										particle_scene_node->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
										particle_scene_node->setPosition(irr::core::vector3df(0, 0, 0));
										particle_scene_node->setScale(irr::core::vector3df(2, 2, 2));
										particle_scene_node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
										particle_scene_node->setMaterialFlag(irr::video::EMF_ZWRITE_ENABLE, false);

										// TODO: let people edit that shit
										irr::scene::IParticleEmitter* em = particle_scene_node->createBoxEmitter(
											irr::core::aabbox3d<irr::f32>(-7, 0, -7, 7, 1, 7), // emitter size
											irr::core::vector3df(0.0f, 0.06f, 0.0f),   // initial direction
											80, 100,                             // emit rate
											irr::video::SColor(0, 255, 255, 255),       // darkest color
											irr::video::SColor(0, 255, 255, 255),       // brightest color
											800, 2000, 0,                         // min and max age, angle
											irr::core::dimension2df(10.f, 10.f),         // min size
											irr::core::dimension2df(20.f, 20.f));        // max size

										particle_scene_node->setEmitter(em);

										object = particle_scene_node;
									}

									if (object)
									{
										irr::scene::ISceneNode* node = nullptr;

										if (node = RENDER->getSceneManager()->getSceneNodeFromName(parent_id); node)
										{
											node->addChild(node);
										}
									}
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

							if (attr_who == "Who" &&
								attr_mat == "Mat")
							{
								const auto mat_id = node->first_attribute()->next_attribute()->value();
								const auto mat_id_cast = std::atoi(node->value());

								if (params.Has3D)
								{
									if (mat_id)
									{
										try
										{
											const auto scene_node = RENDER->getSceneManager()->getSceneNodeFromName(node->first_attribute()->value());

											if (scene_node)
												scene_node->getMaterial(0).AmbientColor.color = mat_id_cast;
										}
										catch (...)
										{
											XPLICIT_INFO("No such Entity had been found!");
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
							XPLICIT_CRITICAL("RoXML: Bad Target3!");
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

							String x = node->first_attribute()->value();
							String y = node->first_attribute()->next_attribute()->value();
							String z = node->first_attribute()->next_attribute()->next_attribute()->value();

							world_node.Rotation.X = std::atof(x.c_str());
							world_node.Rotation.Y = std::atof(y.c_str());
							world_node.Rotation.Z = std::atof(z.c_str());

							if (params.Has3D)
							{
								irr::scene::ICameraSceneNode* scene_node = (irr::scene::ICameraSceneNode*)RENDER->getSceneManager()->getSceneNodeFromName(id.c_str());

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
							XPLICIT_CRITICAL("RoXML: Bad Rotate3!");
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

							String x = node->first_attribute()->value();
							String y = node->first_attribute()->next_attribute()->value();
							String z = node->first_attribute()->next_attribute()->next_attribute()->value();

							world_node.Rotation.X = std::atof(x.c_str());
							world_node.Rotation.Y = std::atof(y.c_str());
							world_node.Rotation.Z = std::atof(z.c_str());

							if (params.Has3D)
							{
								auto scene_node = (irr::scene::ICameraSceneNode*)RENDER->getSceneManager()->getSceneNodeFromName(id.c_str());

								const auto pos = irr::core::vector3df(std::atof(x.c_str()), std::atof(y.c_str()), std::atof(z.c_str()));

								if (scene_node)
									scene_node->setRotation(pos);
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
							node->first_attribute()->next_attribute()->next_attribute())
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

								String x = node->first_attribute()->value();
								String y = node->first_attribute()->next_attribute()->value();
								String z = node->first_attribute()->next_attribute()->next_attribute()->value();

								world_node.Position.X = std::atof(x.c_str());
								world_node.Position.Y = std::atof(y.c_str());
								world_node.Position.Z = std::atof(z.c_str());


								if (params.Has3D)
								{
									const auto scene_node = RENDER->getSceneManager()->getSceneNodeFromName(id.c_str());

									if (scene_node)
										scene_node->setPosition(irr::core::vector3df(std::atof(x.c_str()), std::atof(y.c_str()), std::atof(z.c_str())));
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

								String x = node->first_attribute()->value();
								String y = node->first_attribute()->next_attribute()->value();
								String z = node->first_attribute()->next_attribute()->next_attribute()->value();

								world_node.Size.X = std::atof(x.c_str());
								world_node.Size.Y = std::atof(y.c_str());
								world_node.Size.Z = std::atof(z.c_str());

								if (params.Has3D)
								{
									const auto scene_node = RENDER->getSceneManager()->getSceneNodeFromName(id.c_str());
									scene_node->setScale(irr::core::vector3df(std::atof(x.c_str()), std::atof(y.c_str()), std::atof(z.c_str())));
								}
							}
						}
					}

					if (node_name == "CLua") // and it is really what we think it is.
					{
						// go on and include that!
						world_node.ID = node_name;

						for (size_t i = 0; i < strlen(node->value()); i++)
						{
							if (isalnum(node->value()[i]) ||
								node->value()[i] == '.' ||
								node->value()[i] == '/' ||
								node->value()[i] == '\\' ||
								node->value()[i] == ':')
							{
								world_node.Value += node->value()[i];
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

							auto tmp = std::to_string(xplicit_get_epoch()) + "-roxml-tmp.lua";

							if (HTTP->download(url, tmp))
							{
								XPLICIT_GET_DATA_DIR(full_path);

								String full_download_path;

								full_download_path += full_path;
								full_download_path += "Contents/";
								full_download_path += tmp;

								ComponentSystem::get_singleton_ptr()->add<LuaScriptComponent>(full_download_path.c_str());
							}
						}
						else
						{
							XPLICIT_CRITICAL("Missing script! URL: " + url);
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

	};
}

// Utilities related to data values.

inline void LoadDVFromRoXML(Xplicit::RoXML::RoXMLDocumentParameters& params) noexcept
{
	Xplicit::Thread data_values_job([](Xplicit::RoXML::RoXMLDocumentParameters params) {
		Xplicit::String fmt;

		if (params.DataValues.size() > 0)
		{
			for (size_t i = 0; i < params.DataValues.size(); i++)
			{
				Xplicit::String final_string;

				for (size_t y = 0; y < params.DataValues[i].Values.size(); y++)
				{
					final_string += params.DataValues[i].Values[y];
					final_string += ",";
				}

				fmt = params.DataValues[i].Name.c_str();
				fmt += "=";
				fmt += std::format("{}{}{}", "{", final_string, "}");

				Xplicit::Lua::CLuaStateManager::get_singleton_ptr()->run_string(fmt.c_str());

				fmt.clear();
			}
		}
		}, params);

	data_values_job.detach();
}