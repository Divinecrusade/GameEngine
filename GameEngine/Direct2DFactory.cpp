#include "Direct2DFactory.hpp"


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
        safe_release(render_target);
        containers_safe_release(brushes);
        invalidate_resources();
        safe_release(d2d_factory);
        safe_release(geom);
        safe_release(sink);
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

    ID2D1Bitmap& Direct2DFactory::get_bitmap(GameEngine::Interfaces::ISurface const& srf)
    {
        assert(render_target);

        auto img{ srf.get_pixels() };
        if (!bitmaps.contains(img.get()))
        {
            ID2D1Bitmap* bmp_img{ nullptr };
            FLOAT dpiX{ };
            FLOAT dpiY{ };
            render_target->GetDpi(&dpiX, &dpiY);

            render_target->CreateBitmap
            (
                D2D1::SizeU(srf.get_width(), srf.get_height()),
                reinterpret_cast<void const*>(img.get()),
                srf.get_width() * sizeof(GameEngine::Colour),
                D2D1_BITMAP_PROPERTIES{ PIXEL_FORMAT, dpiX, dpiY },
                &bmp_img
            );
            bitmaps.insert({ img.get(), bmp_img});
        }

        return *bitmaps[img.get()];
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

    void Direct2DFactory::open_sink()
    {
        assert(!sink);

        safe_release(geom);
        auto hr = get_geometry().Open(&sink);

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
    
    void Direct2DFactory::invalidate_resources()
    {
        containers_safe_release(bitmapbrushes, bitmaps);
    }
}