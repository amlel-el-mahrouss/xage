#include "pch.h"
#include "RenderingPage.h"
#if __has_include("RenderingPage.g.cpp")
#include "RenderingPage.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::XPXPlayerXBOX::implementation
{
    int32_t RenderingPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void RenderingPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void RenderingPage::ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        Button().Content(box_value(L"Clicked"));
    }
}
