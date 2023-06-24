/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "LocalReplicationComponent.h"

#include <CommonEngine.h>
#include <xlua.hpp>
#include <Uri.h>

namespace Xplicit::Player
{
	COMPONENT_TYPE LocalReplicationComponent::type() noexcept { return COMPONENT_REPLICATION; }

	const char* LocalReplicationComponent::name() noexcept { return "LocalReplicationManager"; }

	bool LocalReplicationComponent::should_update() noexcept { return true; }

	/*
	 *	This update function takes care of:
	 *		- replication events (create, remove components)
	 */

	void LocalReplicationComponent::update()
	{
		if (!mNetwork)
			return;
		
		NetworkPacket packet;

		if (!mNetwork->read(packet))
			return;
		
		if (packet.cmd[XPLICIT_REPL_CREATE] == NETWORK_REPL_CMD_CREATE)
		{
			switch (packet.id)
			{
			case COMPONENT_ID_SCRIPT:
			{
				Utils::UriParser script = Utils::UriParser(packet.buffer);
				
				if (script.protocol() != "xasset://")
					break;

				XPLICIT_GET_DATA_DIR(path);
				path += "Contents/";
				path += script.get();
				
				Lua::ILuaStateManager::get_singleton_ptr()->run(path.c_str());

				break;
			}
			case COMPONENT_ID_TEXTURE:
			{
				Utils::UriParser script = Utils::UriParser(packet.buffer);

				if (script.protocol() != "xasset://")
					break;

				auto texture = IRR->getVideoDriver()->getTexture(script.get().c_str());
				Collection.push_back(std::pair("ITexture_" + script.get(), reinterpret_cast<void*>(texture)));

				break;
			}
			case COMPONENT_ID_SOUND:
			{
				Utils::UriParser script = Utils::UriParser(packet.buffer);

				if (script.protocol() != "xasset://")
					break;
				
				break;
			}
			default:
				return;
			}

			packet.id = -1;
			packet.cmd[XPLICIT_REPL_CREATE] = NETWORK_CMD_INVALID;
		}
		else if (packet.cmd[XPLICIT_REPL_DESTROY] == NETWORK_REPL_CMD_DESTROY)
		{
			switch (packet.id)
			{
			case COMPONENT_ID_SCRIPT:
				break;
			case COMPONENT_ID_TEXTURE:
				break;
			case COMPONENT_ID_SOUND:
				break;
			default:
				return;
			}

			packet.id = -1;
			packet.cmd[XPLICIT_REPL_DESTROY] = NETWORK_CMD_INVALID;
		}
	}
}