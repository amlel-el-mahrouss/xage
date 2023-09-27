/*
 * =====================================================================
 *
 *			XPXServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "CharacterDownloadComponent.h"

namespace XPX
{
	CharacterDownloadComponent::CharacterDownloadComponent()
		: 
		mNetwork(ComponentSystem::get_singleton_ptr()->get<NetworkServerComponent>("NetworkServerComponent")),
		mPlayerCount(0UL)
	{}

	CharacterDownloadComponent::~CharacterDownloadComponent() = default;

	const size_t& CharacterDownloadComponent::size() const noexcept { return mPlayerCount; }

	const char* CharacterDownloadComponent::name() noexcept { return "CharacterReplicationComponent"; }

	bool CharacterDownloadComponent::should_update() noexcept { return true; }

	void CharacterDownloadComponent::update(ClassPtr ptr)
	{
		CharacterDownloadComponent* self = (CharacterDownloadComponent*)ptr;

		if (!IsValidHeapPtr(self))
			return;

		self->mPlayers = ComponentSystem::get_singleton_ptr()->all_of<CharacterComponent>();

		for (auto& player : self->mPlayers)
		{
			if (!player->get_peer())
				continue;

			// xasset is a wrapper over http, it downloads any contents from play-xplicit.com.

			if (auto replica_xasset = player->get_class()->index_as_string("Content");
				!replica_xasset.empty())
			{
				self->mFactory.send(
					player->get_class()->index_as_number<int>("ContentType"),
					replica_xasset.c_str(),
					player->get_peer()->public_hash);
			
				// you wanna do that, so we avoid calling this guy again.

				player->get_class()->assign("ContentType", "-1");
				player->get_class()->assign("Content", "nil");
			}
		}
	}
}