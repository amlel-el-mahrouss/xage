/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Uri.h
 *			Purpose: Xplicit URI parser
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

#ifndef XPLICIT_XDP_PROTOCOL
#define XPLICIT_XDP_PROTOCOL ("xdp://")
#endif // XPLICIT_XDP_PROTOCOL

#ifndef XPLICIT_XCONNECT_PROTOCOL
#define XPLICIT_XCONNECT_PROTOCOL ("xconnect://")
#endif // XPLICIT_XDP_PROTOCOL

namespace Xplicit::Utils
{
	// Accessors are like iterators, but with an offset.
	using UriAccessor = int32_t;

	class XPLICIT_API UriParser final
	{
	public:
		UriParser(const char* protocol);
		virtual ~UriParser();

		UriParser& operator=(const UriParser&) = default;
		UriParser(const UriParser&) = default;

		std::string get(const UriAccessor offset = 0) noexcept;

	public:
		UriParser& operator/=(const std::string& uri);
		UriParser& operator/=(const char* uri);

	private:
		std::string m_protocol;
		std::vector<char> m_data;

	};

	class XPLICIT_API UriError final : public std::runtime_error
	{
	public:
		UriError(UriParser& uri) : std::runtime_error("UriError"), m_uri(uri) {}
		~UriError() = default; // let the ABI define that.

		UriError& operator=(const UriError&) = default;
		UriError(const UriError&) = default;

		UriParser& get();

	private:
		UriParser m_uri;

	};
}