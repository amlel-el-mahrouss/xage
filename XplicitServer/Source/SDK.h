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

// Game Variables (Sevrer Only)
#include <GameVar.h>

// Mono support (C#)
#include <MonoInterop.h>
#include <MonoComponent.h>

// Networking
#include <NetworkServerComponent.h>

#ifdef XPLICIT_WINDOWS

extern "C"
{
#	include <conio.h>
};

#endif

// NplictNgine
#include <Nplicit.h>
#include <RigidBody.h>

// RapidXML (very fast XML library)
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>
