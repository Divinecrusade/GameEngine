#pragma once

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "Dwrite")

#include "KeyColor.hpp"
#include "Surface.hpp"
#include "SurfaceView.hpp"
#include "DWriteFontNames.hpp"
#include "DWriteFontStyles.hpp"
#include "DWriteFontStretch.hpp"

#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <stdexcept>
#include <cassert>
#include <unordered_map>
#include <algorithm>
#include <functional>


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif // !HINST_THISCOMPONENT


namespace std 
{
    template<> struct hash<GameEngine::KeyColor> 
    {
        std::size_t operator() (GameEngine::KeyColor const& arg) const
        {           
            return hash<uint32_t>{}(arg.get_encoded());
        }
    };

    template<> struct hash<GameEngine::KeyColor const*>
    {
        std::size_t operator() (GameEngine::KeyColor const* arg) const
        {
            return hash<unsigned long long>{}(reinterpret_cast<unsigned long long>(arg));
        }
    };

    template<> struct hash<ID2D1Bitmap*>
    {
        std::size_t operator() (ID2D1Bitmap* arg) const
        {
            return hash<unsigned long long>{}(reinterpret_cast<unsigned long long>(arg));
        }
    };
}


namespace GameEngine
{    
    template<class Interface>
    concept Releasable = requires (Interface& instance)
    {
        instance.Release();
    };

    class Direct2DFactory final
    {
    private:

        template<Releasable Interface>
        static void safe_release(Interface*& releasing_resource)
        {
            if (releasing_resource != nullptr)
            {
                releasing_resource->Release();
                releasing_resource = nullptr;
            }
        }

        template<Releasable... Interfaces>
        static void safe_release(Interfaces*&... releasing_resources)
        {
            constexpr int DUMMY{ 0 };

            (void)std::initializer_list<int>
            {
                (
                    safe_release(releasing_resources),
                    DUMMY
                )
                ...
            };
        }

        template<class... Containers>
        static void containers_safe_release(Containers&... container)
        {
            constexpr int DUMMY{ 0 };

            (void)std::initializer_list<int>
            { 
                (
                    std::ranges::for_each(container, [](auto& val) { safe_release(val.second); }),
                    container.clear(),
                    DUMMY
                )
                ... 
            };
        }

        static D2D1::ColorF get_color(Colour c)
        {
            static constexpr float MAX_COLOR_DEPTH{ 255.f };
            return D2D1::ColorF{ c[Colour::ComponentIndex::R] / MAX_COLOR_DEPTH, c[Colour::ComponentIndex::G] / MAX_COLOR_DEPTH, c[Colour::ComponentIndex::B] / MAX_COLOR_DEPTH, c[Colour::ComponentIndex::A] / MAX_COLOR_DEPTH };
        }

        static D2D1_PIXEL_FORMAT const& get_pixel_format() noexcept
        {
            static D2D1_PIXEL_FORMAT PIXEL_FORMAT{ D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED) };
            
            return PIXEL_FORMAT;
        }

        static constexpr WCHAR const* FONT_NAMES[]{ L"Verdana", L"Gabriola" };

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
        ID2D1PathGeometry&     get_geometry();
        ID2D1SolidColorBrush&  get_brush(KeyColor const& key);
        ID2D1Bitmap&           get_bitmap(SurfaceView srf);
        ID2D1BitmapBrush&      get_bitmapbrush(ID2D1Bitmap& bitmap);
        IDWriteTextFormat&     get_text_format(DWriteFontNames font, FLOAT font_size, int font_weight, DWriteFontStyles style, DWriteFontStretch stretch = DWriteFontStretch::NORMAL);

        void               open_sink();
        void               close_sink();
        ID2D1GeometrySink& get_sink();
        
        D2D1_PIXEL_FORMAT const& PIXEL_FORMAT{ get_pixel_format() };

    private:
    
        HWND const attached_window;
        ID2D1Factory* d2d_factory{ nullptr };
        IDWriteFactory* dwrite_factory{ nullptr };
        ID2D1GeometrySink* sink{ nullptr };

        ID2D1HwndRenderTarget* render_target{ nullptr };
        ID2D1PathGeometry*     geom{ nullptr };
        std::unordered_map<KeyColor, ID2D1SolidColorBrush*> brushes;
        std::unordered_map<KeyColor const*, ID2D1Bitmap*>   bitmaps;
        std::unordered_map<ID2D1Bitmap*, ID2D1BitmapBrush*> bitmapbrushes;
        std::vector<std::pair<DWriteFontNames, IDWriteTextFormat*>> text_formats;
    };
}