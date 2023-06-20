/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "NetworkComponent.h"
#include "XplicitID.h"
#include "Xplicit.h"
#include "Event.h"

namespace Xplicit
{
	constexpr const int XPLICIT_MAX_CONNECTIONS = 60;

	// the main instance behind the networking.
	class XPLICIT_API NetworkServerComponent final : public Component
	{
	public:
		explicit NetworkServerComponent() = default;

	public:
		explicit NetworkServerComponent(const char* ip);
		virtual ~NetworkServerComponent();

	public:
		XPLICIT_COPY_DEFAULT(NetworkServerComponent);

	public:
		virtual INSTANCE_TYPE type() noexcept;
		virtual const char* name() noexcept;
		virtual void update() override;
		const char* dns() noexcept;

	public:
		virtual bool should_update() noexcept override;

	public:
		size_t size() noexcept
		{
			return mPeers.size();
		}

		NetworkInstance* get(const std::size_t& idx) noexcept
		{
			return mPeers.at(idx).second;
		}

	private:
		std::vector<std::pair<Auth::XplicitID, NetworkInstance*>> mPeers;
		ENetAddress mXnetAddress;
		ENetHost* mXnetServer;
		String mAddress;
		Socket mSocket;

	private:
		friend class NetworkServerHelper;

	};
}