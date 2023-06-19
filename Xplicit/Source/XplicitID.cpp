/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: NetworkComponent.h
 *			Purpose: XDP Protocol Client
 *
 * =====================================================================
 */

#include "XplicitID.h"

namespace Xplicit::Auth
{
	constexpr const char XPLICIT_DELIM = ':';

	XplicitID::XplicitID(const int32_t& universe, const int32_t& playerId, const int64_t& hash) noexcept
		: mRegionId(0), mConnectionHash(hash), mXplicitId("")
	{
		mRegionId |= universe;
		mRegionId |= playerId;

		mXplicitId = "XPLICITNGIN_";
		mXplicitId += std::move(std::to_string(mRegionId));
		mXplicitId += ":";
		mXplicitId += std::move(std::to_string(mConnectionHash));
	}

	const String XplicitID::as_string() noexcept
	{
		return mXplicitId;
	}

	const std::int64_t XplicitID::get() noexcept
	{
		return mRegionId | mConnectionHash;
	}
}