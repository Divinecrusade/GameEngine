#include "Surface.hpp"

#include <wingdi.h>
#include <stdexcept>
#include <fstream>

namespace GameEngine
{
    Surface::Surface(std::filesystem::path const& img_src)
    {
        if (!std::filesystem::exists(img_src)) throw std::runtime_error{ "Image not found" };
        if (img_src.extension() != SUPPORTED_EXTENSION) throw std::runtime_error{ "Unsupported image extension" };

        std::ifstream fin{ img_src, std::ifstream::binary };

        BITMAPFILEHEADER bmp_header{ };
        BITMAPINFOHEADER bmp_info{ };

        fin.read(reinterpret_cast<char*>(&bmp_header), sizeof(bmp_header));
        fin.read(reinterpret_cast<char*>(&bmp_info), sizeof(bmp_info));

        width = bmp_info.biWidth;

        int y_start{ };
        int y_end{ };
        int dy{ };
        if (bmp_info.biHeight < 0)
        {
            height = -bmp_info.biHeight;
            y_start = 0;
            y_end = height;
            dy = 1;
        }
        else
        {
            height = bmp_info.biHeight;
            y_start = height - 1U;
            dy = -1;
        }
        buffer = new Colour[width * height];
        
        int const pixel_size{ bmp_info.biBitCount / 8 };
        if (pixel_size != SUPPORTED_PIXEL_SIZE) throw std::runtime_error{ "Not supported colour mode" };
        int const padding{ (4 - (bmp_info.biWidth * pixel_size % 4)) % 4 };
        fin.seekg(bmp_header.bfOffBits, std::ifstream::beg);
        for (int y{ y_start }; y != y_end; y += dy)
        {
            for (int x{ 0 }; x != width; ++x)
            {
                uint32_t rgba{ };
                fin.read(reinterpret_cast<char*>(&rgba), sizeof(rgba));

                buffer[y * width + x] = Colour{ rgba };
            }
            fin.seekg(padding, std::ifstream::cur);
        }
    }

    GameEngine::Surface::~Surface() noexcept
    {
        delete[] buffer;
    }

    // TODO
    void Surface::draw(Interfaces::IGraphics2D& gfx, Geometry::Vector2D<int> const& pos, Colour chroma) const
    {
        assert(false);
    }

    std::vector<std::reference_wrapper<Colour const>> Surface::get_bitmap() const
    {
        std::vector<std::reference_wrapper<Colour const>> tmp;
        size_t const N{ static_cast<size_t>(width) * height };
        tmp.reserve(N);

        for (size_t i{ 0U }; i != N; ++i)
        {
            tmp.emplace_back(std::cref(buffer[i]));
        }

        return std::move(tmp);
    }

    int Surface::get_width() const noexcept
    {
        return width;
    }

    int Surface::get_height() const noexcept
    {
        return height;
    }
}