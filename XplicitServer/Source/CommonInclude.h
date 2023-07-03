/*
 * =====================================================================
 *
 *			XplicitServer
 *			Copyright Xplicit Inc, all rights reserved.
 *
 *			File: CommonInclude.h
 *			Purpose: Server include
 *
 * =====================================================================
 */

#pragma once

#include <Uri.h>
#include <Util.h>
#include <Event.h>
#include <Xplicit.h>
#include <GameVar.h>
#include <Component.h>
#include <MonoInterop.h>
#include <CommonEngine.h>
#include <HelperMacros.h>
#include <MonoComponent.h>
#include <SocketWrapper.h>
#include <NetworkServerComponent.h>

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

#include "ServerConfig.h"

inline Xplicit::Vector<float> XPLICIT_LIMIT(0.0f, 0.0f, 0.f); // World Limit, set this to zero if you don't mind about any limit.
inline float XPLICIT_DESTROY_Y(-1000000.f); // Add which point the Character should be destroyed.