/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "LocalReplicationComponent.h"
#include "LocalNetworkMonitorEvent.h"
#include "GearComponent.h"
#include "Application.h"
#include "GameMenuUI.h"

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

	/*
	 *	This update function takes care of these 3 events:
	 *
	 *	 - Create
	 *   - Update
	 *   - Destroy
	 */

	void LocalReplicationComponent::update(void* class_ptr)
	{
		LocalReplicationComponent* _this = (LocalReplicationComponent*)class_ptr;

		if (!_this->mNetwork)
			return;

		NetworkPacket packet{};

		if (!_this->mNetwork->read(packet))
			return;

		if (packet.cmd[XPLICIT_REPL_CREATE] == NETWORK_REPL_CMD_CREATE)
		{
			switch (packet.id)
			{
			case COMPONENT_ID_GEAR:
			{
				String name = packet.buffer;
				GearComponent* gear = ComponentSystem::get_singleton_ptr()->add<GearComponent>(name.c_str(), "World");
				XPLICIT_ASSERT(gear);

				break;
			}
			case COMPONENT_ID_ROXML:
			{
				String name = packet.buffer;

				if (name.empty() ||
					packet.hash != _this->mHash)
				{
					if (!ComponentSystem::get_singleton_ptr()->get<PopupComponent>("ConnBadChallengeRoXML"))
					{
						ComponentSystem::get_singleton_ptr()->add<PopupComponent>([]()-> void {
							RENDER->closeDevice();
							}, POPUP_TYPE::CHALLENGE,
								"ConnBadChallengeRoXML");

					}

					ComponentSystem::get_singleton_ptr()->remove(_this->mNetwork);
				}

				RoXML::RoXMLDocumentParameters params;
				params.Inline = true;
				params.Path = name;

				parser.parse(params);

				break;
			}
			case COMPONENT_ID_SCRIPT:
			{
				String url = packet.buffer;

				if (url.empty() ||
					url.find(XPLICIT_XASSET_IDENT) == String::npos)
					return;

				String substr = url.erase(url.find(XPLICIT_XASSET_IDENT), strlen(XPLICIT_XASSET_IDENT) + 3);

				url.clear();
				url = "/";
				url += substr;

				std::cout << url << std::endl;

				static LocalNetworkMonitorEvent* monitor = EventSystem::get_singleton_ptr()->get<LocalNetworkMonitorEvent>("LocalNetworkMonitorEvent");
				
				if (!monitor)
					monitor = EventSystem::get_singleton_ptr()->get<LocalNetworkMonitorEvent>("LocalNetworkMonitorEvent");

				monitor->HTTP->set_endpoint(XPLICIT_XASSET_ENDPOINT);

				auto tmp = std::to_string(xplicit_get_epoch()) + "-tmp.lua";

				if (monitor &&
					monitor->HTTP->download(url, tmp))
				{
					XPLICIT_GET_DATA_DIR(full_path);

					String full_download_path;

					full_download_path += full_path;
					full_download_path += "Contents/";
					full_download_path += tmp;

					ComponentSystem::get_singleton_ptr()->add<LuaScriptComponent>(full_download_path.c_str(), true);
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
			case COMPONENT_ID_GEAR:
			{
				GearComponent* gear = ComponentSystem::get_singleton_ptr()->add<GearComponent>("GearComponent");
				ComponentSystem::get_singleton_ptr()->remove(gear);

				break;
			}
			case COMPONENT_ID_ROXML:
			{
				String name = packet.buffer;

				RoXML::RoXMLDocumentParameters params;
				params.Inline = true;
				params.Path = name;

				parser.parse(params);

				if (name.empty() ||
					packet.hash != _this->mHash)
				{
					if (!ComponentSystem::get_singleton_ptr()->get<PopupComponent>("ConnBadChallengeRoXML"))
					{
						ComponentSystem::get_singleton_ptr()->add<PopupComponent>([]()-> void {
							RENDER->closeDevice();
							}, POPUP_TYPE::CHALLENGE,
							"ConnBadChallengeRoXML");

					}

					ComponentSystem::get_singleton_ptr()->remove(_this->mNetwork);
				}

				break;
			}
			case COMPONENT_ID_SCRIPT:
			{
				String name = packet.buffer;

				if (name.empty() ||
					packet.hash != _this->mHash)
				{
					if (!ComponentSystem::get_singleton_ptr()->get<PopupComponent>("ConnChallengeScript"))
					{
						ComponentSystem::get_singleton_ptr()->add<PopupComponent>([]()-> void {
							RENDER->closeDevice();
							},POPUP_TYPE::CHALLENGE,
								"ConnChallengeScript");

					}

					ComponentSystem::get_singleton_ptr()->remove(_this->mNetwork);
				}

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