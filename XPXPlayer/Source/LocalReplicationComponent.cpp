/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "LocalReplicationComponent.h"
#include "LocalNetworkMonitorEvent.h"
#include "LuaScriptComponent.h"
#include "PartComponent.h"
#include "WeaponComponent.h"
#include "MenuUI.h"
#include "App.h"

#include <CLua.hpp>
#include <Enums.h>
#include <RXML.h>
#include <Util.h>
#include <Uri.h>

namespace XPX
{
	COMPONENT_TYPE LocalReplicationComponent::type() noexcept { return COMPONENT_REPLICATION; }

	const char* LocalReplicationComponent::name() noexcept { return "LocalReplicationManager"; }

	bool LocalReplicationComponent::should_update() noexcept { return true; }

	void LocalReplicationComponent::update(ClassPtr class_ptr)
	{
		LocalReplicationComponent* self = (LocalReplicationComponent*)class_ptr;

		if (!self ||
			!self->mNetwork)
			return;

		NetworkPacket packet;
		self->mNetwork->read(packet);

		if (packet.channel == XPLICIT_CHANNEL_DATA &&
			packet.cmd[XPLICIT_NETWORK_CMD_DOWNLOAD] == NETWORK_CMD_DOWNLOAD)
		{
			switch (packet.id)
			{
			case COMPONENT_ID_SCRIPT:
			{
				String url;

				url = packet.replicas[XPLICIT_REPLICA_1];

				if (url.empty() ||
					url.find("xasset://") == String::npos)
					return;

				auto tmp = uuids::to_string(UUIDFactory::version<4>()) + "-LUA";

				if (DownloadURL(url, tmp))
				{
					XPLICIT_GET_DATA_DIR(full_path);

					String full_download_path;

					full_download_path += full_path;
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

				break;
			}
			case COMPONENT_ID_ROXML:
			{
				String url;

				url = packet.replicas[XPLICIT_REPLICA_1];

				if (url.empty() ||
					url.find("xasset://") == String::npos)
					return;

				auto tmp = uuids::to_string(UUIDFactory::version<4>()) + "-TMP-LUA";

				if (DownloadURL(url, tmp))
				{
					XPX::RXML::RXMLDocument XPX_PARSER;

					XPLICIT_GET_DATA_DIR(full_path);

					String full_download_path;

					full_download_path += full_path;
					full_download_path += "Contents/";
					full_download_path += tmp;

					RXML::RXMLDocumentParams params;

					params.Inline = false;
					params.Path = tmp;

					XPX_PARSER.parse(params);
				}

				break;
			}
			default:
				break;
			}
		}
		else if (packet.channel == XPLICIT_CHANNEL_3D)
		{
			String xplicit_id = packet.additional_data;

			if (xplicit_id.find("XPX_") != String::npos)
			{
				xplicit_id = xplicit_id.substr(xplicit_id.find("XPX_"));

				auto bundles = ComponentSystem::get_singleton_ptr()->all_of<MeshComponent>();

				auto it = std::find_if(bundles.begin(), bundles.end(), [&](MeshComponent* mesh) -> bool {
					return mesh->instance_name() == xplicit_id;
					});

				if (it == bundles.cend())
				{
					auto bundle = ComponentSystem::get_singleton_ptr()->add<MeshComponent>("Media/NBot.dae", xplicit_id.c_str(), "world");
					XPLICIT_ASSERT(bundle);
				}
				else
				{
					if (packet.cmd[XPLICIT_NETWORK_CMD_DESTROY] == NETWORK_CMD_DESTROY)
					{
						ComponentSystem::get_singleton_ptr()->remove(*it);
					}
					else
					{
						auto node_bundle = (*it);

						if (node_bundle)
						{
							auto vec = vector3df(packet.pos[0][XPLICIT_NETWORK_X],
								packet.pos[0][XPLICIT_NETWORK_Y],
								packet.pos[0][XPLICIT_NETWORK_Z]);

							node_bundle->node()->setPosition(vec);
						}

						return;
					}
				}
			}
			else
			{
				PartComponent* node = ComponentSystem::get_singleton_ptr()->get<PartComponent>(packet.additional_data);

				if (node)
				{
					if (packet.cmd[XPLICIT_NETWORK_CMD_DESTROY] == NETWORK_CMD_DESTROY)
					{
						ComponentSystem::get_singleton_ptr()->remove(node);
					}
					else
					{
						// get the part.
						auto part = node->node();
						auto vec = vector3df(packet.pos[0][XPLICIT_NETWORK_X],
							packet.pos[0][XPLICIT_NETWORK_Y],
							packet.pos[0][XPLICIT_NETWORK_Z]);

						part->setPosition(vec);

						vec = vector3df(packet.pos[1][XPLICIT_NETWORK_X],
							packet.pos[1][XPLICIT_NETWORK_Y],
							packet.pos[1][XPLICIT_NETWORK_Z]);

						part->setRotation(vec);

						vec = vector3df(packet.pos[2][XPLICIT_NETWORK_X],
							packet.pos[2][XPLICIT_NETWORK_Y],
							packet.pos[2][XPLICIT_NETWORK_Z]);

						part->setScale(vec);
					}
				}
				else
				{
					String parent = String(packet.additional_data);
					String name = "";

					if (parent.find(".") != String::npos)
					{
						name = String(packet.additional_data, parent.find("."));
					}
					else
					{
						name = parent;
						parent = XPLICIT_LUA_NAMESPACE;
					}

					parent = parent.substr(parent.find(".") + 1);

					auto part = ComponentSystem::get_singleton_ptr()->add<PartComponent>(name.c_str(), parent.c_str());
				
					XPLICIT_ASSERT(part);

					CAD->getSceneManager()->addLightSceneNode(part->node(), part->node()->getPosition(),
						SColorf(1.0f, 1.0f, 1.0f),
						300.0f);
					
					for (size_t mat_idx = 0; mat_idx < part->node()->getMaterialCount(); ++mat_idx)
					{
						part->node()->setMaterialTexture(mat_idx, CAD->getVideoDriver()->getTexture("NoTex.png"));
					}
				}
			}
		}
		else if (packet.channel == XPLICIT_CHANNEL_WEAPON)
		{
			String parent = String(packet.additional_data);
			String name = "";

			bool eval = (parent.find(".") != String::npos);
			
			name = eval ? parent : String(packet.additional_data, parent.find("."));
			parent = eval ? parent : XPLICIT_LUA_NAMESPACE;

			parent = parent.substr(parent.find(".") + 1);

			auto gears = ComponentSystem::get_singleton_ptr()->all_of<WeaponComponent>();

			for (auto* gear : gears)
			{
				if (gear &&
					gear->instance_name() == name &&
					gear->group_name() == parent)
				{
					auto node = gear->get_mesh()->node();

					if (!node)
					{
						gear->assign("Path", packet.replicas[XPLICIT_REPLICA_4]);
						return;
					}

					node->setPosition(vector3df(packet.pos[0][XPLICIT_NETWORK_X], packet.pos[0][XPLICIT_NETWORK_Y], packet.pos[0][XPLICIT_NETWORK_Z]));
					node->setRotation(vector3df(packet.pos[2][XPLICIT_NETWORK_X], packet.pos[2][XPLICIT_NETWORK_Y], packet.pos[2][XPLICIT_NETWORK_Z]));
					node->setScale(vector3df(packet.pos[1][XPLICIT_NETWORK_X], packet.pos[1][XPLICIT_NETWORK_Y], packet.pos[1][XPLICIT_NETWORK_Z]));

					return;
				}
			}

			WeaponComponent* gear = ComponentSystem::get_singleton_ptr()->add<WeaponComponent>(name.c_str(), 
																						   packet.replicas[XPLICIT_REPLICA_4], parent.c_str());

			XPLICIT_ASSERT(gear);
		}
	}
}