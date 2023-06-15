/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 *			File: IMeshable.h
 *			Purpose: Mesh Property
 *
 * =====================================================================
 */

#pragma once

#include <Xplicit.h>
#include <Root.h>

namespace Xplicit::Player
{
	/* this classes gives an component a model. */
	class IMeshable
	{
	public:
		IMeshable() = delete;

		IMeshable(const char* path);
		virtual ~IMeshable();

	public:
		IMeshable& operator=(const IMeshable&) = default;
		IMeshable(const IMeshable&) = default;

	protected:
		IAnimatedMeshSceneNode* mNode; // Model Data pointer, generic
		IAnimatedMesh* mMdl; // Model Data pointer, generic

	};
}