/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: NetworkComponent.h
 *			Purpose: XDP Protocol Client
 *
 * =====================================================================
 */

#pragma once

#include "NetworkProtocol.h"
#include "Component.h"
#include "Event.h"

#ifdef XPLICIT_WINDOWS
#ifdef XPLICIT_DEBUG
#define XPLICIT_NET_ASSERT(EXPR) if (!(EXPR)) { MessageBoxA(nullptr, #EXPR, "XplicitNetwork Error", 0); std::terminate(); }
#else
#define XPLICIT_NET_ASSERT(EXPR) 
#endif
#else
#ifdef XPLICIT_DEBUG
#define XPLICIT_NET_ASSERT(EXPR) if (!(EXPR)) { std::string err = #EXPR; err += "C++ Network Exception"; XPLICIT_CRITICAL(err); std::terminate(); }
#else
#define XPLICIT_NET_ASSERT(EXPR) 
#endif
#endif

namespace Xplicit 
{
	enum NETWORK_ERR : int
	{
		NETERR_BAD_CHALLENGE,
		NETERR_PING_TOO_HIGH,
		NETERR_INTERNAL_ERROR,
		NETERR_BAN,
		NETERR_KICK,
		NETERR_COUNT,
	};

    class XPLICIT_API NetworkError final : public std::runtime_error 
    {
    public:
        NetworkError(const int what = NETERR_INTERNAL_ERROR);
        virtual ~NetworkError() = default; // let the A.B.I define that.

        XPLICIT_COPY_DEFAULT(NetworkError);

    public:
        int error() const noexcept;

    private:
        int m_iErr{ 200 };

    };

    class XPLICIT_API NetworkComponent final : public Component
    {
    public:
        NetworkComponent();

        virtual ~NetworkComponent();

        NetworkComponent& operator=(const NetworkComponent& ctx) = default;
        NetworkComponent(const NetworkComponent& ctx) = default;

        virtual const char* name() noexcept override { return ("NetworkComponent"); }
        virtual INSTANCE_TYPE type() noexcept override { return INSTANCE_NETWORK; }

        virtual void update() override;

        bool connect(const char* ip);
        bool send(NetworkPacket& packet, const size_t sz = sizeof(NetworkPacket));
        bool read(NetworkPacket& packet, const size_t sz = sizeof(NetworkPacket));

        NetworkPacket& get() noexcept;
        bool is_reset() noexcept;

    private:
        bool reset() noexcept;

    private:
        PrivateAddressData m_addr;
        NetworkPacket m_packet;
        Socket m_socket;
        bool m_reset;

        friend class NetworkEvent;

    };
}