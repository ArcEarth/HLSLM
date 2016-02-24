#pragma once
#include <cstdint>
#include <type_traits>

namespace DirectX
{
	struct XMFLOAT3;
	struct XMFLOAT2;
	struct XMFLOAT4;
	struct XMFLOAT2A;
	struct XMFLOAT3A;
	struct XMFLOAT4A;

	namespace hlsl
    {
		//using std::size_t;
		using index_t = size_t;
		using uint = uint32_t;

		template <typename _T>
		struct xmscalar;

		template <typename _T, size_t _Size>
		struct xmvector;

		template <typename _T, size_t _Rows, size_t _Cols>
		struct xmmatrix;

		template <typename _T, index_t... _SwzArgs>
		struct xmswizzler;

        template <typename T>
        struct vector_traits
        {
            //static_assert(false,"type not support, please add cutomized vector_traits specialization.");
            static constexpr size_t rows = 0;
            static constexpr int cols = 0;
            using scalar = void;
        };

		template <typename _Ty>
		struct is_xmvector : std::false_type {};

		template <typename _Ty>
		struct is_xmvector<xmscalar<_Ty>> : std::true_type {};

		//template <typename _Ty>
		//struct is_xmvector<xmswizzler<_Ty, 0>> : std::true_type {};

		//template <typename _Ty>
		//struct is_xmvector<xmswizzler<_Ty, 0, 1>> : std::true_type {};

		//template <typename _Ty>
		//struct is_xmvector<xmswizzler<_Ty, 0, 1, 2>> : std::true_type {};

		//template <typename _Ty>
		//struct is_xmvector<xmswizzler<_Ty, 0, 1, 2, 3>> : std::true_type {};

		template <typename _Ty, size_t _Size>
		struct is_xmvector<xmvector<_Ty, _Size>> : std::true_type {};

        template <typename _Ty>
        struct scalar_traits : public std::false_type {};

        template <>
        struct scalar_traits<uint> : public std::true_type {
			using type = uint;
		};

        template <>
        struct scalar_traits<float> : public std::true_type {
			using type = float;
		};
        
        template <typename _Ty>
        struct is_aligned
        {
            static constexpr bool value = (alignof(_Ty) >= alignof(XMVECTOR) && (alignof(_Ty) % alignof(XMVECTOR) == 0));
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
		struct binary_operator_traits<xmvector<_Ty, _Size>, xmswizzler<_Ty, _SwzIdx...>>
		{
			static_assert(_Size == sizeof...(_SwzIdx), "vector dimension must agree");
			using scalar_type = _Ty;
			using return_type = xmvector<_Ty, _Size>;
		};

		template <typename _Ty, size_t _Size, index_t... _SwzIdx>
		struct binary_operator_traits<xmswizzler<_Ty, _SwzIdx...>, xmvector<_Ty, _Size>>
		{
			static_assert(_Size == sizeof...(_SwzIdx), "vector dimension must agree");
			using scalar_type = _Ty;
			using return_type = xmvector<_Ty, _Size>;
		};

		template <typename _Ty, index_t... _SwzIdx, index_t... _RSwzIdx>
		struct binary_operator_traits<xmswizzler<_Ty, _SwzIdx...>, xmswizzler<_Ty, _RSwzIdx...>>
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


		template <typename _Ty>
		struct is_mermery_type : public std::true_type {};

		// Floats and int are load into registers
		template <>
		struct is_mermery_type<float> : std::false_type {};

		template <>
		struct is_mermery_type<uint> : std::false_type {};

		template <typename _Ty>
		struct is_mermery_type<xmscalar<_Ty>> : std::false_type {};

		template <typename _Ty, size_t _Size>
		struct is_mermery_type<xmvector<_Ty, _Size>> : std::false_type {};

		template <typename _Ty, index_t... _SwzArgs>
		struct is_mermery_type<xmswizzler<_Ty, _SwzArgs...>> : std::false_type {};

		template <typename _Ty>
		struct memery_vector_traits : public vector_traits<_Ty>
		{
			using void_type = void;
			using memery_type = _Ty;
			using traits = vector_traits<_Ty>;

			using type = std::conditional_t< traits::rows == 1,
				xmvector<typename traits::scalar, traits::cols>,
				xmmatrix<typename traits::scalar, traits::rows, traits::cols>
			>;
		};

		template <typename _Ty>
		struct enable_memery_traits : public std::enable_if<is_mermery_type<_Ty>::value, memery_vector_traits<_Ty>>
		{
		};

		template <typename _Ty>
		using enable_memery_traits_t = typename enable_memery_traits<_Ty>::type;
}
}