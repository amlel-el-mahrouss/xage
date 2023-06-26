/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

// OpenSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace Xplicit::HTTP
{
    constexpr int16_t XPLICIT_HTTP_PORT = 443;

    class MIMEFactory;
    class HTTPError;
    class HTTPHelpers;
    class HTTPWriter;

    class MIMEFactory final 
    {
    public:
        struct MIME final 
        {
            std::string t_name;
            std::string t_mime;
        };

    public:
        MIMEFactory() = default;
        ~MIMEFactory() = default;

        MIMEFactory& operator=(const MIMEFactory&) = default;
        MIMEFactory(const MIMEFactory&) = default;

    public:
        MIMEFactory::MIME operator()(char* name) 
        {
            if (!name) 
                return { .t_name = "Anything", .t_mime = "*/*" };

            std::string extension = strchr(name, '.');
            XPLICIT_ASSERT(!extension.empty());

            if (!strcmp(extension.c_str(), ".png"))
                return { .t_name = "PNG", .t_mime = "Content-Type: image/png" };
            else if (!strcmp(extension.c_str(), ".bmp"))
                return { .t_name = "Bitmap", .t_mime = "Content-Type: image/bmp" };
            else if (!strcmp(extension.c_str(), ".jpg"))
                return { .t_name = "JPEG", .t_mime = "Content-Type: image/jpeg" };
            else if (!strcmp(extension.c_str(), ".ar"))
                return { .t_name = "Xplicit Package Format", .t_mime = "Content-Type: archive/ar" };

            return { .t_name = "Not allowed", .t_mime = "Content-Type: */not-allowed" };
        }

    };

    namespace HTTP 
    {
        class HTTPSocket final 
        {
            struct sockaddr_in m_Addr;
            std::string m_Dns;
            SOCKET m_Socket;

            friend HTTPWriter;

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

    class HTTPError final : public std::runtime_error 
    {
    public:
        explicit HTTPError(const std::uint16_t what) : std::runtime_error("Xplicit HTTP Error") {  }
        ~HTTPError() override = default; // let the ABI define that.

        HTTPError& operator=(const HTTPError&) = default;
        HTTPError(const HTTPError&) = default;
        
        int error() const { return mError; }

    private:
        int mError{ 200 };

    };

    class HTTPHelpers 
    {
    public:
        static std::string make_get(const std::string& path, 
            const std::string& host
        ) 
        {
            if (path.empty() || host.empty()) return "";

            std::string request = "GET " + path + " HTTP/1.1\r\n";
            request += "Host: " + host + "\r\n";
            request += "Connection: close\r\n";
            request += "\r\n\r\n";

            return request;
        }

        static bool has_field(const std::string& http, const std::string& rest)
        {
            if (http.empty()) 
                return false;

            if (rest.empty())
                throw std::runtime_error("Bad restrict.");

            return http.find(rest) != std::string::npos;
        }

        template <int Base>
        static long content_length(const std::string& http)
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

            return std::stol(final, nullptr, Base);
        }

        static const int bad_pos = -1;

    };

    using HTTPSharedPtr = std::shared_ptr<HTTP::HTTPSocket>;
    
    class HTTPWriter final 
    {
    public:
        explicit HTTPWriter()
        {
            m_SslCtx = init_ssl();
            m_Ssl = SSL_new(m_SslCtx);

            // in case the SSL context fails
            if (m_Ssl == nullptr)
            {
                fprintf(stderr, "SSL_new() failed\n");

                throw EngineError("Bad SSL context, SSL_new() failed!");
            }
        }

        ~HTTPWriter() noexcept 
        {
            if (shutdown(m_Socket->m_Socket, SD_BOTH) == SOCKET_ERROR)
                closesocket(m_Socket->m_Socket);

            char buf[256];
            vsprintf_s<256U>(buf, "[SERVER] %s has been closed!", m_Socket->m_Dns.data());

            XPLICIT_INFO(buf);
            xplicit_log(buf);

            SSL_free(m_Ssl);
            SSL_CTX_free(m_SslCtx);
        }

        HTTPWriter& operator=(const HTTPWriter&) = default;
        HTTPWriter(const HTTPWriter&) = default;

        HTTPSharedPtr create_and_connect(const std::string& dns) 
        {
            if (dns.empty()) 
                throw HTTPError(HTTP_DNS_ERROR);

            HTTPSharedPtr sock = std::make_unique<HTTP::HTTPSocket>();

            if (!sock)
                throw HTTPError(HTTP_INTERNAL_ERROR);
            
            sock->m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

            if (sock->m_Socket == INVALID_SOCKET) 
                throw HTTPError(HTTP_INTERNAL_ERROR);

            ZeroMemory(&sock->m_Addr, sizeof(struct sockaddr_in));

            sock->m_Addr.sin_family = AF_INET;
            ::inet_pton(AF_INET, dns.c_str(), &sock->m_Addr.sin_addr);

            sock->m_Addr.sin_port = ::htons(XPLICIT_HTTP_PORT);
            sock->m_Dns = std::string{ dns.data() };

            int result = ::connect(sock->m_Socket, reinterpret_cast<SOCKADDR*>(&sock->m_Addr), sizeof(sock->m_Addr));
            
            if (result == SOCKET_ERROR) 
                throw HTTPError(HTTP_DNS_ERROR);

            SSL_set_fd(m_Ssl, sock->m_Socket);
            auto status = SSL_connect(m_Ssl);

            if (status != 1)
            {
                SSL_get_error(m_Ssl, status);
                ERR_print_errors_fp(stderr); //High probability this doesn't do anything

                return nullptr;
            }

            printf("[HTTPS] Connected with %s encryption\n", SSL_get_cipher(m_Ssl));

            return sock;
        }

        int64_t send_from_socket(HTTPSharedPtr& sock, Ref<HTTP::HTTPHeader*>& hdr) 
        {
            XPLICIT_ASSERT(sock);
            XPLICIT_ASSERT(hdr);
            XPLICIT_ASSERT(!sock->m_Dns.empty());

            return SSL_write(m_Ssl, hdr->Bytes, hdr->Size);
        }

        int64_t read_from_socket(HTTPSharedPtr& sock, char* bytes, int len) 
        {
            XPLICIT_ASSERT(sock);
            XPLICIT_ASSERT(bytes);
            XPLICIT_ASSERT(len > 0);

            return SSL_read(m_Ssl, bytes, len);
        }

    private:
        SSL_CTX* init_ssl(void) noexcept
        {
            const SSL_METHOD* method = TLS_client_method(); /* Create new client-method instance */
            SSL_CTX* ctx = SSL_CTX_new(method);

            if (ctx == nullptr)
            {
                ERR_print_errors_fp(stderr);
                return nullptr;
            }

            return ctx;
        }

    private:
        HTTPSharedPtr m_Socket;
        SSL_CTX* m_SslCtx;
        SSL* m_Ssl;

    };
}