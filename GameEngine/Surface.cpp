#include "Surface.hpp"


namespace GameEngine
{
    std::tuple<std::ifstream, size_t const, size_t const, bool, int, int> Surface::go_to_pixels(std::filesystem::path const& img_src)
    {
        if (!std::filesystem::exists(img_src)) throw std::runtime_error{ "Image not found" };
        if (std::find_if(std::cbegin(SUPPORTED_EXTENSIONS), std::cend(SUPPORTED_EXTENSIONS),
            [ext = img_src.extension().native()](wchar_t const* const& s)
            {
                return wcscmp(ext.c_str(), s) == 0;
            }) == std::cend(SUPPORTED_EXTENSIONS)) throw std::runtime_error{ "Unsupported image extension" };

        std::ifstream fin{ img_src, std::ifstream::binary };

        BITMAPFILEHEADER bmp_header{ };
        BITMAPINFOHEADER bmp_info{ };

        fin.read(reinterpret_cast<char*>(&bmp_header), sizeof(bmp_header));
        fin.read(reinterpret_cast<char*>(&bmp_info), sizeof(bmp_info));

        int const pixel_size{ bmp_info.biBitCount / 8 };
        if (std::find(std::cbegin(SUPPORTED_PIXEL_SIZES), std::cend(SUPPORTED_PIXEL_SIZES), pixel_size) == std::cend(SUPPORTED_PIXEL_SIZES))
            throw std::runtime_error{ "Not supported colour depth" };
        int const padding{ (4 - (bmp_info.biWidth * pixel_size % 4)) % 4 };
        fin.seekg(bmp_header.bfOffBits, std::ifstream::beg);

        return std::make_tuple(std::move(fin), static_cast<size_t>(bmp_info.biWidth), static_cast<size_t>(bmp_info.biHeight < 0 ? -bmp_info.biHeight : bmp_info.biHeight), bmp_info.biHeight < 0, padding, pixel_size);
    }

    Surface::Surface(std::filesystem::path const& img_src)
    {
        constexpr int IMG_FIN{ 0 };
        constexpr int IMG_WIDTH{ 1 };
        constexpr int IMG_HEIGHT{ 2 };
        constexpr int IMG_IS_REVERSED{ 3 };
        constexpr int IMG_PADDING{ 4 };
        constexpr int IMG_PIXEL_SIZE{ 5 };

        auto img{ go_to_pixels(img_src) };

        this->width = static_cast<size_t>(std::get<IMG_WIDTH>(img));
        this->height = static_cast<size_t>(std::get<IMG_HEIGHT>(img));
        auto tmp_buffer{ std::shared_ptr<Colour[]>(new Colour[this->width * this->height]) };
        
        size_t y_start{ };
        size_t y_end{ };
        int dy{ };
        if (std::get<IMG_IS_REVERSED>(img))
        {
            y_start = 0;
            y_end = height;
            dy = 1;
        }
        else
        {
            y_start = height - 1U;
            y_end = 0;
            dy = -1;
        }

        for (size_t y{ y_start }; y != y_end; y += dy)
        {
            for (size_t x{ 0 }; x != width; ++x)
            {
                uint32_t rgba{ };

                if (std::get<IMG_PIXEL_SIZE>(img) == 4) std::get<IMG_FIN>(img).read(reinterpret_cast<char*>(&rgba), sizeof(rgba));
                else if (std::get<IMG_PIXEL_SIZE>(img) == 3) rgba = Colour::encode(static_cast<uint8_t>(std::get<IMG_FIN>(img).get()), static_cast<uint8_t>(std::get<IMG_FIN>(img).get()), static_cast<uint8_t>(std::get<IMG_FIN>(img).get()), Colour::MAX_COLOUR_DEPTH);
            #ifdef _DEBUG
                else assert(false);
            #endif // DEBUG

                tmp_buffer[static_cast<ptrdiff_t>(y * width + x)] = Colour{ rgba };
            }
            std::get<IMG_FIN>(img).seekg(std::get<IMG_PADDING>(img), std::ifstream::cur);
        }
        this->buffer = std::move(tmp_buffer);
    }

    GameEngine::Surface::Surface(size_t width, size_t height, std::shared_ptr<Colour const[]> buffer)
    :
    width{ width },
    height{ height },
    buffer{ buffer }
    { }

    Surface::Surface(Surface const& srf)
    :
    width { srf.width  },
    height{ srf.height },
    buffer{ srf.buffer }
    { }

    Surface::Surface(Surface&& tmp) noexcept
    {
        this->swap(std::move(tmp));
    }

    Surface& Surface::operator=(Surface const& srf)
    {
        width = srf.width;
        height = srf.height;
        buffer = srf.buffer;

        return *this;
    }

    Surface& GameEngine::Surface::operator=(Surface&& tmp) noexcept
    {
        this->swap(std::move(tmp));

        return *this;
    }

    std::shared_ptr<Colour const[]> Surface::get_pixels() const
    {
        return std::shared_ptr<Colour const[]>(buffer);
    }

    size_t Surface::get_width() const noexcept
    {
        return width;
    }

    size_t Surface::get_height() const noexcept
    {
        return height;
    }

    void Surface::swap(Surface&& tmp) noexcept
    {
        std::swap(width, tmp.width);
        std::swap(height, tmp.height);
        std::swap(buffer, tmp.buffer);
    }
}