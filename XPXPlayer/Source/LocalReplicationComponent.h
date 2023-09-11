/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <Root.h>
#include <NetworkComponent.h>

namespace XPX
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

		static bool should_update() noexcept;
		static void update(void* class_ptr);
		
		XPLICIT_COPY_DEFAULT(LocalReplicationComponent);
		
	private:
		NetworkComponent* mNetwork;
		std::int64_t mHash;

	};
}
