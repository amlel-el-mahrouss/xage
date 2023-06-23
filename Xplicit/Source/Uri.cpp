/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
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
		: m_protocol(protocol)
	{

	}

	UriParser::~UriParser()
	{

	}

	std::string UriParser::get(const UriAccessor offset) noexcept
	{
		if (m_data.empty())
			return (m_protocol + "invalid");

		std::string uri;

		for (size_t i = 0; i < m_data.size(); i++)
		{
			if (m_data[offset] == URI_SEPARATOR)
				break;

			uri.push_back(m_data[i]);
		}

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

		if (!strstr(uri, m_protocol.c_str()))
			return *this;

		int32_t count = 0;

		for (size_t i = strlen(m_protocol.c_str()); i < strlen(uri); ++i)
		{
			if (uri[i] == '\\' || uri[i] == '/')
			{
				m_data.push_back(URI_SEPARATOR);
			}
			else if (uri[i] == ':')
			{
				++i;

				for (size_t y = i; y < strlen(uri); ++y)
				{
					m_port += uri[y];
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

	UriParser& UriError::get() { return m_uri; }
}