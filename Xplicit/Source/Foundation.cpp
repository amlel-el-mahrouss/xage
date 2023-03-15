/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: Foundation.cpp
 *			Purpose: Xplicit Foundation Header
 *
 * =====================================================================
 */

#include "Foundation.h"

static FILE* g_stdlog{ nullptr };

XPLICIT_API FILE* xplicit_get_logger(void) 
{
	return g_stdlog;
}

size_t fstrlen(const char* buffer) 
{
	if (!buffer) return 0;
	if (*buffer == XPLICIT_END_OF_BUFFER) return 0;

	size_t index = 0;
	while (buffer[index] != XPLICIT_END_OF_BUFFER) ++index;

	return index;
}

time_t xplicit_get_epoch() 
{
	static time_t curtime{};
	curtime = time(&curtime);
	
	return curtime;
}

void xplicit_log(const char* msg) 
{
#ifdef XPLICIT_DEBUG
	char buf[sizeof(time_t)];
	snprintf(buf, sizeof(time_t), "%llu", xplicit_get_epoch());
	fprintf(g_stdlog, "[%s - LOG] %s", buf, msg);
#endif // ifdef XPLICIT_DEBUG
}

char dbg_filename[256];

bool xplicit_open_logger() 
{
#ifndef NDEBUG
	snprintf(dbg_filename, 256, "%llu_xplicit.log", xplicit_get_epoch());

	if (fopen_s(&g_stdlog, dbg_filename, "w+") != EXIT_SUCCESS) 
	{
		assert(false);
		exit(EXIT_FAILURE);

		return false;
	}

#endif // !NDEBUG

	return true;
}