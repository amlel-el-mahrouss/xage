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
		
		explicit NetworkServerComponent(const char* ip);
		~NetworkServerComponent() override;
		
		XPLICIT_COPY_DEFAULT(NetworkServerComponent);
		
		COMPONENT_TYPE type() noexcept;
		const char* name() noexcept;
		void update()  override;

		bool should_update() noexcept override;
		
		NetworkInstance* get(const std::size_t& idx) const noexcept;
		const char* dns() const noexcept;
		size_t size() const noexcept;

	private:
		NetworkVector mPeers;
		Socket mSocket;
		String mDns;
		
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