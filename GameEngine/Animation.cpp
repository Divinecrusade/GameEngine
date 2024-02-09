#include "Animation.hpp"


// TODO: test with reversed bmp
GameEngine::Animation::Animation(std::filesystem::path const& sprites_sheet_src, size_t frame_width, size_t frame_height, float frame_duration, size_t n, FramesAlignment direction, Geometry::Vector2D<int> start_point)
:
frame_duration{ frame_duration }
{
    auto sprites_sheet{ Surface::go_to_pixels(sprites_sheet_src) };

    if (n == UNKNOWN_N)
    {
        switch (direction)
        {
            case GameEngine::Animation::FramesAlignment::HORIZONTAL: n = static_cast<size_t>(sprites_sheet.width - start_point.x) / frame_width; break;
            case GameEngine::Animation::FramesAlignment::VERTICAL: n = static_cast<size_t>(sprites_sheet.height - start_point.y) / frame_height; break;
        }
    }

    int y_start{ };
    int y_end{ };
    if (sprites_sheet.dy > 0)
    {
        y_start = 0;
        y_end = static_cast<int>(frame_height);
    }
    else
    {
        y_start = static_cast<int>(frame_height) - 1;
        y_end = 0;
    }

    frames.reserve(n);
    sprites_sheet.fin.seekg((start_point.x + start_point.y * (sprites_sheet.width)) * sprites_sheet.pixel_size + start_point.y * sprites_sheet.padding, std::ifstream::cur);
    auto frame_beg{ sprites_sheet.fin.tellg() };
    for (size_t i{ 0U }; i != n; ++i)
    {
        std::shared_ptr<Colour[]> frame{ new Colour[frame_width * frame_height] };

        for (int y{ y_start }; y != y_end; y += sprites_sheet.dy, sprites_sheet.fin.seekg((sprites_sheet.width - static_cast<int>(frame_width)) * sprites_sheet.pixel_size + sprites_sheet.padding, std::ifstream::cur))
        {
            for (int x{ 0 }; x != static_cast<int>(frame_width); ++x)
            {
                uint32_t rgba{ };

                if (sprites_sheet.pixel_size == 4) sprites_sheet.fin.read(reinterpret_cast<char*>(&rgba), sizeof(rgba));
                else if (sprites_sheet.pixel_size == 3) rgba = Colour::encode(sprites_sheet.fin.get(), sprites_sheet.fin.get(), sprites_sheet.fin.get(), Colour::MAX_COLOUR_DEPTH);
            #ifdef _DEBUG
                else assert(false);
            #endif // DEBUG

                frame[static_cast<size_t>(y) * frame_width + x] = Colour{ rgba };
            }
        }
        frames.emplace_back(frame_width, frame_height, frame);
        
        switch (direction)
        {
            case FramesAlignment::HORIZONTAL: frame_beg += static_cast<int>(frame_width) * sprites_sheet.pixel_size; break;
            case FramesAlignment::VERTICAL: frame_beg += static_cast<int>(frame_width) * static_cast<int>(frame_height) * sprites_sheet.pixel_size + sprites_sheet.padding * static_cast<int>(frame_height); break;
        }
        sprites_sheet.fin.seekg(frame_beg, std::ifstream::beg);
    }
    cur_frame_index = 0U;
}

void GameEngine::Animation::update(float dt)
{
    elapsed_duration += dt;
    while (elapsed_duration >= frame_duration)
    {
        toggle_frame();
        elapsed_duration -= frame_duration;
    }
}

void GameEngine::Animation::draw(Interfaces::IGraphics2D& gfx, Geometry::Vector2D<int> const& pos, Colour chroma) const
{
    gfx.draw_sprite_excluding_color(pos, frames[cur_frame_index], chroma);
}

void GameEngine::Animation::toggle_frame() noexcept
{
    cur_frame_index = (cur_frame_index + 1U) % frames.size();
}
