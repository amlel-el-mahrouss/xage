/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Http.h
 *			Purpose: HTTP client
 *
 * =====================================================================
 */

#pragma once

#include "Foundation.h"

namespace Xplicit::Utils
{
    constexpr int16_t HTTP_PORT = 80;

    class MIMEFactory;
    class HTTPError;
    class HTTPHelpers;
    class HTTPWriter;

    class MIMEFactory final {
    public:
        struct mime final {
            std::string t_name;
            std::string t_mime;
        };

    public:
        static MIMEFactory::mime get(char* name) {
            if (!name) return { .t_name = "Any", .t_mime = "*/*" };
            std::string extension = strchr(name, '.');

            if (!strcmp(extension.c_str(), ".png"))
                return { .t_name = "PNG", .t_mime = "Content-Type: image/png" };
            else if (!strcmp(extension.c_str(), ".bmp"))
                return { .t_name = "Bitmap", .t_mime = "Content-Type: image/bmp" };
            else if (!strcmp(extension.c_str(), ".jpg"))
                return { .t_name = "JPEG", .t_mime = "Content-Type: image/jpeg" };
            else if (!strcmp(extension.c_str(), ".ar"))
                return { .t_name = "Xplicit Package", .t_mime = "Content-Type: application/ar" };
            else if (!strcmp(extension.c_str(), ".exe") || !strcmp(extension.c_str(), ".dll"))
                return { .t_name = "PE+", .t_mime = "Content-Type: application/vnd.microsoft.portable-executable" };

            return { .t_name = "Not allowed", .t_mime = "Content-Type: */not-allowed" };
        }

    };

    namespace HTTP {
        class HTTPSocket final 
        {
        private:
            struct sockaddr_in m_Addr;
            std::string m_Dns;
            SOCKET m_Socket;

            friend Xplicit::Utils::HTTPWriter;

        public:
            HTTPSocket() : m_Socket(~0), m_Addr() {}
            ~HTTPSocket() = default;

            HTTPSocket& operator=(const HTTPSocket&) = default;
            HTTPSocket(const HTTPSocket&) = default;

        };

        enum class RequestType : uint8_t
        {
            GET,
            POST,
        };

        struct HTTPHeader final 
        {
            RequestType Type;
            char* Bytes;
            int Size;

        };

    }

    enum HTTP_ERROR_LIST : int16_t 
    {
        HTTP_OK = 200,
        HTTP_BAD_GATEWAY = 502,
        HTTP_NOT_FOUND = 404,
        HTTP_BAD_REQ = 403,
        HTTP_DNS_ERROR = -1,
        HTTP_INTERNAL_ERROR = -2,
        HTTP_ERROR_COUNT = 6,
    };

    class HTTPError : public std::runtime_error 
    {
    public:
        HTTPError(const uint16_t what) : std::runtime_error("Xplicit HTTP Error") {}
        ~HTTPError() = default; // let the ABI define that.

        HTTPError& operator=(const HTTPError&) = default;
        HTTPError(const HTTPError&) = default;

    public:
        int error() { return m_iErr; }

    private:
        int m_iErr{ 200 };

    };

    class HTTPHelpers 
    {
    public:

        static std::string make_get(const std::string& path, 
            const std::string& host,
            const char* user_agent = "User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.143 Safari/537.36\n"
        ) 
        {
            if (path.empty() || host.empty()) return "";
            if (!user_agent || *user_agent == 0) return "";

            std::string request = "GET " + path + " HTTP/1.1\r\n";
            request += "Host: " + host + "\r\n";
            request += user_agent;
            request += "\r\n\r\n";

            return request;
        }

        static bool has_field(const std::string& http, const std::string& rest)
        {
            if (http.empty()) 
                return false;

            if (rest.empty())
                throw std::runtime_error("Bad restrict");

            return http.find(rest) != std::string::npos;
        }

        template <int Base>
        static int content_length(const std::string& http)
        {
            size_t at = http.find("Content-Length: ");
            if (at == std::string::npos) 
                return HTTPHelpers::bad_pos;

            std::string final;

            for (size_t first = at; first < http.size(); ++first)
            {
                if (http[first] == '\r')
                    break;

                if (http[first] >= '0' && http[first] <= '9') {
                    final += http[first];
                }
            }

            return std::stoi(final, nullptr, Base);
        }

        static const int bad_pos = -1;

    };

    using HTTPSharedPtr = std::shared_ptr<HTTP::HTTPSocket>;
    
    class HTTPWriter final 
    {
    public:
        HTTPWriter() = default;

        ~HTTPWriter() noexcept 
        {
            shutdown(m_Socket->m_Socket, SD_BOTH);
            closesocket(m_Socket->m_Socket);

#ifndef _NDEBUG
            char buf[256];
            vsprintf_s<256U>(buf, "[SERVER] %s has been closed!", m_Socket->m_Dns.data());

            XPLICIT_INFO(buf);
            xplicit_log(buf);
#endif
        }

        HTTPWriter& operator=(const HTTPWriter&) = default;
        HTTPWriter(const HTTPWriter&) = default;

        HTTPSharedPtr create_and_connect(const std::string& dns) {
            if (dns.empty()) 
                throw HTTPError(HTTP_INTERNAL_ERROR);

            HTTPSharedPtr sock = std::make_unique<HTTP::HTTPSocket>();

            if (!sock)
                throw HTTPError(HTTP_INTERNAL_ERROR);
            
            sock->m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

            if (sock->m_Socket == INVALID_SOCKET) 
                throw HTTPError(HTTP_INTERNAL_ERROR);

            ZeroMemory(&sock->m_Addr, sizeof(struct sockaddr_in));

            sock->m_Addr.sin_family = AF_INET;
            inet_pton(AF_INET, dns.c_str(), &sock->m_Addr.sin_addr);

            sock->m_Addr.sin_port = htons(HTTP_PORT);
            sock->m_Dns = std::string{ dns.data() };

            int result = connect(sock->m_Socket, reinterpret_cast<SOCKADDR*>(&sock->m_Addr), sizeof(sock->m_Addr));
            
            if (result == -1) 
                throw HTTPError(HTTP_DNS_ERROR);

            return sock;
        }

        int64_t send_from_socket(HTTPSharedPtr& sock, HTTP::HTTPHeader* hdr) {
#ifndef NDEBUG
            assert(sock);
            assert(hdr);
            assert(!sock->m_Dns.empty());
#endif

            int64_t result{ -1 };
            if (result = send(sock->m_Socket, hdr->Bytes, hdr->Size, 0) != hdr->Size)
                throw HTTPError(HTTP_INTERNAL_ERROR);

            return result;
        }

        int64_t read_from_socket(HTTPSharedPtr& sock, char* bytes, int len) {
            if (!sock) throw HTTPError(HTTP_INTERNAL_ERROR);
            if (!bytes) throw HTTPError(HTTP_INTERNAL_ERROR);

#ifdef XPLICIT_DEBUG
            XPLICIT_ASSERT(sock);
#endif

            int64_t data_length{ -1 };
            data_length = recv(sock->m_Socket, bytes, len, 0);

            return data_length;
        }

    private:
        HTTPSharedPtr m_Socket;

    };
}