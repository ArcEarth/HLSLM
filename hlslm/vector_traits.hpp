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
            static constexpr size_t rows = 0;
            static constexpr int cols = 0;
            using scalar = void;
        };
        
        template <typename _Ty>
        struct is_scalar_supported : public std::false_type {};

        template <>
        struct is_scalar_supported<uint> : public std::true_type {};

        template <>
        struct is_scalar_supported<float> : public std::true_type {};        
        
        template <typename _Ty>
        struct is_aligned
        {
            static constexpr size_t value = alignof(_Ty) == alignof(XMVECTOR);
        };

        template <typename _Ty, size_t sz>
        struct vector_traits<_Ty[sz]>
        {
            static constexpr int rows = 1;
            static constexpr int cols = sz;
            using scalar = _Ty;
        };

        template <typename _Ty, size_t sz_row, size_t sz_col>
        struct vector_traits<_Ty[sz_row][sz_col]>
        {
            static constexpr int rows = sz_row;
            static constexpr int cols = sz_col;
            using scalar = _Ty;
        };

        
        template <>
        struct vector_traits<float>
        {
            static constexpr int rows = 1;
            static constexpr int cols = 1;
            using scalar = float;
        };

        template <>
        struct vector_traits<XMFLOAT2>
        {
            static constexpr int rows = 1;
            static constexpr int cols = 2;
            using scalar = float;
        };

        template <>
        struct vector_traits<XMFLOAT3>
        {
            static constexpr int rows = 1;
            static constexpr int cols = 3;
            using scalar = float;
        };

        template <>
        struct vector_traits<XMFLOAT4>
        {
            static constexpr int rows = 1;
            static constexpr int cols = 3;
            using scalar = float;
        };

        template <>
        struct vector_traits<XMFLOAT2A>
        {
            static constexpr int rows = 1;
            static constexpr int cols = 2;
            using scalar = float;
        };

        template <>
        struct vector_traits<XMFLOAT3A>
        {
            static constexpr int rows = 1;
            static constexpr int cols = 3;
            using scalar = float;
        };

        template <>
        struct vector_traits<XMFLOAT4A>
        {
            static constexpr int rows = 1;
            static constexpr int cols = 3;
            using scalar = float;
        };
    }
}