/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

namespace Xplicit::Player
{
	/*
	 *	@brief LocalHTTPComponent
	 *	@purpose Downloads and xasset:// from HTTP/1.1 using Xplicit's own client.
	 */
	
	class LocalHTTPComponent final
	{
	public:
		LocalHTTPComponent() = default;
		~LocalHTTPComponent() = default;

		XPLICIT_COPY_DEFAULT(LocalHTTPComponent);
		
	public:
		void download(const String& assetId) noexcept;
		void set(const String& endpoint) noexcept;
		
		const char* protocol() { return "xasset://"; }
		const int version() { return 1; }
		
	private:
		std::unique_ptr<FilesystemWrapper> mWriter;
		String mEndpoint;
	
	};
}
