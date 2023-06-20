/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
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

	public:
		explicit LocalReplicationComponent(const std::int64_t& hash)
			: 
			mNetwork(ComponentManager::get_singleton_ptr()->get_first<NetworkComponent>()),
			mHash(hash)
		{}

		virtual ~LocalReplicationComponent() = default;

	public:
		virtual COMPONENT_TYPE type() noexcept override;
		virtual const char* name() noexcept override;

		virtual bool should_update() noexcept override;

		virtual void update() override;

	public:
		XPLICIT_COPY_DEFAULT(LocalReplicationComponent);

	private:
		NetworkComponent* mNetwork;
		std::int64_t mHash;

	};
}
