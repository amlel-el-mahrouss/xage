/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: NetworkServerComponent.h
 *			Purpose: XDP Server
 *
 * =====================================================================
 */

#pragma once

#include "NetworkComponent.h"
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
		NetworkPeer* get(const std::size_t& idx) noexcept;
		size_t size() noexcept;

	private:
		std::vector<NetworkPeer*> mPeers;
		PrivateAddressData mPrivate;
		std::string mAddress;
		Socket mSocket;

	private:
		friend class NetworkServerHelper;

	};

	class XPLICIT_API NetworkServerHelper final
	{
	public:
		static void send(NetworkServerComponent* server, const size_t sz = sizeof(NetworkPacket));
		static void recv(NetworkServerComponent* server, const size_t sz = sizeof(NetworkPacket));
		static void correct(NetworkServerComponent* server);

	};
}