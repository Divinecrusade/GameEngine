#pragma once

namespace GameEngine::Auxiliary::PointerFunctions
{
    template<class T>
    inline T* plus_forward(T* p, ptrdiff_t diff) noexcept
    {
        return p + diff;
    }

    template<class T>
    inline T* minus_forward(T* p, ptrdiff_t diff) noexcept
    {
        return p - diff;
    }

    template<class T>
    inline T* plus_backward(T* p, ptrdiff_t diff) noexcept
    {
        return minus_forward(p, diff);
    }

    template<class T>
    inline T* minus_backward(T* p, ptrdiff_t diff) noexcept
    {
        return plus_forward(p, diff);
    }
}