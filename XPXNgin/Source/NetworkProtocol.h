/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "NginCore.h"
#include "XplicitID.h"

#define XPLICIT_INVALID_HASH (~0)

#ifndef XPLICIT_ADDRESS_ANY
#   define XPLICIT_ADDRESS_ANY INADDR_ANY
#   define XPLICIT_ADDRESS_ANY_STR "0.0.0.0"
#endif // ifndef XPLICIT_ADDRESS_ANY

#define XPLICIT_NETWORK_MAG_0 ('P')
#define XPLICIT_NETWORK_MAG_1 ('X')
#define XPLICIT_NETWORK_MAG_2 ('P')

#define XPLICIT_NETWORK_MAG_COUNT (3U)
#define XPLICIT_NETWORK_CMD_MAX (30U)

#ifndef XPLICIT_INVALID_ADDR
#   define XPLICIT_INVALID_ADDR INADDR_NONE
#endif // ifndef XPLICIT_INVALID_ADDR

#define XPLICIT_END_OF_REPL     (0x7F)

#define XPLICIT_NETWORK_BUF_SZ  (512U)
#define XPLICIT_NETWORK_VERSION (2U)

// applies to usernames and nodes.
#define XPLICIT_NETWORK_NAME_SZ (32)

#define XPLICIT_MAX_REPLICAS    (4)

#define XPLICIT_NUM_CHANNELS    (3)

//! Anything else.
#define XPLICIT_CHANNEL_DATA    (0)

//! Chat replication
#define XPLICIT_CHANNEL_CHAT    (1)

//! 3D world replication
#define XPLICIT_CHANNEL_3D      (2)

//! File transfer.
#define XPLICIT_CHANNEL_FT      (3)

///!
///!
///!

#define XPLICIT_REPLICA_1 (0)
#define XPLICIT_REPLICA_2 (1)
#define XPLICIT_REPLICA_3 (2)
#define XPLICIT_REPLICA_4 (3)

/* Used by the protocol to tell the velocity. */

#define XPLICIT_NETWORK_X     (0)
#define XPLICIT_NETWORK_Y     (1)
#define XPLICIT_NETWORK_Z     (2)
#define XPLICIT_NETWORK_DELTA (3)

/*! max data inside position field. */
#define XPLICIT_NETWORK_POS_MAX (4)

#define XPLICIT_SOCKET_ERROR  (-1)

#define XPLICIT_BAD_ADDRESS     (0xFFFFFFFF)
#define XPLICIT_BAD_ADDRESS_STR ("255.255.255.255")

#include "SocketWrapper.h"

namespace XPX
{
#ifdef XPLICIT_WINDOWS
    using PrivateAddressData = SOCKADDR_IN;
    using Socket = SOCKET;
#else
    typedef struct sockaddr_in PrivateAddressData;
    using Socket = Network::CSocket;
#endif

    // network commands.
    enum NETWORK_CMD : std::int16_t
    {
        NETWORK_CMD_BEGIN = 500, // start network, handshake
        NETWORK_CMD_STOP, // abort connection
        NETWORK_CMD_DEAD,
        NETWORK_CMD_DAMAGE, // Notify client with damage.
        NETWORK_CMD_SPAWN,
        NETWORK_CMD_FORWARD,
        NETWORK_CMD_JUMP,
        NETWORK_CMD_BACKWARD,
        NETWORK_CMD_LEFT,
        NETWORK_CMD_RIGHT,
        NETWORK_CMD_SHUTDOWN,
        NETWORK_CMD_POS,
        NETWORK_CMD_ACCEPT, // Handshake accept.
        NETWORK_CMD_BAN,
        NETWORK_CMD_ACK, // Acknowledge
        NETWORK_CMD_KICK, // also aborts the connection, and exits the client.
        NETWORK_CMD_CHAT, // replication call
        NETWORK_CMD_LCLICK,
        NETWORK_CMD_RCLICK,
        NETWORK_CMD_TARGET,
        NETWORK_CMD_SLOT,
        NETWORK_CMD_DOWNLOAD,
        NETWORK_CMD_DESTROY,
        NETWORK_CMD_CREATE,
        NETWORK_CMD_INVALID, // can be used to indicate an invalid or wrong state.
        NETWORK_CMD_COUNT = 20,
    };

    enum NETWORK_STAT : std::int16_t
    {
        NETWORK_STAT_INVALID      = 255, // 0xFF
		NETWORK_STAT_CONNECTED    = 400,
        NETWORK_STAT_DISCONNECTED = 800,
        NETWORK_STAT_STASIS       = 1200,
        NETWORK_STAT_COUNT        = 2,
    };

    using NetworkFloat = float;

    typedef NetworkFloat NetworkVec3[XPLICIT_NETWORK_POS_MAX];
    typedef char         NetworkReplica[XPLICIT_NETWORK_BUF_SZ];
    typedef char         NetworkChar;

    struct XPLICIT_API NetworkPacket final
    {
    // magic number.
    public:
        char                  magic[XPLICIT_NETWORK_MAG_COUNT];

    // packet and player information.
    public:
        std::int8_t           channel;
        std::int16_t          version;
        std::int16_t          health;
        std::int16_t          size;
        std::int16_t          id;

    // hash and public hash.
    public:
        std::int64_t          public_hash;
        std::int64_t          hash;

    // body.
    public:
        std::int16_t          cmd[XPLICIT_NETWORK_CMD_MAX];

        NetworkVec3           pos[XPLICIT_NETWORK_POS_MAX];
        NetworkReplica        replicas[XPLICIT_MAX_REPLICAS];
        NetworkChar           additional_data[XPLICIT_NETWORK_BUF_SZ];

    };

    class XPLICIT_API NetworkPeer final
    {
    public:
        class XPLICIT_API UniqueAddress final
        {
        public:
            explicit UniqueAddress();
            ~UniqueAddress();
            
        public:
			XPLICIT_COPY_DEFAULT(UniqueAddress);
            
        public:
            const UUID& get() noexcept;
            void invalidate() noexcept;
            
        public:
            const UUID& get_public_uuid() noexcept;
            const UUID& get_private_uuid() noexcept;

        private:
			UUID mPublicUuid; /* public peer uuid */
            UUID mUuid; /* private peer uuid */

            friend NetworkPeer;

        };

        PrivateAddressData address;
        Auth::XplicitID xplicit_id;
		UniqueAddress unique_addr;
        std::uint32_t channel;
        NetworkPacket packet;
        int64_t public_hash; 
		NETWORK_STAT status;
		std::uint32_t port;
        String ip_address;
        int64_t hash;
        
    public:
        explicit NetworkPeer(const XPX::Auth::XplicitID& id);
        ~NetworkPeer();
        
    public:
        XPLICIT_COPY_DEFAULT(NetworkPeer);

    public:
        void timeout() noexcept;
        void reset() noexcept;
        
    };

    XPLICIT_API NetworkPeer* find_peer(uuids::uuid& uuid);

    XPLICIT_API String address_to_string(NetworkPeer* instance);
    XPLICIT_API String address_to_string(const PrivateAddressData& ip);
    XPLICIT_API const bool equals(const PrivateAddressData& lhs, const PrivateAddressData& rhs);
}

#ifdef XPLICIT_WINDOWS
typedef int socklen_t;
#endif // ifdef XPLICIT_WINDOWS

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
#define XPLICIT_NETWORK_CMD_CREATE (15)
#define XPLICIT_NETWORK_CMD_CHAT (16)
#define XPLICIT_NETWORK_CMD_JUMP (17)
#define XPLICIT_NETWORK_CMD_LCLICK (18)
#define XPLICIT_NETWORK_CMD_RCLICK (19)
#define XPLICIT_NETWORK_CMD_TARGET (19)
#define XPLICIT_NETWORK_CMD_SLOT (20)

/*! this is a command reserved by the engine, 
  ! to separate the network and replication commands 
  !*/

#define XPLICIT_LAST_RESERVED_CMD (XPLICIT_NETWORK_CMD_SLOT + 1)

#define XPLICIT_NETWORK_CMD_DOWNLOAD (18)
#define XPLICIT_NETWORK_CMD_DESTROY (19)

namespace XPX::Utils
{
    class InternetProtocolChecker final
    {
    public:
        InternetProtocolChecker() = default;
        ~InternetProtocolChecker() = default;

    public:
        bool operator()(const char* ip) noexcept
        {
            if (!ip ||
                *ip == 0)
                return false;

            int cnter = 0;

            for (std::size_t base = 0; base < strlen(ip); ++base) 
            {
                if (ip[base] == '.')
                {
                    cnter = 0;
                }
                else
                {
                    if (cnter == 3)
                        return false;

                    ++cnter;
                }
            }

            return true;
        }

    };
}