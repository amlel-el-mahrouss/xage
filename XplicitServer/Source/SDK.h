/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: SDK.h
 *			Purpose: Xplicit and Nplicit SDK
 *
 * =====================================================================
 */

#pragma once

#include <Uri.h>
#include <Event.h>
#include <Instance.h>
#include <Xplicit.h>
#include <MonoInterop.h>
#include <MonoInstance.h>
#include <ApplicationContext.h>
#include <NetworkServerInstance.h>

#ifdef XPLICIT_WINDOWS
extern "C"
{
#	include <conio.h>
};
#endif

#include <Nplicit.h>
#include <RigidBody.h>

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>
