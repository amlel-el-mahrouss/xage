/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <Root.h>
#include <NetworkComponent.h>

namespace Xplicit::Player
{
	class LocalReplicationComponent final : public Component
	{
	public:
		LocalReplicationComponent() = delete;
		
		explicit LocalReplicationComponent(const std::int64_t& hash)
			: 
			mNetwork(ComponentSystem::get_singleton_ptr()->get<NetworkComponent>("NetworkComponent")),
			mHash(hash)
		{}

		~LocalReplicationComponent() override = default;
		
		COMPONENT_TYPE type() noexcept override;
		 const char* name() noexcept override;

		bool should_update() noexcept override;

		void update() override;
		
		XPLICIT_COPY_DEFAULT(LocalReplicationComponent);
		
	private:
		NetworkComponent* mNetwork;
		std::int64_t mHash;

	};
}
