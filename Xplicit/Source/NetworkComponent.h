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


#ifndef XPLICIT_SLEEP
#define XPLICIT_SLEEP Sleep
#endif

namespace Xplicit {
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
        NetworkError(const int what = NETERR_INTERNAL_ERROR) : std::runtime_error("XplicitNetwork Error") 
        { 
            m_iErr = WSAGetLastError(); 
            std::string err = "Error: ";
            err += std::to_string(m_iErr);
        }

        virtual ~NetworkError() = default; // let the A.B.I define that.

        NetworkError& operator=(const NetworkError&) = default;
        NetworkError(const NetworkError&) = default;

    public:
        int error() const noexcept { return m_iErr; }

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
        bool send(UDPNetworkPacket& packet);
        bool read(UDPNetworkPacket& packet);

        UDPNetworkPacket& get() noexcept;
        bool is_reset() noexcept;

    public:
        template <typename As>
        As* get_as(const int off = 0) noexcept
        {
            XPLICIT_ASSERT(*m_opt != 0);
            return reinterpret_cast<As*>(m_opt + off);
        }

    private:
        bool reset() noexcept;

    private:
        char m_opt[XPLICIT_NETWORK_OPT_SIZE];
        PrivateAddressData m_addr;
        UDPNetworkPacket m_packet;
        Socket m_socket;
        bool m_reset;

        friend class NetworkEvent;

    };
}