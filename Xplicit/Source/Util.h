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

static inline Xplicit::Color<float> Black(0, 0, 0, 255);
static inline Xplicit::Color<float> White(255, 255, 255, 255);
static inline Xplicit::Color<float> Red(255, 0, 0, 255);

static inline Xplicit::Auth::XplicitID XPLICIT_INVALID_ID(0xFF, 0xFFFFFF);

static inline Xplicit::Auth::XplicitID& GetXplicitID(const std::size_t player_index)
{
	Xplicit::NetworkServerComponent* network = Xplicit::ComponentManager::get_singleton_ptr()->get<Xplicit::NetworkServerComponent>("NetworkServerComponent");

	if (network)
		return network->get(player_index)->xplicit_id;

	return XPLICIT_INVALID_ID;
}