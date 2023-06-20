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
	XplicitID::XplicitID(const int32_t& version, const int32_t& connectionPublicHash) noexcept
		: mVersion(version), mConnectionID(connectionPublicHash)
	{}

	//! Generates a new Xplicit ID for a specific peer.
	//! The Xplicit ID is done as follow
	//! XPLICIT_<VERSION>:<CONNECTION_ID>:<USER_UUID>
	
	const String& XplicitID::as_string() noexcept
	{
		Xplicit::String id;

		id = XPLICIT_UNIVERSE_PREFIX;
		id += std::move(std::to_string(mVersion));
		id += XPLICIT_UNIVERSE_DELIM;
		id += std::move(std::to_string(mConnectionID));
		id += XPLICIT_UNIVERSE_DELIM;

		return id;
	}

	const std::int64_t& XplicitID::get() noexcept
	{
		return mVersion;
	}
}