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
	bool XHTTPManager::download(const String assetId, const String outputFileName) const noexcept
	{
        HTTP::HTTPWriter http_writer;
        
        // We don't use MakeReq.exe here, it could be missing.

        // static is added here wtf??
        // - check the preprocessor value of XPLICIT_GET_DATA_DIR :)
        static XPLICIT_GET_DATA_DIR(full_path);

        String http_path;

        http_path += full_path;
        http_path += "Contents/";
        http_path += outputFileName;

        std::ofstream file = mWriter.write(http_path.c_str());

        constexpr int64_t MAX_BUF = 1000000;

        auto bytes = new char[MAX_BUF];
        XPLICIT_ASSERT(bytes);

        memset(bytes, 0, MAX_BUF);

        if (bytes)
        {
            auto path = HTTP::HTTPHelpers::make_get(assetId, mEndpoint);

            auto http_hdr = HTTP::HTTP::HTTPHeader{ .Type = HTTP::HTTP::RequestType::GET, .Bytes = "", .Size = static_cast<int>(path.size()),};
            memcpy(http_hdr.Bytes, path.data(), strlen(path.data()));

            Ref<HTTP::HTTP::HTTPHeader*> http_hdr_wrapper{ &http_hdr };

            auto sock = http_writer.create_and_connect(mEndpoint);

            if (!sock)
                return false;

            if (!http_writer.send_from_socket(sock, http_hdr_wrapper))
                return false;

            http_writer.read_from_socket(sock, bytes, MAX_BUF);

            std::string _bytes = bytes;

            auto pos_bytes = _bytes.find("\r\n\r\n");

            if (pos_bytes == std::string::npos)
            {
                pos_bytes = _bytes.find("\n\n");
                if (pos_bytes == std::string::npos)
                {
                    std::cout << "[HTTPS] Fail, invalid http response." << "\n";
                    return false;
                }
            }

            if (pos_bytes != std::string::npos)
            {
                std::cout << "Content-Length: " << HTTP::HTTPHelpers::content_length<10>(_bytes) << "\n";
                std::cout << "Body: " << pos_bytes << "\n";

                http_writer.read_from_socket(sock, bytes, HTTP::HTTPHelpers::content_length<10>(_bytes));

                auto sz = HTTP::HTTPHelpers::content_length<10>(_bytes);

                http_writer.read_from_socket(sock, bytes, sz);

                _bytes.clear();
                _bytes = bytes;

                std::string start = _bytes.substr(0, sz);

                file << start;

                file.flush();
                file.close();
            }


            delete[] bytes;

            return true;
        }

        return false;
	}
	
	void XHTTPManager::set_endpoint(const String& endpoint) noexcept
	{
		if (!endpoint.empty())
			mEndpoint = endpoint;
	}
}