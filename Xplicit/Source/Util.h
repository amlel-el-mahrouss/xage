/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <Nplicit.h>

#include "Root.h"
#include "Xplicit.h"
#include "XplicitID.h"
#include "NetworkServerComponent.h"

static inline Xplicit::Color<float> Magenta(0xFF, 0x00, 0xFF, 0xFF);
static inline Xplicit::Color<float> White(0xFF, 0xFF, 0xFF, 0xFF);
static inline Xplicit::Color<float> DakrRed(0x8B, 0, 0, 0xFF);
static inline Xplicit::Color<float> Black(0, 0, 0, 0xFF);
static inline Xplicit::Color<float> Red(0xFF, 0, 0, 0xFF);

static inline Xplicit::Auth::XplicitID XPLICIT_INVALID_ID(0xFF, 0xFFFFFF);

static inline Xplicit::Auth::XplicitID& GetXplicitID(const std::size_t player_index)
{
	Xplicit::NetworkServerComponent* network = Xplicit::ComponentManager::get_singleton_ptr()->get<Xplicit::NetworkServerComponent>("NetworkServerComponent");

	if (network)
		return network->get(player_index)->xplicit_id;

	return XPLICIT_INVALID_ID;
}