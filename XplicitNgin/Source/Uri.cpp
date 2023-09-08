/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright PlayXPlicit, all rights reserved.
 *
 *			File: Uri.cpp
 *			Purpose: Xplicit URI parser
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "Uri.h"

#ifndef URI_MAXSIZE
#define URI_MAXSIZE (1024)
#endif // URI_MAXSIZE

#ifndef URI_SEPARATOR
#define URI_SEPARATOR '\a'
#endif // URI_SEPARATOR

namespace Xplicit::Utils
{
	UriParser::UriParser(const char* protocol)
	{
		for (std::size_t i = 0; i < strlen(protocol); i++)
		{
			if (protocol[i] == ':')
				break;

			m_protocol += protocol[i];
		}
	}

	UriParser::~UriParser() = default;

	std::string UriParser::get() noexcept
	{
		std::string uri;
		
		for (size_t i = 0; i < m_data.size(); i++)
		{
			if (m_data[i] == URI_SEPARATOR)
				continue;

			uri.push_back(m_data[i]);
		}

		uri += '\0';

		return uri;
	}

	UriParser& UriParser::operator/=(const std::string& uri)
	{
		this->operator/=(uri.c_str());

		return *this;
	}

	UriParser& UriParser::operator/=(const char* uri)
	{
		if (!uri ||
			*uri == 0)
			return *this;

		std::string uri_str = uri;

		if (strstr(uri, m_protocol.c_str()))
			return *this;

		for (size_t i = 0; i < uri_str.size(); ++i)
		{
			if (uri_str[i] == '\\' || uri_str[i] == '/')
			{
				m_data.push_back(URI_SEPARATOR);
			}
			else if (uri_str[i] == ':')
			{
				++i;

				for (size_t y = i; y < uri_str.size(); ++y)
				{
					m_port += uri_str[y];
				}

				break;
			}
			else
			{
				m_data.push_back(uri[i]);
			}
		}

		return *this;
	}

	std::string UriParser::port() noexcept { return m_port; }

	std::string UriParser::protocol() noexcept { return m_protocol; }

	UriParser& UriError::get() { return m_uri; }
}