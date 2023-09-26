/*
 * =====================================================================
 *
 *			XPXNgin
 *			Copyright XPX Corporation, all rights reserved.
 *
 *			File: HelperMacros.h
 *			Purpose:
 *
 * =====================================================================
 */

#pragma once

#include "NginCore.h"

#ifndef XPLICIT_GET_DATA_DIR
#ifdef _WIN32
#define XPLICIT_GET_DATA_DIR(DIR)\
XPX::String DIR = getenv("APPDATA");\
DIR += "/NginData/";


#define XPLICIT_GET_DATA_DIR_W(DIR)\
XPX::PString DIR = _wgetenv(L"APPDATA");\
DIR += L"/NginData/";


#else
#define XPLICIT_GET_DATA_DIR(DIR)\
XPX::String DIR = getenv("HOME");\
DIR += "/NginData/";


#endif
#endif // ifndef XPLICIT_GET_DATA_DIR