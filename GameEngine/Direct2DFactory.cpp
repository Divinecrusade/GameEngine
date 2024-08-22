#include "Direct2DFactory.hpp"

#include <algorithm>
#include <ranges>


namespace GameEngine
{
    Direct2DFactory::Direct2DFactory(HWND attached_window)
    :
    attached_window{ attached_window }
    {
        if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d_factory)))
            throw std::runtime_error{ "Failed to create D2D1 factory" };
    }

    Direct2DFactory::~Direct2DFactory()
    {
        containers_safe_release(brushes, bitmapbrushes, bitmaps, text_formats);
        safe_release(render_target, d2d_factory, geom, sink, dwrite_factory);
    }

    RECT Direct2DFactory::get_render_area_size() const noexcept
    {
        RECT render_area{ };
        GetClientRect(attached_window, &render_area);

        return render_area;
    }

    ID2D1HwndRenderTarget& Direct2DFactory::get_render_target()
    {   
        if (!render_target)
        {
            RECT const render_area{ get_render_area_size() };
            D2D1_SIZE_U const d2d1_size{ D2D1::SizeU(static_cast<UINT32>(render_area.right) - static_cast<UINT32>(render_area.left), static_cast<UINT32>(render_area.bottom) - static_cast<UINT32>(render_area.top)) };
            D2D1_HWND_RENDER_TARGET_PROPERTIES const render_properties{ D2D1::HwndRenderTargetProperties(attached_window, d2d1_size) };
            if (FAILED(d2d_factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), render_properties, &render_target)))
                throw std::runtime_error{ "Failed to create D2D1 render target" };
        }
    
        return *render_target;
    }

    ID2D1PathGeometry& Direct2DFactory::get_geometry()
    {
        if (!geom)
        {
            d2d_factory->CreatePathGeometry(&geom);
        }
        
        return *geom;
    }

    ID2D1SolidColorBrush& Direct2DFactory::get_brush(KeyColor const& key)
    {
        assert(render_target);

        if (!brushes.contains(key))
        {
            ID2D1SolidColorBrush* tmp{ nullptr };
            D2D1::ColorF const c{ get_color(key) };
            render_target->CreateSolidColorBrush(c, &tmp);
            brushes.insert({ key, tmp });
        }

        return *brushes[key];
    }

    ID2D1Bitmap& Direct2DFactory::get_bitmap(SurfaceView srf)
    {
        assert(render_target);

        KeyColor const* const raw_ptr_to_srf{ std::to_address(srf.begin()) };
        if (!bitmaps.contains(raw_ptr_to_srf))
        {
            ID2D1Bitmap* bmp_img{ nullptr };
            FLOAT dpiX{ };
            FLOAT dpiY{ };
            render_target->GetDpi(&dpiX, &dpiY);

            render_target->CreateBitmap
            (
                D2D1::SizeU(static_cast<UINT32>(srf.get_width()), static_cast<UINT32>(srf.get_height())),
                reinterpret_cast<void const*>(raw_ptr_to_srf),
                static_cast<UINT32>(srf.get_width()) * sizeof(GameEngine::Colour),
                D2D1_BITMAP_PROPERTIES{ PIXEL_FORMAT, dpiX, dpiY },
                &bmp_img
            );
            bitmaps.insert({ raw_ptr_to_srf, bmp_img});
        }

        return *bitmaps[raw_ptr_to_srf];
    }

    ID2D1BitmapBrush& GameEngine::Direct2DFactory::get_bitmapbrush(ID2D1Bitmap& bitmap)
    {
        assert(render_target);

        if (!bitmapbrushes.contains(&bitmap))
        {
            ID2D1BitmapBrush* brush{ nullptr };
            render_target->CreateBitmapBrush
            (
                &bitmap,
                D2D1::BitmapBrushProperties
                (
                    D2D1_EXTEND_MODE_CLAMP,
                    D2D1_EXTEND_MODE_CLAMP,
                    D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR
                ),
                &brush
            );
            bitmapbrushes.insert({ &bitmap, brush });
        }

        return *bitmapbrushes[&bitmap];
    }

    IDWriteTextFormat& Direct2DFactory::get_text_format(DWriteFontNames font, FLOAT font_size, int font_weight, DWriteFontStyles style, DWriteFontStretch stretch)
    {
        auto it{ std::ranges::find_if(text_formats, [&](auto const& text_format) 
        { 
            return text_format.second->GetFontSize() == font_size && 
                   text_format.second->GetFontWeight() == font_weight && 
                   static_cast<DWriteFontStyles>(text_format.second->GetFontStyle()) == style && 
                   text_format.first == font;
        }) };
        if (it == text_formats.end())
        {
            if (!dwrite_factory)
                if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(dwrite_factory), reinterpret_cast<IUnknown**>(&dwrite_factory))))
                    throw std::runtime_error{ "Failed to create DWrite factory" };

            auto& new_pair{ text_formats.emplace_back(font, nullptr) };

            if (FAILED(dwrite_factory->CreateTextFormat(FONT_NAMES[static_cast<int>(font)], nullptr, static_cast<DWRITE_FONT_WEIGHT>(font_weight), static_cast<DWRITE_FONT_STYLE>(style), static_cast<DWRITE_FONT_STRETCH>(stretch), font_size, L"", &new_pair.second)))
                throw std::runtime_error{ "Failed to create text format" };

            it = text_formats.end() - 1;
        }
        return *it->second;
    }

    void Direct2DFactory::open_sink()
    {
        assert(!sink);

        safe_release(geom);
        get_geometry().Open(&sink);

        assert(sink);
    }

    void Direct2DFactory::close_sink()
    {
        assert(sink);

        sink->Close();
        safe_release(sink);
    }

    ID2D1GeometrySink& GameEngine::Direct2DFactory::get_sink()
    {
        assert(sink);

        return *sink;
    }
}