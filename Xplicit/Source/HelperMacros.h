/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: HelperMacros.h
 *			Purpose:
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

#ifdef XPLICIT_WINDOWS
#ifndef XPLICIT_GET_DATA_DIR
#define XPLICIT_GET_DATA_DIR(DIR)\
Xplicit::String DIR = getenv("APPDATA");\
DIR += "/Data/";

#endif
#elif defined(__linux__)
#ifndef XPLICIT_GET_DATA_DIR
#define XPLICIT_GET_DATA_DIR(DIR)\
DIR += "/var/Data/";

#endif
#endif