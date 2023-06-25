/*
 * =====================================================================
 *
 *			XplicitPlayer
 *			Copyright Xplicit Corporation, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include <CommonEngine.h>

#include "Application.h"
#include "Mesh.h"

namespace Xplicit::Player
{
    class GameObject 
    {
    public:
        GameObject() = default;
        virtual ~GameObject() = default;

        XPLICIT_COPY_DEFAULT(GameObject);

    public:
        virtual GameObject& copy() { return *this; }
        virtual GameObject&& move() { return std::move(*this); }

        virtual const char* name() { return("GameObject"); }
        virtual const char* replicated() { return "GameObject"; }

    public:
        virtual void* create_child(const char* name) = 0;
        virtual int destroy_child(void* name) = 0;

    };
}