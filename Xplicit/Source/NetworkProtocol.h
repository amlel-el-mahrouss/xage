/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

#ifndef XPLICIT_NETWORK_PORT
#define XPLICIT_NETWORK_PORT (64734)
#endif // ifndef XPLICIT_NETWORK_PORT

#define XPLICIT_INVALID_NET_DATA (0xDEEBEFF)

#ifndef XPLICIT_ADDRESS_ANY
#define XPLICIT_ADDRESS_ANY INADDR_ANY
#define XPLICIT_ADDRESS_ANY_STR "0.0.0.0"
#endif

#define XPLICIT_NETWORK_MAG_0 ('X')
#define XPLICIT_NETWORK_MAG_1 ('C')
#define XPLICIT_NETWORK_MAG_2 ('P')

#define XPLICIT_NETWORK_MAG_COUNT (3U)
#define XPLICIT_NETWORK_CMD_MAX (30U)

#ifndef XPLICIT_INVALID_ADDR
#define XPLICIT_INVALID_ADDR INADDR_NONE
#endif

#define XPLICIT_NETWORK_BUF_SZ (128U)
#define XPLICIT_NETWORK_VERSION (1U)

#define XPLICIT_NUM_CHANNELS (2)

#define XPLICIT_CHANNEL_DATA (0)
#define XPLICIT_CHANNEL_CHAT (1)

/* Used by the protocol to tell the velocity. */

#define XPLICIT_NETWORK_X     (0)
#define XPLICIT_NETWORK_Y     (1)
#define XPLICIT_NETWORK_Z     (2)
#define XPLICIT_NETWORK_DELTA (3)

/*! max data inside speed field. */
#define XPLICIT_NETWORK_SPEED_MAX (4)

#define XPLICIT_SOCKET_ERROR  (-1)

#define XPLICIT_BAD_ADDRESS (XPLICIT_INVALID_ADDR)

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
        NETWORK_CMD_BEGIN = 500, // start network, handshake
        NETWORK_CMD_STOP, // abort connection
        NETWORK_CMD_DEAD,
        NETWORK_CMD_DAMAGE, // server to client, reports damage to the player.
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
        NETWORK_CMD_REPL, // replication call done
        NETWORK_CMD_MSG, // chat message
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
        NETWORK_STAT_INVALID = 255, // 0xFF
		NETWORK_STAT_CONNECTED = 400,
        NETWORK_STAT_DISCONNECTED = 800,
        NETWORK_STAT_COUNT = 2,
    };

    using NetworkFloat = float;

	class XPLICIT_API NetworkPacket final
	{
	public:
		char                  magic[XPLICIT_NETWORK_MAG_COUNT];
		Xplicit::NetworkFloat speed[XPLICIT_NETWORK_SPEED_MAX];
        char                  buffer[XPLICIT_NETWORK_BUF_SZ];
		std::int16_t          cmd[XPLICIT_NETWORK_CMD_MAX];
		std::int64_t          public_hash;
        std::int8_t           channel;
		std::int32_t          version;
		std::int64_t          health;
		std::int64_t          hash;
		std::size_t           size;
		std::int32_t          id;

	};

    class XPLICIT_API NetworkInstance final
    {
    public:
        class XPLICIT_API UniqueAddress final
        {
        public:
            UniqueAddress();
            ~UniqueAddress();
            
			XPLICIT_COPY_DEFAULT(UniqueAddress);
            
			const uuids::uuid& get() noexcept { return mUuid; }
            
			void invalidate() noexcept
            {
                mUuid = UUIDFactory::version<4>(); 
                mPublicUuid = UUIDFactory::version<4>(); 
            }
            
			const UUID& get_public_uuid() noexcept { return this->get(); }
			const UUID& get_private_uuid() noexcept { return mUuid; }

        private:
			UUID mPublicUuid; /* public peer uuid */
            UUID mUuid; /* private peer uuid */

            friend NetworkInstance;

        };

        PrivateAddressData address;
		UniqueAddress unique_addr;
        std::uint32_t channel;
        NetworkPacket packet;
        int64_t public_hash; 
		NETWORK_STAT status;
		std::uint32_t port;
        String str_address;
        int64_t hash;
 
        explicit NetworkInstance();
        ~NetworkInstance();
        
        XPLICIT_COPY_DEFAULT(NetworkInstance);
        
        void reset() noexcept
		{
			for (size_t cmd_index = 0; cmd_index < XPLICIT_NETWORK_CMD_MAX; ++cmd_index)
			{
				packet.cmd[cmd_index] = NETWORK_CMD_INVALID;
			}

            str_address.clear();
			
            status = NETWORK_STAT_INVALID;
			hash = XPLICIT_INVALID_NET_DATA;

			public_hash = -1;
		}

    };

    XPLICIT_API bool equals(PrivateAddressData& lhs, PrivateAddressData& rhs);
}

#ifdef XPLICIT_WINDOWS
typedef int socklen_t;
#endif // XPLICIT_WINDOWS

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
#define XPLICIT_NETWORK_CMD_REPL (15)
#define XPLICIT_NETWORK_CMD_CHAT (16)

/*! this is a command reserved by the engine, 
  ! to separate the network and replication commands 
  !*/

#define XPLICIT_LAST_RESERVED_CMD (XPLICIT_NETWORK_CMD_CHAT + 1)

#define XPLICIT_REPL_CREATE (18)
#define XPLICIT_REPL_DESTROY (19)
#define XPLICIT_REPL_UPDATE (20)
#define XPLICIT_REPL_TOUCH (21)
#define XPLICIT_REPL_CLICK (22)
#define XPLICIT_REPL_DBL_CLICK (23)

namespace Xplicit::Utils
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