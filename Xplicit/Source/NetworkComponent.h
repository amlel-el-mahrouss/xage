/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
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
		NETWORK_ERR_BAD_CHALLENGE,
		NETWORK_ERR_PING_TOO_HIGH,
		NETWORK_ERR_INTERNAL_ERROR,
		NETWORK_ERR_BAN,
		NETWORK_ERR_KICK,
		NETWORK_ERR_COUNT,
	};

    class XPLICIT_API NetworkError final : public std::runtime_error 
    {
    public:
        NetworkError(const int what = NETWORK_ERR_INTERNAL_ERROR);
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

    public:
        bool connect(const char* ip);

    public:
        bool send(NetworkPacket& packet, const std::size_t sz = sizeof(NetworkPacket));
        bool read(NetworkPacket& packet, const std::size_t sz = sizeof(NetworkPacket));

        NetworkPacket& get() noexcept;
        bool is_reset() noexcept;

    private:
        bool reset() noexcept;

    private:
		PrivateAddressData mAddr;
		NetworkPacket mPacket;
        Socket mSocket;
        bool mReset;

        friend class NetworkEvent;

    };
}