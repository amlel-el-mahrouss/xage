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
#define XPLICIT_UNIVERSE_ID (1U)
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

		const bool contains(std::int32_t bytes) noexcept;

	private:
		Xplicit::String mXplicitId;

	private:
		std::int64_t mConnectionHash;
		std::int64_t mRegionId;

	};
}