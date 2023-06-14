/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: SocketWrapper.h
 *			Purpose: C++ Socket Wrapper
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

#ifdef XPLICIT_WINDOWS

#define XPLICIT_SOCKET socket
#define XPLICIT_CLOSE closesocket
#define XPLICIT_SHUTDOWN shutdown

#else

#define XPLICIT_SOCKET socket
#define XPLICIT_CLOSE close
#define XPLICIT_SHUTDOWN shutdown

#endif // XPLICIT_WINDOWS


namespace Xplicit::Network
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
		Socket(const SOCKET_TYPE type);
		~Socket();

		Socket& operator=(const Socket&) = default;
		Socket(const Socket&) = default;

		operator bool() noexcept;

		template <typename Data, size_t Size = sizeof(Data)>
		void send(Data ptr, size_t sz = 0);

		template <typename Data, size_t Size = sizeof(Data)>
		void recv(Data ptr, size_t sz = 0);

	public:
		CSocket m_iSocket;


	};
}

#include "SocketWrapper.inl"