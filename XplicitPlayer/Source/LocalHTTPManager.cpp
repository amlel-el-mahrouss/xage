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
        auto sock = http_writer.create_and_connect(mEndpoint);

        if (!sock)
            return;

        auto hdr = new HTTP::HTTP::HTTPHeader{ .Type = HTTP::HTTP::RequestType::GET, .Bytes = const_cast<char*>(assetId.data()), .Size = static_cast<int>(assetId.size()), };

        Ref<HTTP::HTTP::HTTPHeader*> hdr_wrapper{ hdr };

        if (!http_writer.send_from_socket(sock, hdr_wrapper))
        {
            delete hdr;
            return;
        }

        static XPLICIT_GET_DATA_DIR(full_path);

        String http_path;

        http_path += full_path;
        http_path += "Contents/";
        http_path += assetId;

        std::ofstream file = mWriter->write(http_path.c_str());

        constexpr int64_t MAX_BUF = 100000;

        auto bytes = new char[MAX_BUF];
        memset(bytes, 0, MAX_BUF);

        http_writer.read_from_socket(sock, bytes, MAX_BUF);

        file << bytes;
        file.close();

        delete bytes;
        delete hdr;
	}
	
	void LocalHTTPManager::set(const String& endpoint) noexcept
	{
		if (!endpoint.empty())
			mEndpoint = endpoint;
	}
}