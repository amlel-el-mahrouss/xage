/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <Xplicit.h>
#include <Http.h>

namespace Xplicit::Player
{
	/*
	 *	@brief LocalHTTPManager
	 *	@brief Downloads and xasset:// from HTTP/1.1 using Xplicit's own client.
	 */
	
	class LocalHTTPManager final
	{
	public:
		LocalHTTPManager() = default;
		~LocalHTTPManager() = default;

	public:
		XPLICIT_COPY_DEFAULT(LocalHTTPManager);
		
	public:
		void download(const String& assetId) const noexcept;
		void set_endpoint(const String& endpoint) noexcept;
		
		const char* protocol() noexcept { return "xasset://"; }
		float version() const noexcept { return 1.1f; }
		
	private:
		std::unique_ptr<FilesystemWrapper> mWriter;
		String mEndpoint;
	
	};
}
