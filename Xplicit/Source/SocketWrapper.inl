/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: SocketWrapper.inl
 *			Purpose: C++ Socket Wrapper
 *
 * =====================================================================
 */

/// <summary>
/// Winsock ::send wrapper
/// </summary>
/// <typeparam name="Data">Data type</typeparam>
/// <param name="ptr">Data pointer</param>
template <typename Data, size_t Size>
void Xplicit::Network::Socket::send(Data ptr, size_t sz)
{
	::send(m_iSocket, (const char*)ptr, sz < 1 ? Size : sz, 0);
}

/// <summary>
/// Winsock ::recv wrapper
/// </summary>
/// <typeparam name="Data"></typeparam>
/// <param name="ptr"></param>
template <typename Data, size_t Size>
void Xplicit::Network::Socket::recv(Data ptr, size_t sz)
{
	::recv(m_iSocket, (char*)ptr, sz < 1 ? Size : sz, 0);
}