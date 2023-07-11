/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright Xplicit Inc, all rights reserved.
 *
 * =====================================================================
 */

#pragma once

#include "ImGuiUIBase.h"
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

    public:
        UIFrame* createButton(const char* name)
        {
            auto* frm = this->createFrame(name);

            //! TODO logic

            return frm;
        }

        UIFrame* createCheckBox(const char* name)
        {
            auto* frm = this->createFrame(name);

            //! TODO logic
            
            return frm;
        }

    private:
        UIFrame* createFrame(const char* name)
        {
            XPLICIT_ASSERT(name);
            UIFrame* frm = new UIFrame();
        
            frm->Name = name;
            
            return frm;
        }

    };
}