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
	XplicitID::XplicitID(const std::int64_t& universe,
						 const std::int64_t& id) noexcept
		: mRegionId(universe), mConnectionID(id), mXplicitId("")
	{
		this->generate(id);
	}

	const String& XplicitID::as_string() noexcept
	{
		return mXplicitId;
	}

	const std::int64_t& XplicitID::as_int() const noexcept
	{
		static const std::int64_t id = mRegionId | mConnectionID;
		return id;
	}

	void XplicitID::generate(const std::int64_t& id) noexcept
	{
		if (id == -1)
			return;

		mConnectionID = id;

		if (mConnectionID < 0)
			mConnectionID = -mConnectionID;

		mXplicitId = XPLICIT_UNIVERSE_PREFIX;
		mXplicitId += std::move(std::to_string(mRegionId));
		mXplicitId += XPLICIT_UNIVERSE_DELIM;
		mXplicitId += std::move(std::to_string(mConnectionID));
	}
}