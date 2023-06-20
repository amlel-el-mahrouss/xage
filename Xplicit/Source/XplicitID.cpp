/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#include "XplicitID.h"

namespace Xplicit::Auth
{
	XplicitID::XplicitID(const int32_t& universe, const int32_t& connectionPublicHash) noexcept
		: mRegionId(universe), mConnectionHash(connectionPublicHash), mXplicitId("")
	{
		mXplicitId = XPLICIT_UNIVERSE_PREFIX;
		mXplicitId += std::move(std::to_string(mRegionId));
		mXplicitId += XPLICIT_UNIVERSE_DELIM;
		mXplicitId += std::move(std::to_string(mConnectionHash));
	}

	const String& XplicitID::as_string() noexcept
	{
		return mXplicitId;
	}

	const std::int64_t& XplicitID::get() noexcept
	{
		static const std::int64_t id = mRegionId | mConnectionHash;

		/* return the complete id */
		return id;
	}

	const bool XplicitID::contains(std::int32_t bytes) noexcept
	{
		return (bytes & mRegionId);
	}
}