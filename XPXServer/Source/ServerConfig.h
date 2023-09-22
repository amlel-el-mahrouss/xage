// this file handles every configuration bit of the server.

#pragma once

#include <NginCore.h>

#include <Uri.h>
#include <Util.h>
#include <Enums.h>
#include <HelperMacros.h>

#include <GameVar.h>

#include <Event.h>
#include <Component.h>

#include <MonoInterop.h>
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
#include <NMath.h>

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>

#define XPLICIT_XASSET_ENDPOINT "play-xplicit.com"
#define XPLICIT_PLACE_ENDPOINT "play-xplicit.com"

inline std::string XPLICIT_PLACE_ID = "";