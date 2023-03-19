/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: SocketWrapper.h
 *			Purpose: C++ Socket Wrapper
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

#ifdef XPLICIT_WINDOWS

#define XPLICIT_CLOSE closesocket
#define XPLICIT_SHUTDOWN shutdown

#else

#define XPLICIT_CLOSE close
#define XPLICIT_SHUTDOWN shutdown

#endif // XPLICIT_WINDOWS


namespace Xplicit::Details
{
	enum class SOCKET_TYPE
	{
		TCP,
		UDP,
		COUNT,
	};

	using CSocket = uintptr_t;

	class XPLICIT_API Socket final
	{
	public:
		Socket(const SOCKET_TYPE type)
		{
			_Socket = socket(AF_INET,
				type == SOCKET_TYPE::TCP ? SOCK_STREAM : SOCK_DGRAM,
				type == SOCKET_TYPE::TCP ? IPPROTO_TCP : IPPROTO_UDP);

			XPLICIT_ASSERT(_Socket != SOCKET_ERROR);
		}

		~Socket()
		{
			if (XPLICIT_SHUTDOWN(_Socket, SD_BOTH))
				XPLICIT_CLOSE(_Socket);
		}

		Socket& operator=(const Socket&) = default;
		Socket(const Socket&) = default;

		CSocket _Socket;

		operator bool()
		{
#ifdef XPLICIT_WINDOWS
			return _Socket != SOCKET_ERROR;
#else
#	error SOCKET_ERROR is undefined
#endif // XPLICIT_WINDOWS
		}

	};
}