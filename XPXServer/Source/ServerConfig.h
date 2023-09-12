// this file handles every configuration bit of the server.

#pragma once

#include <Uri.h>
#include <Util.h>
#include <Event.h>
#include <NginCore.h>
#include <GameVar.h>
#include <Component.h>
#include <MonoInterop.h>
#include <Enums.h>
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

inline float XPLICIT_DESTROY_Y(-1000000.f); // Add which point the Character should be destroyed.

#define XPLICIT_XASSET_ENDPOINT "play-xplicit.com"
#define XPLICIT_PLACE_ENDPOINT "play-xplicit.com"

inline std::string XPLICIT_PLACE_ID = "";