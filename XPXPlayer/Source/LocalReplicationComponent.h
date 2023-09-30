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

//!
//! Sample code for a replication manager.
//! Use it for any XAGE based project.
//! 

namespace XPX
{
	class LocalReplicationComponent final : public Component
	{
	public:
		LocalReplicationComponent() = delete;
		
	public:
		LocalReplicationComponent(const std::int64_t& hash /* PRIVATE NETWORK HASH */, const String& myId /* XPX LOBBY ID */);
		~LocalReplicationComponent() override = default;
		
	public:
		COMPONENT_TYPE type() noexcept override;
		const char* name() noexcept override;

		static bool should_update() noexcept;
		static void update(ClassPtr class_ptr);
		
	public:
		XPLICIT_COPY_DEFAULT(LocalReplicationComponent);
		
	private:
		NetworkComponent* mNetwork;
		std::int64_t mHash;
		String mXpxId;

	};
}
