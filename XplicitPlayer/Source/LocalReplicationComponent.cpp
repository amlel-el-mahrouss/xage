/*
 * =====================================================================
 *
 *			XplicitNgin
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

#include <CommonEngine.h>
#include <CLua/CLua.hpp>
#include <RoXML.h>
#include <Util.h>
#include <Uri.h>


namespace XPX
{
	static XPX::RoXML::RoXMLDocumentParser XPX_PARSER;
	static XHTTPManager XPX_HTTP;

	COMPONENT_TYPE LocalReplicationComponent::type() noexcept { return COMPONENT_REPLICATION; }

	const char* LocalReplicationComponent::name() noexcept { return "LocalReplicationManager"; }

	bool LocalReplicationComponent::should_update() noexcept { return true; }

	void LocalReplicationComponent::update(ClassPtr class_ptr)
	{
		LocalReplicationComponent* self = (LocalReplicationComponent*)class_ptr;

		if (!self->mNetwork)
			return;

		NetworkPacket packet = self->mNetwork->get();

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

				XPX_HTTP.set_endpoint("play-xplicit.com");

				if (XPX_HTTP.download(url, tmp))
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

				XPX_HTTP.set_endpoint("play-xplicit.com");

				if (XPX_HTTP.download(url, tmp))
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