/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "NginCore.h"

#define XPLICIT_UNIVERSE_DELIM "_"
#define XPLICIT_UNIVERSE_PREFIX "XPX_"

#define XPLICIT_ID_DUMMY "XPX_0_0000000000"

namespace XPX::Auth
{
	//!
	//! @class : XplicitID
	//! @brief : Xplicit IDentifier or @brief play-xplicit.com ID
	//! 
	class XPLICIT_API XplicitID final
	{
	public:
		XplicitID() = delete;

	public:
		//! notice pass the public hash here, not the private one!
		//! otherwise this will leak the private hash of the user!
		explicit XplicitID(const std::int64_t& universe, 
						   const std::int64_t& id) noexcept;

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
		const String& as_string() noexcept;

	public:
		/**
		 * \brief Generates a new XplicitID
		 */
		void generate(const std::int64_t& id) noexcept;

	private:
		std::int64_t mConnectionID;
		std::int64_t mRegionId;
		String mXplicitId;

	};
}