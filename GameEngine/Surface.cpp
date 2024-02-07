#include "Surface.hpp"


namespace GameEngine
{
    Surface::Surface(std::filesystem::path const& img_src)
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

        int width = bmp_info.biWidth;
        int height{ };

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
        assert(width > 0);
        this->width = static_cast<size_t>(width);
        assert(height > 0);
        this->height = static_cast<size_t>(height);
        auto buffer { std::shared_ptr<Colour[]>(new Colour[this->width * this->height]) };
        
        int const pixel_size{ bmp_info.biBitCount / 8 };
        if (std::find(std::cbegin(SUPPORTED_PIXEL_SIZES), std::cend(SUPPORTED_PIXEL_SIZES), pixel_size) == std::cend(SUPPORTED_PIXEL_SIZES)) 
            throw std::runtime_error{ "Not supported colour depth" };
        int const padding{ (4 - (bmp_info.biWidth * pixel_size % 4)) % 4 };
        fin.seekg(bmp_header.bfOffBits, std::ifstream::beg);
        for (int y{ y_start }; y != y_end; y += dy)
        {
            for (int x{ 0 }; x != width; ++x)
            {
                uint32_t rgba{ };

                if (pixel_size == 4) fin.read(reinterpret_cast<char*>(&rgba), sizeof(rgba));
                else if (pixel_size == 3) rgba = Colour::encode(fin.get(), fin.get(), fin.get(), Colour::MAX_COLOUR_DEPTH);
            #ifdef _DEBUG
                else assert(false);
            #endif // DEBUG

                buffer[static_cast<size_t>(y) * width + x] = Colour{ rgba };
            }
            fin.seekg(padding, std::ifstream::cur);
        }
        this->buffer = std::move(buffer);
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