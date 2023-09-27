/*
 * =====================================================================
 *
 *			XPXNgin.AI
 *			Copyright XPX Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

namespace XPX
{
	enum
	{
		XPX_AI_FRIENDLY,
		XPX_AI_ENEMMY,
		XPX_AI_NEUTRAL,
	};

	typedef int XPX_AI;
	typedef int XPX_HEALTH;
	typedef bool XPX_BOOL;

	class NpCharacterRecipe
	{
	public:
		explicit NpCharacterRecipe() {}
		virtual ~NpCharacterRecipe() {}

		XPX_BOOL m_bHuman;

		XPI_AI m_iFriendliness;

		XPX_HEALTH m_iHealth;
		XPX_HEALTH m_iArmor;

		XPX_HEALTH m_iJumpSpeed;
		XPX_HEALTH m_iSprintSpeed;
		XPX_HEALTH m_iWalkSpeed;

	};

}