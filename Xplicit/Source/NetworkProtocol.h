/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: NetworkProtocol.h
 *			Purpose: Game Network Protocol implementation
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

#ifndef XPLICIT_UDP_PORT
#define XPLICIT_UDP_PORT (60001)
#endif // ifndef XPLICIT_UDP_PORT

#ifndef XPLICIT_ADDRESS_ANY
#define XPLICIT_ADDRESS_ANY INADDR_ANY
#endif

#ifndef XPLICIT_ADDRESS_ANY_STR
#define XPLICIT_ADDRESS_ANY_STR "0.0.0.0"
#endif

#define XPLICIT_NETWORK_MAG_0 ('X')
#define XPLICIT_NETWORK_MAG_1 ('C')
#define XPLICIT_NETWORK_MAG_2 ('N')

#define XPLICIT_NETWORK_MAG_COUNT (3U)
#define XPLICIT_NETWORK_CMD_MAX (30U)

#ifndef XPLICIT_INVALID_ADDR
#define XPLICIT_INVALID_ADDR INADDR_NONE
#endif

#define XPLICIT_NETWORK_VERSION (7U)

/* Used by the protocol to tell the velocity. */

#define XPLICIT_NETWORK_X     (0)
#define XPLICIT_NETWORK_Y     (1)
#define XPLICIT_NETWORK_Z     (2)
#define XPLICIT_NETWORK_DELTA (3)

#define XPLICIT_SOCKET_ERROR  (-1)

namespace Xplicit
{
#ifdef XPLICIT_WINDOWS
    using PrivateAddressData = SOCKADDR_IN;
    using Socket = SOCKET;
#else
    typedef struct sockaddr_in PrivateAddressData;
    using Socket = socket;
#endif

    // network commands.
    enum NETWORK_CMD : int16_t
    {
        NETWORK_CMD_BEGIN = 100, // start network, handshake
        NETWORK_CMD_STOP, // abort connection
        NETWORK_CMD_DEAD,
        NETWORK_CMD_DAMAGE,
        NETWORK_CMD_SPAWN,
        NETWORK_CMD_FORWARD,
        NETWORK_CMD_BACKWARD,
        NETWORK_CMD_LEFT,
        NETWORK_CMD_RIGHT,
        NETWORK_CMD_SHUTDOWN,
        NETWORK_CMD_POS,
        NETWORK_CMD_ACCEPT, // handshake has been accepted.
        NETWORK_CMD_BAN,
        NETWORK_CMD_ACK, // acknowledge
        NETWORK_CMD_KICK, // also aborts the connection, and exits the client.
        NETWORK_CMD_INVALID, // can be used to indicate an invalid or wrong state.
        NETWORK_CMD_COUNT = 15,
    };

    /* replication network commands. */
    enum NETWORK_REPL_CMD : int16_t
    {
        NETWORK_REPL_CMD_CREATE = NETWORK_CMD_INVALID + 1,
        NETWORK_REPL_CMD_DESTROY,
        NETWORK_REPL_CMD_UPDATE,
        NETWORK_REPL_CMD_TOUCH,
        NETWORK_REPL_CMD_CLICK,
        NETWORK_REPL_CMD_DBL_CLICK,
        NETWORK_REPL_CMD_COUNT = 7,
    };

    enum NETWORK_STAT : int16_t
    {
		NETWORK_STAT_CONNECTED = 7,
        NETWORK_STAT_DISCONNECTED = 8,
        NETWORK_STAT_COUNT = 2,
    };

    /* network float type */
    using nfloat = float;

    /*
        hash = player communication id
        id = replication id.
        size = size of current packet
    */
	class XPLICIT_API NetworkPacket
	{
	public:
        char8_t magic[XPLICIT_NETWORK_MAG_COUNT];
		std::int16_t cmd[XPLICIT_NETWORK_CMD_MAX];
		std::int32_t version;

	public:
        std::int64_t public_hash; /* Public hash being sent (SHARED) */
        std::int64_t delta; /* delta time from the server. */
        std::int64_t hash; /* the private hash (CLIENT to SERVER) */
        std::size_t size; /* size of current packet. */
        std::int32_t id; /* component id */

	public:
        std::int64_t health; /* Player's Health 0-100 */
        Xplicit::nfloat speed[4]; /* Player's speed (X, Y, Z, Delta) */

	};

    class XPLICIT_API NetworkPeer final
    {
    public:
        class XPLICIT_API UniqueAddress final
        {
        public:
            UniqueAddress();
            ~UniqueAddress();

        public:
			XPLICIT_COPY_DEFAULT(UniqueAddress);

        public:
			const uuids::uuid& get() noexcept { return mUuid; }

			const uuids::uuid& get_public_uuid() noexcept { return this->get(); }
			const uuids::uuid& get_private_uuid() noexcept { return mUuid; }

        private:
            uuids::uuid mPublicUuid; /* for public hash */
            uuids::uuid mUuid; /* private hash */

        private:
            std::string mName;

            friend NetworkPeer;

        };

    public:
        UniqueAddress unique_addr; /* unique network address of this peer */
        PrivateAddressData addr; /* current socket address. */
        NetworkPacket packet; /* current network packet. */
        int64_t public_hash; /* Public hash, for other clients */
        NETWORK_STAT stat; /* current network status */
        int64_t hash; /* connection hash. */
 
    public:
        NetworkPeer();
        ~NetworkPeer();

    public:
        XPLICIT_COPY_DEFAULT(NetworkPeer);

    public:
        bool operator==(const NetworkPeer& cl);
        bool operator!=(const NetworkPeer& cl);

        void reset() noexcept;

    };

    XPLICIT_API bool equals(PrivateAddressData& lhs, PrivateAddressData& rhs);
}

#ifdef XPLICIT_WINDOWS
typedef int socklen_t;
#endif // XPLICIT_WINDOWS

// reserved network command slots

#define XPLICIT_NETWORK_CMD_FORWARD (0)
#define XPLICIT_NETWORK_CMD_BACKWARD (1)
#define XPLICIT_NETWORK_CMD_LEFT (2)
#define XPLICIT_NETWORK_CMD_RIGHT (3)
#define XPLICIT_NETWORK_CMD_BEGIN (4)
#define XPLICIT_NETWORK_CMD_STOP (5)
#define XPLICIT_NETWORK_CMD_ACK (6)
#define XPLICIT_NETWORK_CMD_BAN (7)
#define XPLICIT_NETWORK_CMD_KICK (8)
#define XPLICIT_NETWORK_CMD_ACCEPT (9)
#define XPLICIT_NETWORK_CMD_POS (10)
#define XPLICIT_NETWORK_CMD_DEAD (11)
#define XPLICIT_NETWORK_CMD_SPAWN (12)
#define XPLICIT_NETWORK_CMD_DAMAGE (13)
#define XPLICIT_NETWORK_CMD_SHUTDOWN (14)

// basically the last command reserved.
#define XPLICIT_LAST_RESERVED_CMD (XPLICIT_NETWORK_CMD_JUMP + 1)

#define XPLICIT_REPL_CREATE (16)
#define XPLICIT_REPL_DESTROY (17)
#define XPLICIT_REPL_UPDATE (18)
#define XPLICIT_REPL_TOUCH (19)
#define XPLICIT_REPL_CLICK (20)
#define XPLICIT_REPL_DBL_CLICK (21)
