/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: CommonInclude.h
 *			Purpose: Server include
 *
 * =====================================================================
 */

#pragma once

#include <Uri.h>
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