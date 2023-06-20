/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

/* XplicitNgin first universe */
#define XPLICIT_UNIVERSE_ID (1)
#define XPLICIT_UNIVERSE_DELIM ":"
#define XPLICIT_UNIVERSE_PREFIX "XPLICIT_"

namespace Xplicit::Auth
{
	class XPLICIT_API XplicitID final
	{
	public:
		XplicitID() = delete;

	public:
		explicit XplicitID(
			const int32_t& universe, 
			const int32_t& connectionPublicHash) noexcept;

		~XplicitID() = default;

	public:
		XPLICIT_COPY_DEFAULT(XplicitID);

	public:
		const std::int64_t& get() noexcept;
		const String& as_string() noexcept;

	private:
		std::int64_t mConnectionID;
		std::int64_t mVersion;

	};
}