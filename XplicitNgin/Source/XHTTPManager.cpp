/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

// @file XHTTPManager.cpp

#include "XHTTPManager.h"
#include "HelperMacros.h"

namespace Xplicit::Player
{
	void XHTTPManager::download(const String& assetId) const noexcept
	{
        HTTP::HTTPWriter http_writer;
        
        auto sock = http_writer.create_and_connect(mEndpoint);

        if (!sock)
            return;

        // We don't use MakeReq.exe here, it could be missing.

        auto http_hdr = HTTP::HTTP::HTTPHeader{ .Type = HTTP::HTTP::RequestType::GET, .Bytes = const_cast<char*>(assetId.data()), .Size = static_cast<int>(assetId.size()), };

        Ref<HTTP::HTTP::HTTPHeader*> http_hdr_wrapper{ &http_hdr };

        if (!http_writer.send_from_socket(sock, http_hdr_wrapper))
            return;

        // static is added here wtf??
        // - check the preprocessor value of XPLICIT_GET_DATA_DIR :)
        static XPLICIT_GET_DATA_DIR(full_path);

        String http_path;

        http_path += full_path;
        http_path += "Contents/";
        http_path += assetId;

        std::ofstream file = mWriter.write(http_path.c_str());

        constexpr int64_t MAX_BUF = 1000000;

        auto bytes = new char[MAX_BUF];
        XPLICIT_ASSERT(bytes);

        if (bytes)
        {
            memset(bytes, 0, MAX_BUF);

            http_writer.read_from_socket(sock, bytes, MAX_BUF);

            file << bytes;
            file.close();

            delete bytes;
        }
	}
	
	void XHTTPManager::set_endpoint(const String& endpoint) noexcept
	{
		if (!endpoint.empty())
			mEndpoint = endpoint;
	}
}