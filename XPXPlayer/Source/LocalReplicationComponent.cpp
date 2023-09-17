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
#include "GearComponent.h"
#include "Application.h"
#include "MenuUI.h"

#include <CLua.hpp>
#include <Enums.h>
#include <RoXML.h>
#include <Util.h>
#include <Uri.h>

namespace XPX
{
	static XPX::RoXML::RoXMLDocumentParser XPX_PARSER;

	COMPONENT_TYPE LocalReplicationComponent::type() noexcept { return COMPONENT_REPLICATION; }

	const char* LocalReplicationComponent::name() noexcept { return "LocalReplicationManager"; }

	bool LocalReplicationComponent::should_update() noexcept { return true; }

	void LocalReplicationComponent::update(ClassPtr class_ptr)
	{
		LocalReplicationComponent* self = (LocalReplicationComponent*)class_ptr;

		if (!self->mNetwork)
			return;

		NetworkPacket packet = self->mNetwork->get();

		if (packet.channel == XPLICIT_CHANNEL_DATA)
		{
			if (packet.cmd[XPLICIT_REPL_CREATE] == NETWORK_REPL_CMD_CREATE)
			{
				switch (packet.id)
				{
				case COMPONENT_ID_SCRIPT:
				{
					String url;

					url = packet.replicas[XPLICIT_REPLICA_PLAYER];

					if (url.empty() ||
						url.find("xasset://") == String::npos)
						return;

					auto tmp = uuids::to_string(UUIDFactory::version<4>()) + "-TMP-LUA";

					if (DownloadURL(url, tmp))
					{
						XPLICIT_GET_DATA_DIR(full_path);

						String full_download_path;

						full_download_path += full_path;
						full_download_path += "Contents/";
						full_download_path += tmp;

						ComponentSystem::get_singleton_ptr()->add<LuaScriptComponent>(full_download_path.c_str());
					}

					break;
				}
				case COMPONENT_ID_ROXML:
				{
					String url;

					url = packet.replicas[XPLICIT_REPLICA_PLAYER];

					if (url.empty() ||
						url.find("xasset://") == String::npos)
						return;

					auto tmp = uuids::to_string(UUIDFactory::version<4>()) + "-TMP-LUA";

					if (DownloadURL(url, tmp))
					{
						XPLICIT_GET_DATA_DIR(full_path);

						String full_download_path;

						full_download_path += full_path;
						full_download_path += "Contents/";
						full_download_path += tmp;

						RoXML::RoXMLDocumentParameters params;

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
			else if (packet.cmd[XPLICIT_REPL_DESTROY] == NETWORK_REPL_CMD_DESTROY)
			{
				switch (packet.id)
				{
				case COMPONENT_ID_SCRIPT:
				{
					String name;
					name = packet.replicas[XPLICIT_REPLICA_PLAYER];

					if (auto script = ComponentSystem::get_singleton_ptr()->get<LuaScriptComponent>(name.c_str()))
					{
#ifdef XPLICIT_DEBUG
						XPLICIT_INFO("Removing script...");
#endif

						if (ComponentSystem::get_singleton_ptr()->remove(script))
							XPLICIT_INFO("Success! script has been deallocated.");
					}

					break;
				}
				}
			}
		}
		else if (packet.channel == XPLICIT_CHANNEL_PHYSICS)
		{
			NetworkPacketRepl repl_packet = *reinterpret_cast<NetworkPacketRepl*>(&packet);

			ISceneNode* node = CAD->getSceneManager()->getSceneNodeFromName(repl_packet.node_name);

			if (node)
			{
				node->setPosition(vector3df(repl_packet.pos_x, repl_packet.pos_y, repl_packet.pos_z));
				node->setScale(vector3df(repl_packet.scale_x, repl_packet.scale_y, repl_packet.scale_z));
			}
			else
			{
				switch (repl_packet.node_kind)
				{
				case 0: // sphere
				{
					node = CAD->getSceneManager()->addSphereSceneNode(repl_packet.scale_x, repl_packet.scale_y, CAD->getSceneManager()->getSceneNodeFromName(repl_packet.node_parent));
					break;
				}
				case 1: // cube
				{
					node = CAD->getSceneManager()->addCubeSceneNode(repl_packet.scale_x, CAD->getSceneManager()->getSceneNodeFromName(repl_packet.node_parent));
					break;
				}
				case 2: // mesh
				{
					auto mesh_component = ComponentSystem::get_singleton_ptr()->add<MeshComponent>(repl_packet.node_path, repl_packet.node_name, repl_packet.node_parent);
					XPLICIT_ASSERT(mesh_component);
					
					break;
				}
				default:
					break;
				}
			}
		}
	}
}