/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

#ifdef XPLICIT_WINDOWS

#define XPLICIT_SOCKET socket
#define XPLICIT_CLOSE closesocket
#define XPLICIT_SHUTDOWN shutdown
#define XPLICIT_ACCEPT accept
#define XPLICIT_CONNECT connect
#define XPLICIT_IOCTL ioctlsocket

#else

#define XPLICIT_SOCKET socket
#define XPLICIT_CLOSE close
#define XPLICIT_SHUTDOWN shutdown
#define XPLICIT_ACCEPT accept
#define XPLICIT_CONNECT connect
#define XPLICIT_IOCTL ioctl

#endif // XPLICIT_WINDOWS

namespace XPX
{
	enum class SOCKET_FLAG : std::int64_t
	{
		NON_BLOCKING = FIONBIO,
		ASYNC = FIOASYNC,
	};
}

namespace XPX::Network
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
		explicit Socket(const SOCKET_TYPE type);
		~Socket();

		Socket& operator=(const Socket&) = default;
		Socket(const Socket&) = default;

	public:
		operator bool() noexcept;

	public:
		template <typename Data, size_t Size = sizeof(Data)>
		void send(Data ptr, std::size_t sz = 0);

		template <typename Data, size_t Size = sizeof(Data)>
		void recv(Data ptr, std::size_t sz = 0);

	public:
		CSocket PublicSocket;


	};
}

#include "SocketWrapper.inl"