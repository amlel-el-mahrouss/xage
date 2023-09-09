#pragma once

#include "Config.h"

#define XPLICIT_LUA_GLOBAL "_G"
#define XPLICIT_LUA_NAMESPACE "World"

XPLICIT_API void XplicitLoadBaseLua();

// CLua builtin types
struct Vector3
{
private:
	float X, Y, Z;

public:
	static int get_x(lua_State* L)
	{
		Vector3* vec = (Vector3*)lua_touserdata(L, 1);

		std::string index = luaL_checkstring(L, 2);
		luaL_argcheck(L, index == "X", 2, "index out of range");

		lua_pushnumber(L, vec->X);
		return 1;
	}


	static int set_x(lua_State* L)
	{
		Vector3* vec = (Vector3*)lua_touserdata(L, 1);

		std::string index = luaL_checkstring(L, 2);
		luaL_argcheck(L, index == "Z", 2, "index out of range");

		float X = lua_tonumber(L, 3);

		vec->X = X;

		return 1;
	}

	static int get_y(lua_State* L)
	{
		Vector3* vec = (Vector3*)lua_touserdata(L, 1);

		std::string index = luaL_checkstring(L, 2);
		luaL_argcheck(L, index == "Y", 2, "index out of range");

		lua_pushnumber(L, vec->Y);
		return 1;
	}


	static int set_y(lua_State* L)
	{
		Vector3* vec = (Vector3*)lua_touserdata(L, 1);

		std::string index = luaL_checkstring(L, 2);
		luaL_argcheck(L, index == "Z", 2, "index out of range");

		float Y = lua_tonumber(L, 3);

		vec->Y = Y;

		return 1;
	}

	static int get_z(lua_State* L)
	{
		Vector3* vec = (Vector3*)lua_touserdata(L, 1);

		std::string index = luaL_checkstring(L, 2);
		luaL_argcheck(L, index == "Z", 2, "index out of range");

		lua_pushnumber(L, vec->Y);
		return 1;
	}


	static int set_z(lua_State* L)
	{
		Vector3* vec = (Vector3*)lua_touserdata(L, 1);

		std::string index = luaL_checkstring(L, 2);
		luaL_argcheck(L, index == "Z", 2, "index out of range");

		float Z = lua_tonumber(L, 3);

		vec->Z = Z;

		return 1;
	}

	static int add(lua_State* L)
	{
		Vector3* vec = (Vector3*)lua_touserdata(L, 1);
		Vector3* vec2 = (Vector3*)lua_touserdata(L, 2);

		vec->X += vec2->X;
		vec->Y += vec2->Y;
		vec->Z += vec2->Z;

		return 0;
	}

	static int mul(lua_State* L)
	{
		Vector3* vec = (Vector3*)lua_touserdata(L, 1);
		Vector3* vec2 = (Vector3*)lua_touserdata(L, 2);

		vec->X *= vec2->X;
		vec->Y *= vec2->Y;
		vec->Z *= vec2->Z;

		return 0;
	}

	static int sub(lua_State* L)
	{
		Vector3* vec = (Vector3*)lua_touserdata(L, 1);
		Vector3* vec2 = (Vector3*)lua_touserdata(L, 2);

		vec->X -= vec2->X;
		vec->Y -= vec2->Y;
		vec->Z -= vec2->Z;

		return 0;
	}

};
