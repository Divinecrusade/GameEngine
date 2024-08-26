#include "SurfaceView.hpp"


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

    Surface::RowView Surface::operator[](std::size_t i_row) const noexcept
    {
        const_iterator const begin{ buffer.get() + n_cols * i_row };

        return RowView{ begin, begin + static_cast<const_iterator::difference_type>(n_cols) };
    }

    Surface::BMP_HANDLER Surface::parse_img(std::filesystem::path const& img_src)
    {
        assert(std::filesystem::exists(img_src));
        assert
        (
            std::ranges::find_if(SUPPORTED_EXTENSIONS,
            [ext = img_src.extension().native()](wchar_t const* const& s)
            {
                return wcscmp(ext.c_str(), s) == 0;
            }) != SUPPORTED_EXTENSIONS.end()
        );

        std::ifstream fin{ img_src, std::ifstream::binary };
        fin.exceptions(std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit);

        BITMAPFILEHEADER bmp_header{ };
        BITMAPINFOHEADER bmp_info{ };

        fin.read(reinterpret_cast<char*>(&bmp_header), sizeof(bmp_header));
        fin.read(reinterpret_cast<char*>(&bmp_info),   sizeof(bmp_info));

        std::size_t const pixel_size{ static_cast<std::size_t>(bmp_info.biBitCount) / 8U };

        if (pixel_size != SUPPORTED_PIXEL_SIZE) throw std::invalid_argument{ "Image pixel size (colour depth) is not supported or bmp-table is corrupted" };

        constexpr std::streamoff ALIGNMENT{ 4LL };
        std::streamoff const padding{ static_cast<std::streamoff>((ALIGNMENT - (bmp_info.biWidth * pixel_size % ALIGNMENT)) % ALIGNMENT) };
        fin.seekg(bmp_header.bfOffBits, std::ifstream::beg);

        return BMP_HANDLER{ std::move(fin), static_cast<std::size_t>(bmp_info.biWidth), static_cast<std::size_t>(bmp_info.biHeight < 0 ? -bmp_info.biHeight : bmp_info.biHeight), bmp_info.biHeight < 0L, padding };
    }

    std::tuple<std::unique_ptr<Colour[]>, std::size_t, std::size_t> Surface::read_img(BMP_HANDLER&& img)
    {
        std::unique_ptr<Colour[]> tmp_buffer{ std::make_unique<Colour[]>(img.get_width() * img.get_height()) };

        for (std::size_t y{ img.get_pixels_table_y_start() }; y != img.get_pixels_table_y_end(); y += img.get_pixels_table_dy())
        {
            for (std::size_t x{ 0U }; x != img.get_width(); ++x)
            {
                tmp_buffer[y * img.get_width() + x] = Colour
                {
                    Colour::encode
                    (
                        static_cast<uint8_t>(img.get_stream().get()),
                        static_cast<uint8_t>(img.get_stream().get()),
                        static_cast<uint8_t>(img.get_stream().get()),
                        Colour::MAX_COLOUR_DEPTH
                    )
                };
            }
            img.get_stream().seekg(img.get_padding(), std::ifstream::cur);
        }

        return std::make_tuple(std::move(tmp_buffer), img.get_height(), img.get_width());
    }

    Surface::Surface(std::filesystem::path const& img_src)
    :
    Surface{ std::make_from_tuple<Surface>(read_img(parse_img(img_src))) }
    { }

    Surface::Surface(std::unique_ptr<Colour[]>&& buffer, std::size_t n_rows, std::size_t n_cols)
    :
    buffer{ std::move(buffer) },
    n_rows{ n_rows },
    n_cols{ n_cols }
    { }

    Surface::Surface(Surface const& other)
    :
    buffer{ std::make_unique<Colour[]>(other.n_rows * other.n_cols) },
    n_rows{ other.n_rows },
    n_cols{ other.n_cols }
    {
        std::ranges::copy(other, this->begin());
    }

    Surface::Surface(SurfaceView other)
    :
    buffer{ std::make_unique<Colour[]>(other.size()) },
    n_rows{ other.get_height() },
    n_cols{ other.get_width() }
    {
        std::ranges::copy(other, this->begin());
    }

    std::size_t Surface::get_width()  const noexcept
    {
        return n_cols;
    }

    std::size_t Surface::get_height() const noexcept
    {
        return n_rows;
    }
}