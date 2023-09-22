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
	COMPONENT_TYPE LocalReplicationComponent::type() noexcept { return COMPONENT_REPLICATION; }

	const char* LocalReplicationComponent::name() noexcept { return "LocalReplicationManager"; }

	bool LocalReplicationComponent::should_update() noexcept { return true; }

	void LocalReplicationComponent::update(ClassPtr class_ptr)
	{
		LocalReplicationComponent* self = (LocalReplicationComponent*)class_ptr;

		if (!self ||
			!self->mNetwork)
			return;

		NetworkPacket packet = self->mNetwork->get();

		if (packet.channel == XPLICIT_CHANNEL_DATA)
		{
			if (packet.cmd[XPLICIT_NETWORK_CMD_DOWNLOAD] != NETWORK_CMD_DOWNLOAD)
				return;

			switch (packet.id)
			{
			case COMPONENT_ID_SCRIPT:
			{
				String url;

				url = packet.replicas[XPLICIT_REPLICA_PLAYER];

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

				url = packet.replicas[XPLICIT_REPLICA_PLAYER];

				if (url.empty() ||
					url.find("xasset://") == String::npos)
					return;

				auto tmp = uuids::to_string(UUIDFactory::version<4>()) + "-TMP-LUA";

				if (DownloadURL(url, tmp))
				{
					XPX::RoXML::RoXMLDocumentParser XPX_PARSER;

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
		else if (packet.channel == XPLICIT_CHANNEL_PHYSICS)
		{
			ISceneNode* node = CAD->getSceneManager()->getSceneNodeFromName(packet.additional_data);

			if (node)
			{
				if (packet.cmd[XPLICIT_NETWORK_CMD_DESTROY] == NETWORK_CMD_DESTROY)
				{
					node->drop();
					return;
				}

				std::cout << "SCALE:" << std::endl;

				std::cout << packet.pos_second[XPLICIT_NETWORK_X] << std::endl;
				std::cout << packet.pos_second[XPLICIT_NETWORK_Y] << std::endl;
				std::cout << packet.pos_second[XPLICIT_NETWORK_Z] << std::endl;

				std::cout << "POS:" << std::endl;

				std::cout << packet.pos[XPLICIT_NETWORK_X] << std::endl;
				std::cout << packet.pos[XPLICIT_NETWORK_Y] << std::endl;
				std::cout << packet.pos[XPLICIT_NETWORK_Z] << std::endl;

				node->setPosition(vector3df(packet.pos[XPLICIT_NETWORK_X], packet.pos[XPLICIT_NETWORK_Y], packet.pos[XPLICIT_NETWORK_Z]));
				node->setScale(vector3df(packet.pos_second[XPLICIT_NETWORK_X], packet.pos_second[XPLICIT_NETWORK_Y], packet.pos_second[XPLICIT_NETWORK_Z]));
				node->setRotation(vector3df(packet.pos_third[XPLICIT_NETWORK_X], packet.pos_third[XPLICIT_NETWORK_Y], packet.pos_third[XPLICIT_NETWORK_Z]));

				node->getMaterial(0).AmbientColor.setRed(packet.pos_fourth[XPLICIT_NETWORK_X]);
				node->getMaterial(0).AmbientColor.setGreen(packet.pos_fourth[XPLICIT_NETWORK_Y]);
				node->getMaterial(0).AmbientColor.setBlue(packet.pos_fourth[XPLICIT_NETWORK_Z]);

				node->getMaterial(0).AmbientColor.setAlpha(packet.pos_fourth[XPLICIT_NETWORK_DELTA]);

				node->getMaterial(0).DiffuseColor.setRed(packet.pos_fourth[XPLICIT_NETWORK_X]);
				node->getMaterial(0).DiffuseColor.setGreen(packet.pos_fourth[XPLICIT_NETWORK_Y]);
				node->getMaterial(0).DiffuseColor.setBlue(packet.pos_fourth[XPLICIT_NETWORK_Z]);

				node->getMaterial(0).DiffuseColor.setAlpha(packet.pos_third[XPLICIT_NETWORK_X]);
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
					parent = "world";
				}

				parent = parent.substr(parent.find(".") + 1);

				auto part = ComponentSystem::get_singleton_ptr()->add<PartComponent>(name.c_str(), parent.c_str());
				XPLICIT_ASSERT(part);
			}
		}
		else if (packet.channel == XPLICIT_CHANNEL_REPL_GEAR)
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
				parent = "world";
			}

			parent = parent.substr(parent.find(".") + 1);

			auto gears = ComponentSystem::get_singleton_ptr()->all_of<GearComponent>();

			for (auto* gear : gears)
			{
				if (gear &&
					gear->instance_name() == name &&
					gear->group_name() == parent)
				{
					auto node = gear->get_mesh()->node();

					if (!node)
					{
						gear->assign("Path", packet.replicas[XPLICIT_REPLICA_EVENT]);
						return;
					}

					node->setPosition(vector3df(packet.pos[XPLICIT_NETWORK_X], packet.pos[XPLICIT_NETWORK_Y], packet.pos[XPLICIT_NETWORK_Z]));
					node->setScale(vector3df(packet.pos_second[XPLICIT_NETWORK_X], packet.pos_second[XPLICIT_NETWORK_Y], packet.pos_second[XPLICIT_NETWORK_Z]));

					node->getMaterial(0).AmbientColor.setRed(packet.pos_fourth[XPLICIT_NETWORK_X]);
					node->getMaterial(0).AmbientColor.setGreen(packet.pos_fourth[XPLICIT_NETWORK_Y]);
					node->getMaterial(0).AmbientColor.setBlue(packet.pos_fourth[XPLICIT_NETWORK_Z]);

					node->getMaterial(0).AmbientColor.setAlpha(packet.pos_third[XPLICIT_NETWORK_X]);

					node->getMaterial(0).DiffuseColor.setRed(packet.pos_fourth[XPLICIT_NETWORK_X]);
					node->getMaterial(0).DiffuseColor.setGreen(packet.pos_fourth[XPLICIT_NETWORK_Y]);
					node->getMaterial(0).DiffuseColor.setBlue(packet.pos_fourth[XPLICIT_NETWORK_Z]);

					node->getMaterial(0).DiffuseColor.setAlpha(packet.pos_third[XPLICIT_NETWORK_X]);

					return;
				}
			}

			GearComponent* gear = ComponentSystem::get_singleton_ptr()->add<GearComponent>(name.c_str(), 
				packet.replicas[XPLICIT_REPLICA_EVENT], parent.c_str());

			XPLICIT_ASSERT(gear);
		}
	}
}