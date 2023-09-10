/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright PlayXPlicit, all rights reserved.
 *
 *			File: HelperMacros.h
 *			Purpose:
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"

#ifndef XPLICIT_GET_DATA_DIR
#define XPLICIT_GET_DATA_DIR(DIR)\
XPX::String DIR = getenv("APPDATA");\
DIR += "/XplicitNgin/";

#endif // ifndef XPLICIT_GET_DATA_DIR