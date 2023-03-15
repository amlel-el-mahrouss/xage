/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Uri.h
 *			Purpose: Xplicit URI library
 *
 * =====================================================================
 */

#pragma once

#include <Foundation.h>

#ifndef XPLICIT_URI_PROTOCOL
#define XPLICIT_URI_PROTOCOL ("xconnect://")
#endif // XPLICIT_URI_PROTOCOL

#ifndef XPLICIT_XDP_PROTOCOL
#define XPLICIT_XDP_PROTOCOL ("xdp://")
#endif // XPLICIT_XDP_PROTOCOL

namespace Xplicit::Utils
{
	class UriParser final
	{
	public:
		UriParser(const char* protocol);
		virtual ~UriParser();

		UriParser& operator=(const UriParser&) = default;
		UriParser(const UriParser&) = default;

		std::string get() noexcept;

	public:
		UriParser& operator/=(const std::string& uri);
		UriParser& operator/=(const char* uri);

	private:
		std::string m_protocol;
		std::vector<char> m_data;

	};

	class UriError final : public std::runtime_error
	{
	public:
		/* should allocated on the heap btw, anything on the stack can pop out.. */
		UriError(UriParser& uri) : std::runtime_error("UriError"), m_uri(uri) {}
		~UriError() = default; // let the ABI define that.

		UriError& operator=(const UriError&) = default;
		UriError(const UriError&) = default;

		UriParser& get() { return m_uri; }

	private:
		UriParser m_uri;

	};
}