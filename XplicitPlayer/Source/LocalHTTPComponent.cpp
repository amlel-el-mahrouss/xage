/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

// @file LocalHTTPComponent.cpp

#include "LocalHTTPComponent.h"
#include <Http.h>

namespace Xplicit::Player
{
	void download(const String& assetId) noexcept
	{
		Thread thrd([&](String asset) {
            String request = mEndpoint + "/" + asset;
    
            HTTPWriter writer;
            
            HTTP::HTTPHeader* hdr = new http::http_header{
                .Type = http::http_request_type::GET,
                .Bytes = request.c_str(),
                .SizeOfBytes = (int64_t)request.size(),
            };
        
            NonNil<HTTP::HTTPHeader*> hdr_wrapper{ hdr };

            auto sock = writer.create_socket_and_connect(host);
            writer.send_from_socket(sock, hdr_wrapper);
            
            std::ofstream file = mWriter->create{ assetId + ".xasset" };

            char* bytes = new char[1000000];
            memset(bytes, 0, 1000000);

            writer.read_from_socket(sock, bytes, 1000000);
        
            file << bytes;
        
            file.flush();
            file.close();
        
            delete hdr;
		}, assetId);
		
		thrd.detach();
	}
	
	void set(const String& endpoint) noexcept
	{
		if (!endpoint.empty())
			mEndpoint = endpoint;
	}
}