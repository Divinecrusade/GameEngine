#include "Animation.hpp"


namespace GameEngine
{
    Animation::Animation(float animation_duration, std::filesystem::path const& sprites_sheet_src, std::size_t frame_width, std::size_t frame_height, std::optional<std::size_t> n, std::optional<FramesAlignment> direction, std::optional<Geometry::Vector2D<int>> start_point)
    :
    frame_duration{ frame_duration }
    { 
        Surface::BMP_HANDLER sprites_sheet{ Surface::parse_img(sprites_sheet_src) };

        if (!n)
        {
            switch (direction.value_or(DEFAULT_DIRECTION))
            {
                case FramesAlignment::HORIZONTAL: n.emplace((sprites_sheet.get_width() - static_cast<std::size_t>(start_point.value_or(DEFAULT_START_POINT).x))  / frame_width);  break;
                case FramesAlignment::VERTICAL:   n.emplace((sprites_sheet.get_height() - static_cast<std::size_t>(start_point.value_or(DEFAULT_START_POINT).y)) / frame_height); break;
            }
        }

        frames = std::make_shared<std::vector<Surface>>();
        frames->reserve(n.value());
        frame_duration = animation_duration / n.value();
        sprites_sheet.get_stream().seekg
        (
            static_cast<std::streamoff>
            (
                (
                    static_cast<std::size_t>(start_point.value_or(DEFAULT_START_POINT).x) +
                    static_cast<std::size_t>(start_point.value_or(DEFAULT_START_POINT).y) *
                    sprites_sheet.get_width()
                ) *
                sprites_sheet.get_pixel_size() +
                static_cast<std::size_t>(start_point.value_or(DEFAULT_START_POINT).y) *
                sprites_sheet.get_padding()
                ), std::ifstream::cur);
        auto frame_beg{ sprites_sheet.get_stream().tellg() };
        for (std::size_t i{ 0U }; i != n; ++i)
        {
            std::unique_ptr<Colour[]> frame{ new Colour[frame_width * frame_height] };

            for (std::size_t y{ sprites_sheet.get_pixels_table_y_start() }; y != sprites_sheet.get_pixels_table_y_end(); y += sprites_sheet.get_pixels_table_dy(),
                sprites_sheet.get_stream().seekg
                (
                    static_cast<std::streamoff>
                    (
                        (sprites_sheet.get_width() - frame_width) *
                        sprites_sheet.get_pixel_size() +
                        sprites_sheet.get_padding()
                    ),
                    std::ifstream::cur))
            {
                for (std::size_t x{ 0U }; x != frame_width; ++x)
                {
                    frame[y * frame_width + x] = Colour
                    {
                        Colour::encode
                        (
                            static_cast<uint8_t>(sprites_sheet.get_stream().get()),
                            static_cast<uint8_t>(sprites_sheet.get_stream().get()),
                            static_cast<uint8_t>(sprites_sheet.get_stream().get()),
                            Colour::MAX_COLOUR_DEPTH
                        )
                    };
                }
            }
            frames->emplace_back(std::move(frame), frame_height, frame_width);

            switch (direction.value_or(FramesAlignment::HORIZONTAL))
            {
                case FramesAlignment::HORIZONTAL: frame_beg += static_cast<std::streamoff>(frame_width * sprites_sheet.get_pixel_size()); break;
                case FramesAlignment::VERTICAL:   frame_beg += static_cast<std::streamoff>(frame_width * frame_height * sprites_sheet.get_pixel_size()) + sprites_sheet.get_padding() * static_cast<std::streamoff>(frame_height); break;
            }
            sprites_sheet.get_stream().seekg(frame_beg, std::ifstream::beg);
        }
    }

    void Animation::update(float dt)
    {
        elapsed_duration += dt;
        while (elapsed_duration >= frame_duration)
        {
            toggle_frame();
            elapsed_duration -= frame_duration;
        }
    }

    bool Animation::is_finished() const noexcept
    {
        return cur_frame_index == frames->size();
    }

    void Animation::reset() noexcept
    {
        elapsed_duration = 0.f;
        cur_frame_index = 0U;
    }

    SurfaceView GameEngine::Animation::get_cur_frame() const noexcept
    {
        assert(cur_frame_index < frames->size());
        return (*frames)[cur_frame_index];
    }

    inline void Animation::toggle_frame() noexcept
    {
        if (is_finished()) return;
        ++cur_frame_index;
    }
}