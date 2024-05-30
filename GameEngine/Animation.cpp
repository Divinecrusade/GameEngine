#include "Animation.hpp"


namespace GameEngine
{
    std::vector<Surface> Animation::get_frames_from_sprites_sheet(std::filesystem::path const& sprites_sheet_src, std::size_t frame_width, std::size_t frame_height, std::size_t n, FramesAlignment direction, Geometry::Vector2D<int> start_point)
    {
        assert(start_point.x >= 0);
        assert(start_point.y >= 0);
        Surface::BMP_HANDLER sprites_sheet{ Surface::parse_img(sprites_sheet_src) };

        if (n == UNKNOWN_N)
        {
            switch (direction)
            {
                case GameEngine::Animation::FramesAlignment::HORIZONTAL: n = (sprites_sheet.get_width()  - static_cast<std::size_t>(start_point.x)) / frame_width;  break;
                case GameEngine::Animation::FramesAlignment::VERTICAL:   n = (sprites_sheet.get_height() - static_cast<std::size_t>(start_point.y)) / frame_height; break;
            }
        }

        std::vector<Surface> frames{  };
        frames.reserve(n);
        sprites_sheet.get_stream().seekg
        (
            static_cast<std::streamoff>
            (
                (
                    static_cast<std::size_t>(start_point.x) + 
                    static_cast<std::size_t>(start_point.y) * 
                    sprites_sheet.get_width()
                ) * 
                sprites_sheet.get_pixel_size() +
                static_cast<std::size_t>(start_point.y) * 
                sprites_sheet.get_padding()
            ), std::ifstream::cur);
        auto frame_beg{ sprites_sheet.get_stream().tellg()};
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
            frames.emplace_back(std::move(frame), frame_height, frame_width);

            switch (direction)
            {
                case FramesAlignment::HORIZONTAL: frame_beg += static_cast<std::streamoff>(frame_width * sprites_sheet.get_pixel_size()); break;
                case FramesAlignment::VERTICAL:   frame_beg += static_cast<std::streamoff>(frame_width * frame_height * sprites_sheet.get_pixel_size()) + sprites_sheet.get_padding() * static_cast<std::streamoff>(frame_height); break;
            }
            sprites_sheet.get_stream().seekg(frame_beg, std::ifstream::beg);
        }

        return frames;
    }

    Animation::Animation(std::shared_ptr<std::vector<Surface> const> const& frames, float frame_duration)
    :
    frames{ frames },
    frame_duration{ frame_duration }
    { }

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

    Surface const& GameEngine::Animation::get_cur_frame() const noexcept
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