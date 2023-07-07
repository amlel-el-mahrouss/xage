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

// Common includes
#include "ClassComponent.h"
#include "DataValue.h"
#include "Root.h"
#include "Util.h"

// CLua
#include <lua/CLua.hpp>

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
							String klass_to_instanciate;

							if (klass &&
								strcmp(klass->name(), "Object") == 0)
								klass_to_instanciate = klass->value();

							// Here is the list of run-time components { "Light", "Mesh", "Sound", "Particle" };

							if (strcmp(node->first_attribute()->name(), "Name") == 0)
							{
								auto node_id = node->first_attribute()->value();

								world_node.Name = node_name;
								world_node.ID = node_id;

								const char* script_id = "";
								const char* parent_id = "";

								// For for a lua attribute!
								if (node->first_attribute()->next_attribute() &&
									strcmp(node->first_attribute()->next_attribute()->name(), "Lua") == 0)
									script_id = node->first_attribute()->next_attribute()->value();

								// now check for a parent!
								if (node->first_attribute()->next_attribute() &&
									strcmp(node->first_attribute()->next_attribute()->name(), "Parent") == 0)
									parent_id = node->first_attribute()->next_attribute()->value();

								auto component = ComponentSystem::get_singleton_ptr()->add<ClassComponent>(
									Vector<float>(0, 0, 0),
									Vector<float>(0, 0, 0),
									Color<float>(0, 0, 0),
									params.NoLua ? script_id : nullptr,
									parent_id,
									node_id);

								if (params.Has3D)
								{
									void* object = nullptr;

									if (klass_to_instanciate == "Light")
									{
										irr::scene::ILightSceneNode* light = nullptr;
										object = light = RENDER->getSceneManager()->addLightSceneNode();
										light->setName(node_id);
									}

									if (klass_to_instanciate == "Stud")
									{
										irr::scene::ISceneNode* stud = nullptr;
										object = stud = RENDER->getSceneManager()->addMeshSceneNode(RENDER->getSceneManager()->getGeometryCreator()->createCubeMesh());
										stud->setName(node_id);
									}

									if (klass_to_instanciate == "Mesh")
									{
										XPLICIT_GET_DATA_DIR(mesh_path);

										for (size_t i = 0; i < strlen(node->value()); i++)
										{
											if (isalnum(node->value()[i]) ||
												node->value()[i] == '.' ||
												node->value()[i] == '/' ||
												node->value()[i] == '\\')
												mesh_path += node->value()[i];
										}

										auto mesh_ptr = RENDER->getSceneManager()->addMeshSceneNode(RENDER->getSceneManager()->getMesh(mesh_path.c_str()));
										mesh_ptr->setName(node_id);

										object = mesh_ptr;
									}

									if (klass_to_instanciate == "Particle")
									{
										auto emitter = RENDER->getSceneManager()->addParticleSystemSceneNode(true);
										emitter->setName(node_id);

										object = emitter;
									}

									if (object)
									{
										irr::scene::ISceneNode* node = nullptr;

										if (node = RENDER->getSceneManager()->getSceneNodeFromName(parent_id); !node)
										{
											ComponentSystem::get_singleton_ptr()->remove<ClassComponent>(component);
										}
										else
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
												scene_node->getMaterial(0).DiffuseColor.color = mat_id_cast;
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
								const auto scene_node = RENDER->getSceneManager()->getSceneNodeFromName(id.c_str());

								if (scene_node)
									scene_node->setRotation(irr::core::vector3df(std::atof(x.c_str()), std::atof(y.c_str()), std::atof(z.c_str())));
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

					// if it is a CLua snippet
					if (node_name == "CLua") // and it is really what we think it is.
					{
						// go on and include that!
						world_node.ID = node_name;
						world_node.Value = node->value();

						Lua::CLuaStateManager::get_singleton_ptr()->run_string(world_node.Value.c_str());
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