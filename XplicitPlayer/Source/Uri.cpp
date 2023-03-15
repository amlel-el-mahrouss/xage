/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: Uri.cpp
 *			Purpose: Xplicit URI library
 *
 * =====================================================================
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

	std::string UriParser::get() noexcept
	{
		if (m_data.empty())
			return (m_protocol + "invalid");

		std::string uri;

		for (size_t i = 0; i < m_data.size(); i++)
		{
			if (m_data[i] == URI_SEPARATOR)
				continue;

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
				m_data.push_back(URI_SEPARATOR);
			else
				m_data.push_back(uri[i]);
		}

		return *this;
	}
}