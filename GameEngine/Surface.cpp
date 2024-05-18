#include "Surface.hpp"


namespace GameEngine
{
    Surface::iterator Surface::begin() noexcept
    {
        return iterator{ buffer.get() };
    }

    Surface::iterator Surface::end() noexcept
    {
        return iterator{ buffer.get() + n_cols * n_rows };
    }

    std::pair<Surface::iterator const, Surface::iterator const> Surface::operator[](std::size_t i_row) noexcept
    {
        iterator const begin{ buffer.get() + n_cols * i_row };
        iterator const end{ begin + n_cols };

        return std::make_pair(begin, end);
    }

    Surface::const_iterator Surface::begin() const noexcept
    {
        return this->cbegin();
    }

    Surface::const_iterator Surface::end() const noexcept
    {
        return this->cend();
    }

    Surface::const_iterator Surface::cbegin() const noexcept
    {
        return const_iterator{ buffer.get() };
    }

    Surface::const_iterator Surface::cend() const noexcept
    {
        return const_iterator{ buffer.get() + n_cols * n_rows };
    }

    std::pair<Surface::const_iterator const, Surface::const_iterator const> Surface::operator[](std::size_t i_row) const noexcept
    {
        const_iterator const begin{ buffer.get() + n_cols * i_row };
        const_iterator const end  { begin + n_cols };

        return std::make_pair(begin, end);
    }

    Surface::reverse_iterator Surface::rbegin() noexcept
    {
        return reverse_iterator{ buffer.get() + n_cols * n_rows - 1U };
    }

    Surface::reverse_iterator Surface::rend() noexcept
    {
        return reverse_iterator{ buffer.get() - 1U };
    }

    Surface::const_reverse_iterator Surface::rbegin() const noexcept
    {
        return crbegin();
    }

    Surface::const_reverse_iterator Surface::rend() const noexcept
    {
        return crend();
    }

    Surface::const_reverse_iterator Surface::crbegin() const noexcept
    {
        return const_reverse_iterator{ buffer.get() + n_cols * n_rows - 1U };
    }

    Surface::const_reverse_iterator Surface::crend() const noexcept
    {
        return const_reverse_iterator{ buffer.get() - 1U };
    }

    std::tuple<std::ifstream, std::size_t const, std::size_t const, bool const, std::streamoff const, int const> Surface::parse_img(std::filesystem::path const& img_src)
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

        constexpr std::streamoff ALIGNMENT{ 4LL };
        int const pixel_size{ bmp_info.biBitCount / 8 };
        if (std::find(std::cbegin(SUPPORTED_PIXEL_SIZES), std::cend(SUPPORTED_PIXEL_SIZES), pixel_size) == std::cend(SUPPORTED_PIXEL_SIZES))
            throw std::runtime_error{ "Not supported colour depth" };
        std::streamoff const padding{ (ALIGNMENT - (static_cast<std::streamoff>(bmp_info.biWidth) * pixel_size % ALIGNMENT)) % ALIGNMENT };
        fin.seekg(bmp_header.bfOffBits, std::ifstream::beg);

        return std::make_tuple(std::move(fin), static_cast<std::size_t>(bmp_info.biWidth), static_cast<std::size_t>(bmp_info.biHeight < 0 ? -bmp_info.biHeight : bmp_info.biHeight), bmp_info.biHeight < 0L, padding, pixel_size);
    }

    std::tuple<std::size_t const, std::size_t const, int const> Surface::get_pixel_table_info(bool reversed, std::size_t height)
    {
        std::size_t y_start{ };
        std::size_t y_end{ };
        int dy{ };
        if (reversed)
        {
            y_start = 0U;
            y_end = height;
            dy = 1;
        }
        else
        {
            y_start = height - 1U;
            y_end = 0U;
            dy = -1;
        }

        return std::make_tuple(y_start, y_end, dy);
    }

    std::tuple<std::unique_ptr<Colour[]>, std::size_t, std::size_t> Surface::read_img(std::tuple<std::ifstream, std::size_t const, std::size_t const, bool const, std::streamoff const, int const>&& img)
    {
        std::unique_ptr<Colour[]> tmp_buffer{ new Colour[std::get<IMG_WIDTH>(img) * std::get<IMG_HEIGHT>(img)] };
        auto const pixel_table_info{ get_pixel_table_info(std::get<IMG_REVERSED>(img), std::get<IMG_HEIGHT>(img)) };

        for (std::size_t y{ std::get<PIXEL_TABLE_INFO_Y_START>(pixel_table_info) }; y != std::get<PIXEL_TABLE_INFO_Y_END>(pixel_table_info); y += std::get<PIXEL_TABLE_INFO_DY>(pixel_table_info))
        {
            for (std::size_t x{ 0U }; x != std::get<IMG_WIDTH>(img); ++x)
            {
                tmp_buffer[static_cast<ptrdiff_t>(y * std::get<IMG_WIDTH>(img) + x)] = Colour
                {
                    Colour::encode
                    (
                        static_cast<uint8_t>(std::get<IMG_FIN>(img).get()),
                        static_cast<uint8_t>(std::get<IMG_FIN>(img).get()),
                        static_cast<uint8_t>(std::get<IMG_FIN>(img).get()),
                        Colour::MAX_COLOUR_DEPTH
                    )
                };
            }
            std::get<IMG_FIN>(img).seekg(std::get<IMG_PADDING>(img), std::ifstream::cur);
        }

        return std::make_tuple(std::move(tmp_buffer), std::get<IMG_HEIGHT>(img), std::get<IMG_WIDTH>(img));
    }

    Surface::Surface(std::filesystem::path const& img_src)
    :
    Surface{ std::move(std::make_from_tuple<Surface>(read_img(parse_img(img_src)))) }
    { }

    Surface::Surface(std::unique_ptr<Colour[]> buffer, std::size_t n_rows, std::size_t n_cols)
    :
    buffer{ std::move(buffer) },
    n_rows{ n_rows },
    n_cols{ n_cols }
    { }

    Surface::Surface(Surface const& other)
    :
    buffer{ new Colour[other.n_rows * other.n_cols] },
    n_rows{ other.n_rows },
    n_cols{ other.n_cols }
    {
        std::copy(other.begin(), other.end(), this->begin());
    }

    Surface::Surface(Surface&& other_tmp) noexcept
    :
    buffer{ other_tmp.buffer.release() },
    n_rows{ other_tmp.n_rows },
    n_cols{ other_tmp.n_cols }
    { }

    std::size_t Surface::get_width()  const noexcept
    {
        return n_cols;
    }

    std::size_t Surface::get_height() const noexcept
    {
        return n_rows;
    }
}