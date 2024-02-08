#pragma once

#pragma comment(lib, "d2d1.lib")

#include "KeyColor.hpp"
#include "ISurface.hpp"

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

    template<> struct hash<GameEngine::KeyColor const*>
    {
        size_t operator() (GameEngine::KeyColor const* arg) const
        {
            return hash<unsigned long>{}(reinterpret_cast<unsigned long>(arg));
        }
    };

    template<> struct hash<ID2D1Bitmap*>
    {
        size_t operator() (ID2D1Bitmap* arg) const
        {
            return hash<unsigned long>{}(reinterpret_cast<unsigned long>(arg));
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

        static D2D1_PIXEL_FORMAT const& get_pixel_format() noexcept
        {
            static D2D1_PIXEL_FORMAT PIXEL_FORMAT{ D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED) };
            
            return PIXEL_FORMAT;
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
        ID2D1PathGeometry& get_geometry();
        ID2D1SolidColorBrush& get_brush(KeyColor const& key);
        ID2D1Bitmap& get_bitmap(GameEngine::Interfaces::ISurface const& srf);
        ID2D1BitmapBrush& get_bitmapbrush(ID2D1Bitmap& bitmap);
        ID2D1GeometrySink& open_sink();
        void close_sink();

        void free_resources();

        D2D1_PIXEL_FORMAT const& PIXEL_FORMAT{ get_pixel_format() };

    private:
    
        HWND const attached_window;
        ID2D1Factory* d2d_factory{ nullptr };
        ID2D1GeometrySink* sink{ nullptr };

        ID2D1HwndRenderTarget* render_target{ nullptr };
        ID2D1PathGeometry*     geom{ nullptr };
        std::unordered_map<KeyColor, ID2D1SolidColorBrush*> brushes;
        std::unordered_map<KeyColor const*, ID2D1Bitmap*>   bitmaps;
        std::unordered_map<ID2D1Bitmap*, ID2D1BitmapBrush*> bitmapbrushes;
    };
}