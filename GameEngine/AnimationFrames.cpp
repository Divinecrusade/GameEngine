#include "AnimationFrames.hpp"


namespace GameEngine
{
    AnimationFrames::AnimationFrames(std::filesystem::path const& sprites_sheet_src, std::size_t frame_width, std::size_t frame_height, std::optional<std::size_t> n, std::optional<FramesAlignment> direction, std::optional<Geometry::Vector2D<int>> start_point)
    {
        assert(frame_width > 0);
        assert(frame_height > 0);

        Surface::BMP_HANDLER sprites_sheet{ Surface::parse_img(sprites_sheet_src) };

        if (!n)
        {
            switch (direction.value_or(DEFAULT_DIRECTION))
            {
                case FramesAlignment::HORIZONTAL: n = (sprites_sheet.width  - static_cast<std::size_t>(start_point.value_or(DEFAULT_START_POINT).x)) / frame_width;  break;
                case FramesAlignment::VERTICAL:   n = (sprites_sheet.height - static_cast<std::size_t>(start_point.value_or(DEFAULT_START_POINT).y)) / frame_height; break;
            }
        }

        assert(n.has_value());

        frames.reserve(*n);
        sprites_sheet.fin.seekg
        (
            static_cast<std::streamoff>
            (
                (
                    static_cast<std::size_t>(start_point.value_or(DEFAULT_START_POINT).x) +
                    static_cast<std::size_t>(start_point.value_or(DEFAULT_START_POINT).y) *
                    sprites_sheet.width
                ) *
                sprites_sheet.get_pixel_size() +
                static_cast<std::size_t>(start_point.value_or(DEFAULT_START_POINT).y) *
                sprites_sheet.padding
            ), std::ifstream::cur);

        auto frame_beg{ sprites_sheet.fin.tellg() };
        for (std::size_t i{ 0U }; i != n; ++i)
        {
            std::unique_ptr<Colour[]> frame{ std::make_unique<Colour[]>(frame_width * frame_height) };

            for (
                    std::size_t y{ sprites_sheet.get_pixels_table_y_start() }; 
                    y != sprites_sheet.get_pixels_table_y_end(); 
                    y += sprites_sheet.get_pixels_table_dy(),
                    sprites_sheet.fin.seekg
                    (
                        static_cast<std::streamoff>
                        (
                            (sprites_sheet.width - frame_width) *
                            sprites_sheet.get_pixel_size() +
                            sprites_sheet.padding
                        ),
                        std::ifstream::cur
                    )
                )
            {
                for (std::size_t x{ 0U }; x != frame_width; ++x)
                {
                    frame[y * frame_width + x] = Colour
                    {
                        Colour::encode
                        (
                            static_cast<uint8_t>(sprites_sheet.fin.get()),
                            static_cast<uint8_t>(sprites_sheet.fin.get()),
                            static_cast<uint8_t>(sprites_sheet.fin.get()),
                            Colour::MAX_COLOUR_DEPTH
                        )
                    };
                }
            }
            frames.emplace_back(std::move(frame), frame_height, frame_width);

            switch (direction.value_or(FramesAlignment::HORIZONTAL))
            {
                case FramesAlignment::HORIZONTAL: frame_beg += static_cast<std::streamoff>(frame_width * sprites_sheet.get_pixel_size()); break;
                case FramesAlignment::VERTICAL:   frame_beg += static_cast<std::streamoff>(frame_width * frame_height * sprites_sheet.get_pixel_size()) + sprites_sheet.padding * static_cast<std::streamoff>(frame_height); break;
            }
            sprites_sheet.fin.seekg(frame_beg, std::ifstream::beg);
        }
    }
}
