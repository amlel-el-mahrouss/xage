/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "Xplicit.h"
#include "Root.h"

namespace Xplicit::ImGui
{
    class XPLICIT_API ImGuiSingleton final
    {
        explicit ImGuiSingleton() = default;

    public:
        ~ImGuiSingleton() = default;

    public:
        XPLICIT_COPY_DEFAULT(ImGuiSingleton);
        

    }
}