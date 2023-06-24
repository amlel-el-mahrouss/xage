/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: Uri.h
 *			Purpose: Xplicit URI parser
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

#ifndef XPLICIT_XASSET_PROTOCOL
#define XPLICIT_XASSET_PROTOCOL ("xasset://")
#endif // XPLICIT_XASSET_PROTOCOL

#ifndef XPLICIT_XCONNECT_PROTOCOL
#define XPLICIT_XCONNECT_PROTOCOL ("xconnect://")
#endif // XPLICIT_XCONNECT_PROTOCOL

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

		std::string port() noexcept;
		std::string protocol() noexcept;
		std::string get() noexcept;

	public:
		UriParser& operator/=(const std::string& uri);
		UriParser& operator/=(const char* uri);

	private:
		std::vector<char> m_data;
		std::string m_protocol;
		std::string m_port;

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