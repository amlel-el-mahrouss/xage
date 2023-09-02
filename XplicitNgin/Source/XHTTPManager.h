/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"
#include "Http.h"
#include "Uri.h"

namespace Xplicit::Player
{
	/*
	 *	@brief XHTTPManager
	 *	@brief Downloads and xasset:// from HTTP/1.1 using Xplicit's own client.
	 */
	
	class XPLICIT_API XHTTPManager final
	{
	public:
		XHTTPManager() = default;
		~XHTTPManager() = default;

	public:
		XPLICIT_COPY_DEFAULT(XHTTPManager);
		
	public:
		bool download(const String assetId, const String outputFileName) const noexcept;
		void set_endpoint(const String& endpoint) noexcept;
		
	public:
		const char* protocol() noexcept { return XPLICIT_XASSET_PROTOCOL; }
		float version() const noexcept { return 1.1f; }
		
	private:
		//! Filesystem wrapper, create, open, delete!
		FilesystemWrapper mWriter;

		//! HTTP endpoint
		String mEndpoint;
	
	};
}
