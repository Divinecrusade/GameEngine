#include "Animation.hpp"


namespace GameEngine
{
    std::vector<Surface> Animation::get_frames_from_sprites_sheet(std::filesystem::path const& sprites_sheet_src, size_t frame_width, size_t frame_height, size_t n, FramesAlignment direction, Geometry::Vector2D<int> start_point)
    {
        assert(start_point.x >= 0);
        assert(start_point.y >= 0);
        auto       sprites_sheet   { Surface::parse_img(sprites_sheet_src) };
        auto const pixel_table_info{ Surface::get_pixel_table_info(std::get<Surface::IMG_REVERSED>(sprites_sheet), std::get<Surface::IMG_HEIGHT>(sprites_sheet)) };

        if (n == UNKNOWN_N)
        {
            switch (direction)
            {
                case GameEngine::Animation::FramesAlignment::HORIZONTAL: n = (std::get<Surface::IMG_WIDTH>(sprites_sheet) - static_cast<size_t>(start_point.x)) / frame_width;  break;
                case GameEngine::Animation::FramesAlignment::VERTICAL:   n = (std::get<Surface::IMG_HEIGHT>(sprites_sheet) - static_cast<size_t>(start_point.y)) / frame_height; break;
            }
        }

        std::vector<Surface> frames{  };
        frames.reserve(n);
        std::get<Surface::IMG_FIN>(sprites_sheet).seekg
        (
            static_cast<std::streamoff>
            (
                (
                    static_cast<size_t>(start_point.x) + 
                    static_cast<size_t>(start_point.y) * 
                    std::get<Surface::IMG_WIDTH>(sprites_sheet)
                ) * 
                std::get<Surface::IMG_PIXEL_SIZE>(sprites_sheet) + 
                static_cast<size_t>(start_point.y) * 
                std::get<Surface::IMG_PADDING>(sprites_sheet)
            ), std::ifstream::cur);
        auto frame_beg{ std::get<Surface::IMG_FIN>(sprites_sheet).tellg() };
        for (size_t i{ 0U }; i != n; ++i)
        {
            std::unique_ptr<Colour[]> frame{ new Colour[frame_width * frame_height] };
            
            for (size_t y{ std::get<Surface::PIXEL_TABLE_INFO_Y_START>(pixel_table_info) }; y != std::get<Surface::PIXEL_TABLE_INFO_Y_END>(pixel_table_info); y += std::get<Surface::PIXEL_TABLE_INFO_DY>(pixel_table_info),
                std::get<Surface::IMG_FIN>(sprites_sheet).seekg
                (
                    static_cast<std::streamoff>
                    (
                        (std::get<Surface::IMG_WIDTH>(sprites_sheet) - frame_width) * 
                        std::get<Surface::IMG_PIXEL_SIZE>(sprites_sheet) + 
                        std::get<Surface::IMG_PADDING>(sprites_sheet)
                    ), 
                    std::ifstream::cur))
            {
                for (size_t x{ 0U }; x != frame_width; ++x)
                {
                    frame[static_cast<ptrdiff_t>(y * frame_width + x)] = Colour
                    { 
                        Colour::encode
                        (
                            static_cast<uint8_t>(std::get<Surface::IMG_FIN>(sprites_sheet).get()),
                            static_cast<uint8_t>(std::get<Surface::IMG_FIN>(sprites_sheet).get()),
                            static_cast<uint8_t>(std::get<Surface::IMG_FIN>(sprites_sheet).get()),
                            Colour::MAX_COLOUR_DEPTH
                        )
                    };
                }
            }
            frames.emplace_back(std::move(frame), frame_height, frame_width);

            switch (direction)
            {
                case FramesAlignment::HORIZONTAL: frame_beg += static_cast<std::streamoff>(frame_width) * std::get<Surface::IMG_PIXEL_SIZE>(sprites_sheet); break;
                case FramesAlignment::VERTICAL:   frame_beg += static_cast<std::streamoff>(frame_width) * frame_height * std::get<Surface::IMG_PIXEL_SIZE>(sprites_sheet) + std::get<Surface::IMG_PADDING>(sprites_sheet) * static_cast<std::streamoff>(frame_height); break;
            }
            std::get<Surface::IMG_FIN>(sprites_sheet).seekg(frame_beg, std::ifstream::beg);
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