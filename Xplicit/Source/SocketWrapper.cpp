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
		this->m_iSocket = XPLICIT_SOCKET(AF_INET,
			type == SOCKET_TYPE::TCP ? SOCK_STREAM : SOCK_DGRAM,
			type == SOCKET_TYPE::TCP ? IPPROTO_TCP : IPPROTO_UDP);

		XPLICIT_ASSERT(m_iSocket != SOCKET_ERROR);
	}

	Socket::~Socket()
	{
		if (XPLICIT_SHUTDOWN(m_iSocket, SD_BOTH))
			XPLICIT_CLOSE(m_iSocket);
	}

	Socket::operator bool() noexcept
	{
		return m_iSocket != -1;
	}
}