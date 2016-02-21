#pragma once

#include "xmmatrix.hpp"

namespace DirectX
{
    namespace hlsl
    {
        template <typename T>
        struct vector_traits
        {
            static_assert(false,"type not support, please add cutomized vector_traits specialization.");

            static constexpr int Size = 0;
            static constexpr int Rows = 0;
            static constexpr int Cols = 0;
            static constexpr bool Aligned = alignof(T) == alignof(XMVECTOR);
            using Scalar = void;
        };

        template <>
        struct vector_traits<XMFLOAT2>
        {
            static_assert(false,"type not support, please add cutomized vector_traits specialization.");

            static constexpr int Size = 2;
            static constexpr int Rows = 1;
            static constexpr int Cols = 2;
            static constexpr bool Aligned = false;
            using Scalar = void;
        };
    }
}