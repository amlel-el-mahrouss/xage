/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

 /**
 @file
 */

#include "NginCore.h"

static FILE* XPLICIT_LOGGER{ nullptr };

XPLICIT_API FILE* xplicit_get_logger(void) 
{
	return XPLICIT_LOGGER;
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
	time_t curtime = time(nullptr);
	return curtime;
}

void xplicit_log(const char* msg) 
{
#ifdef XPLICIT_DEBUG
	char buf[sizeof(time_t)];
	snprintf(buf, sizeof(time_t), "%llu", xplicit_get_epoch());
	fprintf(XPLICIT_LOGGER, "[%s - LOG] %s", buf, msg);
#endif // ifdef XPLICIT_DEBUG
}

char dbg_filename[256];

bool xplicit_open_logger() 
{
#ifdef XPLICIT_DEBUG
	snprintf(dbg_filename, 256, "%llu_xplicit.log", xplicit_get_epoch());

	if (fopen_s(&XPLICIT_LOGGER, dbg_filename, "w+") != EXIT_SUCCESS) 
	{
		assert(false);
		exit(EXIT_FAILURE);

		return false;
	}

#endif // !NDEBUG

	return true;
}
