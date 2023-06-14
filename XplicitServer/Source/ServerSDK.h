/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: SDK.h
 *			Purpose: Server include
 *
 * =====================================================================
 */

#pragma once

#include <Event.h>
#include <Component.h>
#include <Uri.h>
#include <Xplicit.h>
#include <GameVar.h>
#include <MonoInterop.h>
#include <MonoComponent.h>
#include <Common.h>
#include <SocketWrapper.h>
#include <HelperMacros.h>
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
