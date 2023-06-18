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

namespace Xplicit::ID
{
	class XPLICIT_API XplicitID final
	{
	public:
		explicit XplicitID() noexcept;

		~XplicitID() = default;

	public:
		XPLICIT_COPY_DEFAULT(XplicitID);

	public:
		operator bool();
		bool verify() noexcept;

	private:
		String mId;

	};
}