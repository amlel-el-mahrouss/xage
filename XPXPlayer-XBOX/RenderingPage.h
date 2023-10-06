#pragma once

#include "RenderingPage.g.h"

namespace winrt::XPXPlayerXBOX::implementation
{
    struct RenderingPage : RenderingPageT<RenderingPage>
    {
        RenderingPage() 
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void ClickHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::XPXPlayerXBOX::factory_implementation
{
    struct RenderingPage : RenderingPageT<RenderingPage, implementation::RenderingPage>
    {
    };
}
