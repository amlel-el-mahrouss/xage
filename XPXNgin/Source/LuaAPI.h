#pragma once

#include "Config.h"

#include <CLua.hpp>

#define XPLICIT_LUA_GLOBAL "_G"
#define XPLICIT_LUA_NAMESPACE "world"

XPLICIT_API void XplicitLoadBaseLua();

class XPXUri
{
public:
	static const char* name() { return "Uri"; }

public:
	XPX::Utils::UriParser Uri = { XPLICIT_XASSET_PROTOCOL };

public:
	XPXUri() = default;
	~XPXUri() = default;

public:
	XPLICIT_COPY_DEFAULT(XPXUri);

public:
	static int parse_url(lua_State* L)
	{
		XPXUri* uri = (XPXUri*)lua_touserdata(L, 1);
		XPX::String uri_str = lua_tostring(L, 2);

		uri_str = uri_str.erase(uri_str.find(XPLICIT_XASSET_PROTOCOL), strlen(XPLICIT_XASSET_PROTOCOL));

		if (uri &&
			!uri_str.empty())
		{
			uri->Uri /= uri_str;

			lua_pushstring(L, uri->Uri.get().c_str());
			return 1;
		}

		lua_pushstring(L, "invalid-asset");
		return 1;
	}

};

