/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright PlayXPlicit, all rights reserved.
 *
 * =====================================================================
 */

#include "LocalReplicationComponent.h"
#include "LocalNetworkMonitorEvent.h"
#include "LuaScriptComponent.h"
#include "GearComponent.h"
#include "Application.h"
#include "MenuUI.h"

#include <CommonEngine.h>
#include <CLua/CLua.hpp>
#include <RoXML.h>
#include <Util.h>
#include <Uri.h>

#ifndef XPLICIT_XASSET_IDENT
#	define XPLICIT_XASSET_IDENT ("xasset")
#endif // ifndef XPLICIT_XASSET_IDENT

Xplicit::RoXML::RoXMLDocumentParser parser;

namespace Xplicit::Player
{
	COMPONENT_TYPE LocalReplicationComponent::type() noexcept { return COMPONENT_REPLICATION; }

	const char* LocalReplicationComponent::name() noexcept { return "LocalReplicationManager"; }

	bool LocalReplicationComponent::should_update() noexcept { return true; }

	void LocalReplicationComponent::update(ClassPtr class_ptr)
	{
		LocalReplicationComponent* self = (LocalReplicationComponent*)class_ptr;

		if (!self->mNetwork)
			return;

		NetworkPacket packet;
		self->mNetwork->read(packet);

		if (packet.cmd[XPLICIT_REPL_CREATE] == NETWORK_REPL_CMD_CREATE)
		{
			switch (packet.id)
			{
			case COMPONENT_ID_SCRIPT:
			{
				String url;

				for (size_t i = 0; i < XPLICIT_MAX_REPLICA_SLOTS; i++)
				{
					url = packet.replicas[i];

					if (url.empty() ||
						url.find(XPLICIT_XASSET_IDENT) == String::npos)
						continue;
				}

				if (url.empty() ||
					url.find(XPLICIT_XASSET_IDENT) == String::npos)
					return;

				String substr = url.erase(url.find(XPLICIT_XASSET_IDENT), strlen(XPLICIT_XASSET_IDENT) + 3);

				url.clear();
				url = "/";
				url += substr;

				static LocalNetworkMonitorEvent* monitor = EventSystem::get_singleton_ptr()->get<LocalNetworkMonitorEvent>("LocalNetworkMonitorEvent");

				if (!monitor)
					monitor = EventSystem::get_singleton_ptr()->get<LocalNetworkMonitorEvent>("LocalNetworkMonitorEvent");

				String endpoint = XPLICIT_XASSET_ENDPOINT;
				monitor->HTTP->set_endpoint(endpoint);

				auto tmp = std::to_string(xplicit_get_epoch()) + "-tmp.lua";

				if (monitor &&
					monitor->HTTP->download(url, tmp))
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

				for (size_t i = 0; i < XPLICIT_MAX_REPLICA_SLOTS; i++)
				{
					url = packet.replicas[i];

					if (url.empty() ||
						url.find(XPLICIT_XASSET_IDENT) == String::npos)
						continue;
				}

				if (url.empty() ||
					url.find(XPLICIT_XASSET_IDENT) == String::npos)
					return;

				String substr = url.erase(url.find(XPLICIT_XASSET_IDENT), strlen(XPLICIT_XASSET_IDENT) + 3);

				url.clear();
				url = "/";
				url += substr;

				static LocalNetworkMonitorEvent* monitor = EventSystem::get_singleton_ptr()->get<LocalNetworkMonitorEvent>("LocalNetworkMonitorEvent");

				if (!monitor)
					monitor = EventSystem::get_singleton_ptr()->get<LocalNetworkMonitorEvent>("LocalNetworkMonitorEvent");

				String endpoint = XPLICIT_XASSET_ENDPOINT;
				monitor->HTTP->set_endpoint(endpoint);

				auto tmp = std::to_string(xplicit_get_epoch()) + "-tmp.roxml";

				if (monitor &&
					monitor->HTTP->download(url, tmp))
				{
					XPLICIT_GET_DATA_DIR(full_path);

					String full_download_path;

					full_download_path += full_path;
					full_download_path += "Contents/";
					full_download_path += tmp;

					RoXML::RoXMLDocumentParameters params;

					params.Inline = false;
					params.Path = tmp;

					parser.parse(params);
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

				for (size_t i = 0; i < XPLICIT_MAX_REPLICA_SLOTS; i++)
				{
					name = packet.replicas[i];

					if (name.empty())
						continue;
				}

				if (name.empty())
					return;

				if (auto script = ComponentSystem::get_singleton_ptr()->get<LuaScriptComponent>(name.c_str()))
				{
#ifdef XPLICIT_DEBUG
					XPLICIT_INFO("Removing Script with success!");
#endif

					ComponentSystem::get_singleton_ptr()->remove(script);
				}

				break;
			}
			}
		}
	}
}