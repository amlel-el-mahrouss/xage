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
	constexpr const std::int16_t XPLICIT_MAX_CONNECTIONS = 16;

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
		const std::uint16_t port() const noexcept;
		const char* dns() const noexcept;
		size_t size() const noexcept;

	private:
		NetworkVector mPeers;
		std::uint16_t mPort;
		Socket mSocket;
		String mDns;

	private:
		friend class NetworkServerContext;

	};

	class XPLICIT_API NetworkServerContext final
	{
		// try slots.
	public:
		static void send_all(const NetworkServerComponent* server) noexcept;
		static void recv_all(const NetworkServerComponent* server) noexcept;
		
		static void recv_from(const NetworkServerComponent* server, NetworkInstance* peer, NetworkPacket& packet) noexcept;
		static void recv(const NetworkServerComponent* server, NetworkInstance* peer) noexcept;
		static void send(const NetworkServerComponent* server, NetworkInstance* peer) noexcept;
		
	};

	class XPLICIT_API NetworkPacketQueue final
	{
		NetworkPacketQueue() = default;

	public:
		~NetworkPacketQueue() = default;

	public:
		XPLICIT_COPY_DEFAULT(NetworkPacketQueue);

	public:
		static constexpr std::size_t size = 1024;

	public:
		static NetworkPacketQueue* get_singleton_ptr() noexcept
		{
			static NetworkPacketQueue* queue = nullptr;
			return queue;
		}

		NetworkPacket& push() noexcept
		{
			if (mQueueSize > size)
				throw EngineError("Max packets reached.");

			if (mQueue[mQueueSize] == nullptr)
			{
				const auto ptr = mQueue[mQueueSize] = create();
				++mQueueSize;

				return *ptr;
			}
			else
			{
				memset(mQueue[mQueueSize], 0, sizeof(NetworkPacket));
				return *mQueue[mQueueSize];
			}
		}

		void pop() noexcept
		{
			--mQueueSize;
		}

	private:
		NetworkPacket* create() noexcept
		{
			return mPackets.allocate();
		}

		void deallocate(NetworkPacket* packet) noexcept
		{
			if (packet)
				mPackets.deallocate(packet);

			packet = nullptr;
		}
		
		Pool<NetworkPacket, size> mPackets;
		NetworkPacket* mQueue[size];
		std::size_t mQueueSize;
		
	};
}