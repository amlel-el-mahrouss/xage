/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: SocketWrapper.cpp
 *			Purpose: C++ Socket Wrapper
 *
 * =====================================================================
 */

#include "SocketWrapper.h"

namespace Xplicit::Network
{
	Socket::Socket(const SOCKET_TYPE type)
	{
		this->_Socket = XPLICIT_SOCKET(AF_INET,
			type == SOCKET_TYPE::TCP ? SOCK_STREAM : SOCK_DGRAM,
			type == SOCKET_TYPE::TCP ? IPPROTO_TCP : IPPROTO_UDP);

		XPLICIT_ASSERT(_Socket != SOCKET_ERROR);
	}

	Socket::~Socket()
	{
		if (XPLICIT_SHUTDOWN(_Socket, SD_BOTH))
			XPLICIT_CLOSE(_Socket);
	}
}