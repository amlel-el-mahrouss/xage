/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "SocketWrapper.h"

namespace Xplicit::Network
{
	Socket::Socket(const SOCKET_TYPE type)
		: PublicSocket(SOCKET_ERROR)
	{
		if (type == SOCKET_TYPE::TCP)
		{
			this->PublicSocket = XPLICIT_SOCKET(AF_INET,
				SOCK_STREAM,
				IPPROTO_TCP);
		}
		else if (type == SOCKET_TYPE::UDP)
		{
			this->PublicSocket = XPLICIT_SOCKET(AF_INET,
				SOCK_DGRAM,
				IPPROTO_UDP);
		}

		XPLICIT_ASSERT(this->PublicSocket != SOCKET_ERROR);
	}

	Socket::~Socket()
	{
		if (XPLICIT_SHUTDOWN(this->PublicSocket, SD_BOTH))
			XPLICIT_CLOSE(this->PublicSocket);
	}

	Socket::operator bool() noexcept
	{
		return this->PublicSocket != SOCKET_ERROR;
	}
}