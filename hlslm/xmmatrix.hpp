#pragma once
#include "xmvector.hpp"

namespace DirectX
{
    namespace hlsl
    {
        template <typename _T, size_t _Rows, size_t _Cols>
        struct xmmatrix
        {
            static constexpr size_t Size = _Rows * _Cols;
            static constexpr size_t Rows = _Rows;
            static constexpr size_t Cols = _Cols;
            using Scalar = _T;
            using RowVectorType = xmvector<Scalar,Cols>;

            RowVectorType r[Rows];  
        };
        
        using xmmatrix4f   = xmmatrix<float,4,4>;
        using xmmatrix3f   = xmmatrix<float,3,3>;
        using xmmatrix4x4f = xmmatrix<float,4,4>;
        using xmmatrix3x3f = xmmatrix<float,3,3>;
        using xmmatrix2x4f = xmmatrix<float,2,4>;
        using xmmatrix3x4f = xmmatrix<float,2,4>;
    }
}