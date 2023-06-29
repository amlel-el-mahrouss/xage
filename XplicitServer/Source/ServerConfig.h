// this file handles every configuration bit of the server.

#pragma once

inline const char* XPLICIT_EVENTS[] = {
	"Move",
	"Leave",
	"Join",
	"Death",
	"Spawn",
	"Damage",
};

#define XPLICIT_EVENT_MAX (6)

#define XPLICIT_XASSET_ENDPOINT ("play-xplicit.com/library")
