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

#ifndef XPLICIT_GET_DATA_DIR
#define XPLICIT_GET_DATA_DIR(DIR)\
Xplicit::String DIR = getenv("APPDATA");\
DIR += "/XplicitNgin/";

#endif // ifndef XPLICIT_GET_DATA_DIR