/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Protocol.h
 *			Purpose: Lightwieght Game Network Protocol
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"
#include "ApplicationContext.h"

#ifndef XPLICIT_NETWORK_PORT
#define XPLICIT_NETWORK_PORT (60001)
#endif // ifndef XPLICIT_NETWORK_PORT

#ifndef XPLICIT_ADDRESS_ANY
#define XPLICIT_ADDRESS_ANY INADDR_ANY
#endif

#ifndef XPLICIT_ADDRESS_ANY_STR
#define XPLICIT_ADDRESS_ANY_STR "0.0.0.0"
#endif

#define XPLICIT_NETWORK_MAG_0 ('X')
#define XPLICIT_NETWORK_MAG_1 ('P')
#define XPLICIT_NETWORK_MAG_2 ('X')

#define XPLICIT_NETWORK_MAG_COUNT (3U)
#define XPLICIT_NETWORK_CMD_MAX (20U)

#ifndef XPLICIT_INVALID_ADDR
#define XPLICIT_INVALID_ADDR INADDR_NONE
#endif

namespace Xplicit 
{

#ifdef XPLICIT_WINDOWS
    using PrivateAddressData = SOCKADDR_IN;
    using Socket = SOCKET;
#else
    typedef struct sockaddr_in PrivateAddressData;
    using Socket = socket;
#endif

    enum NETWORK_CMD : int16_t
    {
        NETWORK_CMD_BEGIN = 100, // start network, handshake
        NETWORK_CMD_STOP, // abort connection
        NETWORK_CMD_DEAD,
        NETWORK_CMD_DAMAGE,
        NETWORK_CMD_SPAWN,
        NETWORK_CMD_FORWARD,
        NETWORK_CMD_BACKWARDS,
        NETWORK_CMD_LEFT,
        NETWORK_CMD_RIGHT,
        NETWORK_CMD_POS,
        NETWORK_CMD_ACCEPT, // handshake has been accepted.
        NETWORK_CMD_WATCHDOG,
        NETWORK_CMD_ACK, // acknowledge
        NETWORK_CMD_KICK, // also aborts the connection, and exits the client.
        NETWORK_CMD_INVALID, // can be used to indicate an invalid or wrong state.
        NETWORK_CMD_COUNT = 15,
    };

    enum NETWORK_STAT : int16_t
    {
        NETWORK_STAT_CONNECTED,
        NETWORK_STAT_DISCONNECTED,
        NETWORK_STAT_COUNT,
    };

    class NetworkPacket
    {
    public:
        char magic[XPLICIT_NETWORK_MAG_COUNT];
        NETWORK_CMD cmd[XPLICIT_NETWORK_CMD_MAX];

    public:
        int64_t public_hash; /* Public hash being sent */
        int64_t health; /* the health being sent (SERVER only) */
        int64_t hash; /* the private hash (SERVER only) */

    public:
        float X;
        float Y;
        float Z;

    public:
        float pitch;
        float yaw;
        float roll;

    };

    class XPLICIT_API NetworkPeer final
    {
    public:
        class XPLICIT_API UniqueAddress final
        {
        public:
            UniqueAddress()
                : uuid(UUIDFactory::version<4>()), name("Peer")
            {}

            ~UniqueAddress() = default;

            UniqueAddress& operator=(const UniqueAddress&) = default;
            UniqueAddress(const UniqueAddress&) = default;

            const uuids::uuid& get() noexcept { return uuid; }

        private:
            uuids::uuid public_uuid; /* Just here for saving purposes. */
            uuids::uuid uuid; /* Given to the connecting client */

            std::string name;

            friend NetworkPeer;

        };

    public:
        UniqueAddress unique_addr; /* unique network address of this peer */
        PrivateAddressData addr; /* current socket address. */
        NetworkPacket packet; /* current network packet. */
        int64_t public_hash; /* Public hash, for other clients */
        NETWORK_STAT stat; /* current network status */
        int64_t hash; /* connection hash. */
        bool bad; /* is the current peer bad, (has sent bad packets?) */

    public:
        NetworkPeer();
        ~NetworkPeer();

        NetworkPeer& operator=(const NetworkPeer&) = default;
        NetworkPeer(const NetworkPeer&) = default;

        bool operator==(const NetworkPeer& cl);
        bool operator!=(const NetworkPeer& cl);

    public:
        void reset() noexcept;

    };

    XPLICIT_API bool equals(PrivateAddressData& lhs, PrivateAddressData& rhs);
}

#ifdef XPLICIT_WINDOWS
typedef int socklen_t;
#endif // XPLICIT_WINDOWS

// reserved slots

#define XPLICIT_NETWORK_CMD_FORWARD (0)
#define XPLICIT_NETWORK_CMD_BACKWARD (1)
#define XPLICIT_NETWORK_CMD_LEFT (2)
#define XPLICIT_NETWORK_CMD_RIGHT (3)
#define XPLICIT_NETWORK_CMD_BEGIN (4)
#define XPLICIT_NETWORK_CMD_STOP (5)
#define XPLICIT_NETWORK_CMD_ACK (6)
#define XPLICIT_NETWORK_CMD_WATCHDOG (7)
#define XPLICIT_NETWORK_CMD_KICK (8)
#define XPLICIT_NETWORK_CMD_ACCEPT (9)
#define XPLICIT_NETWORK_CMD_POS (10)
#define XPLICIT_NETWORK_CMD_DEAD (11)
#define XPLICIT_NETWORK_CMD_SPAWN (12)
#define XPLICIT_NETWORK_CMD_DAMAGE (13)

#define XPLICIT_LAST_RESERVED_CMD XPLICIT_NETWORK_CMD_DAMAGE