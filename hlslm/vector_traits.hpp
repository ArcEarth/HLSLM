#pragma once

#include "xmmatrix.hpp"

namespace DirectX
{
    namespace hlsl
    {
        template <typename T>
        struct vector_traits
        {
            //static_assert(false,"type not support, please add cutomized vector_traits specialization.");
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

		template <typename lhs_t, typename rhs_t>
		struct binary_operator_traits;

		template <typename _Ty, size_t _Size>
		struct binary_operator_traits<xmvector<_Ty,_Size>, xmvector<_Ty, _Size>>
		{
			using scalar_type = _Ty;
			using return_type = xmvector<_Ty, _Size>;
		};

		template <typename _Ty, size_t _Size, index_t... _SwzIdx>
		struct binary_operator_traits<xmvector<_Ty, _Size>, xmvector_swizzler<_Ty, _SwzIdx...>>
		{
			static_assert(_Size == sizeof...(_SwzIdx), "vector dimension must agree");
			using scalar_type = _Ty;
			using return_type = xmvector<_Ty, _Size>;
		};

		template <typename _Ty, size_t _Size, index_t... _SwzIdx>
		struct binary_operator_traits<xmvector_swizzler<_Ty, _SwzIdx...>, xmvector<_Ty, _Size>>
		{
			static_assert(_Size == sizeof...(_SwzIdx), "vector dimension must agree");
			using scalar_type = _Ty;
			using return_type = xmvector<_Ty, _Size>;
		};

		template <typename _Ty, index_t... _SwzIdx, index_t... _RSwzIdx>
		struct binary_operator_traits<xmvector_swizzler<_Ty, _SwzIdx...>, xmvector_swizzler<_Ty, _RSwzIdx...>>
		{
			static_assert(sizeof...(_RSwzIdx) == sizeof...(_SwzIdx), "vector dimension must agree");
			using scalar_type = _Ty;
			using return_type = xmvector<_Ty, sizeof...(_RSwzIdx)>;
		};

		template <typename _Ty, size_t _Size>
		struct binary_operator_traits<xmvector<_Ty, _Size>, xmscalar<_Ty>>
		{
			using scalar_type = _Ty;
			using return_type = xmvector<_Ty, _Size>;
		};

		template <typename _Ty, size_t _Size>
		struct binary_operator_traits<xmscalar<_Ty>, xmvector<_Ty, _Size>>
		{
			using scalar_type = _Ty;
			using return_type = xmvector<_Ty, _Size>;
		};

		template <typename _Ty>
		struct binary_operator_traits<xmscalar<_Ty>, xmscalar<_Ty>>
		{
			using scalar_type = _Ty;
			using return_type = xmscalar<_Ty>;
		};

		template <typename lhs_t, typename rhs_t>
		using binary_operator_return_type = typename binary_operator_traits<lhs_t, rhs_t>::return_type;
}
}