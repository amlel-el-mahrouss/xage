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
#include <CLua/CLua.hpp>

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
								const char* parent_id = XPLICIT_CLASS_NAMESPACE;

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

								XPLICIT_ASSERT(component);

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
											if (node->value()[i] == '\r' ||
												node->value()[i] == '\n')
												continue;

											mesh_path += node->value()[i];
										}

										auto mesh = RENDER->getSceneManager()->getMesh(mesh_path.c_str());
										
										if (mesh)
										{
											auto mesh_ptr = RENDER->getSceneManager()->addMeshSceneNode(mesh);

											if (mesh_ptr)
												mesh_ptr->setName(node_id);

											object = mesh_ptr;
										}
										else
										{
											object = nullptr;
										}
									}

									if (klass_to_instanciate == "Sky")
									{
										irr::scene::ISceneNode* nod = nullptr;
										nod = RENDER->getSceneManager()->addSkyDomeSceneNode(RENDER->getVideoDriver()->getTexture(node->value()));
										object = nod;

										nod->setName(node_id);
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

					if (node_name == "Sound")
					{
						if (node->first_attribute() &&
							strcmp(node->first_attribute()->name(), "Path") == 0)
						{
							auto component = ComponentSystem::get_singleton_ptr()->add<ClassComponent>(
								Vector<float>(0, 0, 0),
								Vector<float>(0, 0, 0),
								Color<float>(0, 0, 0),
								nullptr,
								"World",
								node->value());

							component->insert("Path", node->first_attribute()->value());
							component->insert("Play", "function(self) _G.World.SoundService.Play(self.Path); end");
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