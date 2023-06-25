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
	 *	@brief LocalHTTPManager
	 *	@purpose Downloads and xasset:// from HTTP/1.1 using Xplicit's own client.
	 */
	
	class LocalHTTPManager final
	{
	public:
		LocalHTTPManager() = default;
		~LocalHTTPManager() = default;

		XPLICIT_COPY_DEFAULT(LocalHTTPManager);
		
	public:
		void download(const String& assetId) const noexcept;
		void set(const String& endpoint) noexcept;
		
		const char* protocol() noexcept { return "xasset://"; }
		float version() const noexcept { return 1.1f; }
		
	private:
		std::unique_ptr<FilesystemWrapper> mWriter;
		String mEndpoint;
	
	};
}
