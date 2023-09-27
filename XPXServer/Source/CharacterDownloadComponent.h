/*
 * =====================================================================
 *
 *			XPXServer
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "ServerConfig.h"
#include "CharacterComponent.h"
#include "NetworkUtils.h"

namespace XPX
{
	class CharacterDownloadComponent final : public Component
	{
	public:
		explicit CharacterDownloadComponent();
		~CharacterDownloadComponent() override;

	public:
		CharacterDownloadComponent& operator=(const CharacterDownloadComponent&) = default;
		CharacterDownloadComponent(const CharacterDownloadComponent&) = default;

	public:
		const char* name() noexcept override;

		static bool should_update() noexcept;
		static void update(ClassPtr ptr);

	public:
		const size_t& size() const noexcept;

	private:
		std::vector<CharacterComponent*> mPlayers;
		NetworkFileTransferFactory mFactory;
		NetworkServerComponent* mNetwork;
		std::size_t mPlayerCount;

	};
}