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
	constexpr const std::int16_t XPLICIT_MAX_CONNECTIONS = 30;

	typedef std::vector<std::pair<Auth::XplicitID, NetworkInstance*>> NetworkVector;

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
		virtual COMPONENT_TYPE type() noexcept;
		virtual const char* name() noexcept;
		virtual void update() override;
		const char* dns() noexcept;

	public:
		virtual bool should_update() noexcept override;

	public:
		NetworkInstance* get(const std::size_t& idx) noexcept;
		size_t size() noexcept;

	private:
		NetworkVector mPeers;
		Socket mSocket;
		String mDns;

	private:
		friend class NetworkServerHelper;

	};

	class XPLICIT_API NetworkServerHelper final
	{
	public:
		static void send_to(
			NetworkServerComponent* server,
			NetworkInstance* peer,
			const std::size_t sz = sizeof(NetworkPacket));

		static void send(NetworkServerComponent* server, const std::size_t sz = sizeof(NetworkPacket));

	public:
		static void recv_from(
			NetworkServerComponent* server,
			NetworkInstance* peer,
			const std::size_t sz = sizeof(NetworkPacket));

		static void recv(NetworkServerComponent* server, const std::size_t sz = sizeof(NetworkPacket));

	public:
		static void correct(NetworkServerComponent* server);

	};
}