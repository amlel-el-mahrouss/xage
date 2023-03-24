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

// ECS
#include <Event.h>
#include <Component.h>

// Useful
#include <Uri.h>
#include <Xplicit.h>

// Game Variables
#include <GameVar.h>

// C# support.
#include <MonoInterop.h>
#include <MonoComponent.h>

// Common
#include <Common.h>

// Networking
#include <SocketWrapper.h>
#include <NetworkDownload.h>
#include <NetworkServerComponent.h>

#ifdef XPLICIT_WINDOWS

extern "C"
{
#	include <conio.h>
};

#endif

// Physics
#include <Nplicit.h>
#include <RigidBody.h>

// XML
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>
