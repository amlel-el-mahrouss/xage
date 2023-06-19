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

#pragma once

#include "Xplicit.h"

#define XPLICIT_UNIVERSE_ID (1U)

namespace Xplicit::Auth
{
	class XPLICIT_API XplicitID final
	{
	public:
		XplicitID() = delete;

	public:
		explicit XplicitID(const int32_t& universe, const int32_t& playerId, const int64_t& hash) noexcept;
		~XplicitID() = default;

	public:
		XPLICIT_COPY_DEFAULT(XplicitID);

	public:
		const std::int64_t get() noexcept;
		const String as_string() noexcept;

	private:
		std::int64_t mConnectionHash;
		std::int64_t mRegionId;

	};
}