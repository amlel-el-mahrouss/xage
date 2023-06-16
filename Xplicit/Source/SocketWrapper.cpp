/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
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
		this->PublicSocket = XPLICIT_SOCKET(AF_INET,
			type == SOCKET_TYPE::TCP ? SOCK_STREAM : SOCK_DGRAM,
			type == SOCKET_TYPE::TCP ? IPPROTO_TCP : IPPROTO_UDP);

		XPLICIT_ASSERT(this->PublicSocket != SOCKET_ERROR);
	}

	Socket::~Socket()
	{
		if (XPLICIT_SHUTDOWN(this->PublicSocket, SD_BOTH))
			XPLICIT_CLOSE(this->PublicSocket);
	}

	Socket::operator bool() noexcept
	{
		return this->PublicSocket != -1;
	}
}