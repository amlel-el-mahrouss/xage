/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#include "LocalReplicationComponent.h"
#include "Application.h"
#include "GameMenuUI.h"

#include <CommonEngine.h>
#include <RoXML.h>
#include <CLua.hpp>
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

	void LocalReplicationComponent::update()
	{
		if (!mNetwork)
			return;

		NetworkPacket packet{};

		if (!mNetwork->read(packet))
			return;

		if (packet.cmd[XPLICIT_REPL_CREATE] == NETWORK_REPL_CMD_CREATE)
		{
			switch (packet.id)
			{

			case COMPONENT_ID_ROXML:
			{
				String name = packet.buffer;

				RoXML::RoXMLDocumentParameters params;
				params.Inline = true;
				params.Path = name;

				parser.load(params);

				if (name.empty() ||
					packet.hash != this->mHash)
				{
					if (!ComponentManager::get_singleton_ptr()->get<PopupComponent>("ConnBadChallengeRoXML"))
					{
						ComponentManager::get_singleton_ptr()->add<PopupComponent>([]()-> void {
							if (Bites::ObjectInputSystem::get_singleton_ptr()->key_down(VK_RETURN))
								RENDER->queueEndRendering();
							}, Ogre::Vector2(XPLICIT_DIM.X / 2.8,
								XPLICIT_DIM.Y / 2.8),
								POPUP_TYPE::CHALLENGE,
								"ConnBadChallengeRoXML");

					}

					ComponentManager::get_singleton_ptr()->remove(mNetwork);
				}

				break;
			}
			case COMPONENT_ID_SCRIPT:
			{
				auto script = Utils::UriParser(packet.buffer);
				script /= packet.buffer;

				if (script.protocol() != XPLICIT_XASSET_IDENT)
					return;

				// TODO: download stuff
				// String full_download_path = mMonitor->HTTP->download(script.get());
				// ComponentManager::get_singleton_ptr()->add<LuaScriptComponent>(full_download_path.c_str(), true);

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
				String name = packet.buffer;

				if (name.empty() ||
					packet.hash != this->mHash)
				{
					if (!ComponentManager::get_singleton_ptr()->get<PopupComponent>("ConnChallengeScript"))
					{
						ComponentManager::get_singleton_ptr()->add<PopupComponent>([]()-> void {
							if (Bites::ObjectInputSystem::get_singleton_ptr()->key_down(VK_RETURN))
								RENDER->queueEndRendering();
							}, Ogre::Vector2(XPLICIT_DIM.X / 2.8,
								XPLICIT_DIM.Y / 2.8),
								POPUP_TYPE::CHALLENGE,
								"ConnChallengeScript");

					}

					ComponentManager::get_singleton_ptr()->remove(mNetwork);
				}

				if (auto script = ComponentManager::get_singleton_ptr()->get<LuaScriptComponent>(name.c_str()))
				{
#ifdef XPLICIT_DEBUG
					XPLICIT_INFO("Removing Script with success!");
#endif

					ComponentManager::get_singleton_ptr()->remove(script);
				}

				break;
			}
			}
		}
	}
}