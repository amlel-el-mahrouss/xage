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

#define XPLICIT_UNIVERSE_DELIM ":"
#define XPLICIT_UNIVERSE_PREFIX "XPLICIT_"

namespace Xplicit::Auth
{
	//!
	//! @class : XplicitID
	//! @purpose : Xplicit IDentifier
	//! 
	//! @brief Xplicit.com ID
	//! 
	class XPLICIT_API XplicitID final
	{
	public:
		XplicitID() = delete;

	public:
		//! notice pass the public hash here, not the private one!
		//! otherwise this will leak the private hash of the user!
		explicit XplicitID(const int32_t& universe, 
			const int32_t& hash) noexcept;

		~XplicitID() = default;

	public:
		XPLICIT_COPY_DEFAULT(XplicitID);

	public:
		/**
		 * \brief Returns the current XplicitID as an integer.
		 * \return This XplicitID as a 64-bit long integer.
		 */
		const std::int64_t& as_int() const noexcept;

		/**
		 * \brief Returns the current XplicitID
		 * \return This XplicitID as a string.
		 */
		const String& as_string()  noexcept;

	private:
		String mXplicitId;
		
		std::int64_t mConnectionHash;
		std::int64_t mRegionId;

	};
}