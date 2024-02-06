#pragma once

#pragma comment(lib, "d2d1.lib")

#include "KeyColor.hpp"

#include <d2d1.h>
#include <wincodec.h>
#include <stdexcept>
#include <cassert>
#include <unordered_map>
#include <algorithm>


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif // !HINST_THISCOMPONENT


namespace std 
{
    template<> struct hash<GameEngine::KeyColor> 
    {
        size_t operator() (GameEngine::KeyColor const& arg) const
        {           
            return hash<uint32_t>{}(arg.get_encoded());
        }
    };
}


namespace GameEngine
{
    template<class Interface>
    concept Releasable =
        requires (Interface & instance)
    {
        instance.Release();
    };

    class Direct2DFactory
    {
    private:

        template<Releasable Interface>
        __forceinline static void safe_release(Interface*& realising_resource)
        {
            if (realising_resource != nullptr)
            {
                realising_resource->Release();
                realising_resource = nullptr;
            }
        }

        __forceinline static D2D1::ColorF get_color(Colour c)
        {
            static constexpr float MAX_COLOR_DEPTH{ 255.f };
            return D2D1::ColorF{ c[Colour::ComponentIndex::R] / MAX_COLOR_DEPTH, c[Colour::ComponentIndex::G] / MAX_COLOR_DEPTH, c[Colour::ComponentIndex::B] / MAX_COLOR_DEPTH, c[Colour::ComponentIndex::A] / MAX_COLOR_DEPTH };
        }

    public:

        Direct2DFactory() = delete;
        Direct2DFactory(HWND attached_window);
        Direct2DFactory(Direct2DFactory const&) = delete;
        Direct2DFactory(Direct2DFactory&&) = delete;

        Direct2DFactory& operator=(Direct2DFactory const&) = delete;
        Direct2DFactory&& operator=(Direct2DFactory&&) = delete;

        ~Direct2DFactory();

        RECT get_render_area_size() const noexcept;

        ID2D1HwndRenderTarget& get_render_target();
        ID2D1SolidColorBrush& get_brush(KeyColor const& key);
        void free_resources();


    private:
    
        HWND const attached_window;
        ID2D1Factory* d2d_factory{ nullptr };

        ID2D1HwndRenderTarget* render_target{ nullptr };
        std::unordered_map<KeyColor, ID2D1SolidColorBrush*> brushes;
    };
}