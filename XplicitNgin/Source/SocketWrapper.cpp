/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright PlayXPlicit, all rights reserved.
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
				0);
		}
		else if (type == SOCKET_TYPE::UDP)
		{
			this->PublicSocket = XPLICIT_SOCKET(AF_INET,
				SOCK_DGRAM,
				0);
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