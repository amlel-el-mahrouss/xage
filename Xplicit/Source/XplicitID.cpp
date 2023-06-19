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
		: mRegionId(0), mConnectionHash(hash)
	{
		mRegionId |= region;
		mRegionId |= id;
	}

	const String XplicitID::as_string() noexcept
	{
		String id = "XPLICIT_";
		id += std::to_string(mRegionId);
		id += ":";
		id += std::to_string(mConnectionHash);
	}

	const std::int64_t XplicitID::get() noexcept
	{
		return mRegionId | mConnectionHash;
	}
}