/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

// @file LocalHTTPManager.cpp

#include "LocalHTTPManager.h"
#include <HelperMacros.h>

namespace Xplicit::Player
{
	void LocalHTTPManager::download(const String& assetId) const noexcept
	{
        HTTP::HTTPWriter http_writer;
        
        String request = mEndpoint + "/" + assetId;

        auto hdr = new HTTP::HTTP::HTTPHeader{
            .Type = HTTP::HTTP::RequestType::GET,
            .Bytes = request.data(),
            .Size = static_cast<int>(request.size()),
        };

        Ref<HTTP::HTTP::HTTPHeader*> hdr_wrapper{ hdr };

        auto sock = http_writer.create_and_connect( mEndpoint );

        if (!sock)
            return;

        if (!http_writer.send_from_socket(sock, hdr_wrapper))
            return;

        XPLICIT_GET_DATA_DIR(fullPath);
        fullPath += "Contents/";
        fullPath += assetId;

        std::ofstream file = mWriter->write(fullPath.c_str());

        constexpr int64_t MAX_BUF = 100000;

        auto bytes = new char[MAX_BUF];
        memset(bytes, 0, MAX_BUF);

        http_writer.read_from_socket(sock, bytes, MAX_BUF);

        file << bytes;

        file.flush();
        file.close();

        delete hdr;
	}
	
	void LocalHTTPManager::set(const String& endpoint) noexcept
	{
		if (!endpoint.empty())
			mEndpoint = endpoint;
	}
}