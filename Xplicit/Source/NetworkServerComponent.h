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
	constexpr const std::int16_t XPLICIT_MAX_CONNECTIONS = 29;

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
		
		friend class NetworkServerContext;

	};

	class XPLICIT_API NetworkServerContext final
	{
		// try slots.
		static void try_recv(NetworkServerComponent* server, NetworkInstance* peer) noexcept;
		static void try_send(NetworkServerComponent* server, NetworkInstance* peer) noexcept;

	public:
		static void accept_send(NetworkServerComponent* server) noexcept;
		static void accept_recv(NetworkServerComponent* server) noexcept;
		
		static void try_correct(NetworkServerComponent* server) noexcept;
		static void send(NetworkServerComponent* server, NetworkInstance* peer) noexcept;
		static void recv(NetworkServerComponent* server, NetworkInstance* peer, NetworkPacket& packet) noexcept;
		
		enum class NETWORK_CONTEXT
		{
			ONLINE,
			DISCONNECTED,
			COUNT,
		};
		
	};
}