/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: NetworkInstance.h
 *			Purpose: XPX Protocol Client
 *
 * =====================================================================
 */

#pragma once

#include "NetworkProtocol.h"
#include "Instance.h"
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

    class XPLICIT_API NetworkInstance : public Instance
    {
    public:
        NetworkInstance();

        virtual ~NetworkInstance();

        NetworkInstance& operator=(const NetworkInstance& ctx) = default;
        NetworkInstance(const NetworkInstance& ctx) = default;

        virtual const char* name() noexcept override { return ("NetworkInstance"); }
        virtual INSTANCE_TYPE type() noexcept override { return INSTANCE_NETWORK; }

        virtual void update() override;

        bool connect(const char* ip);
        bool send(NetworkPacket& packet);
        bool read(NetworkPacket& packet);

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