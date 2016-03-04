#pragma once

#include "vector_traits.hpp"
#include "xmvector.hpp"
// For XMVectorAddInt etc...
#include <DirectXMathExtend.h>

namespace DirectX
{
	namespace hlsl
	{
		//template <size_t _Size>
		//inline xmvector<float, _Size> XM_CALLCONV operator-(const xmvector<float, _Size> xmv)
		//{
		//	xmvector<float, _Size> ret;
		//	ret.v = XMVectorNegate(xmv.v);
		//	return ret;
		//}

		namespace traits
		{
			template <typename lhs_t, typename rhs_t>
			struct enable_if_binary_operator_type : public std::enable_if<
				binary_operator_traits<lhs_t, rhs_t>::overload, typename binary_operator_traits<lhs_t, rhs_t>::return_type >
			{};

			template <typename lhs_t, typename rhs_t>
			using enable_if_binary_operator_t = typename enable_if_binary_operator_type<lhs_t, rhs_t>::type;
		}

		namespace detail
		{
			template <typename _Ty, size_t _Size>
			inline XMVECTOR XM_CALLCONV xmfoward(const xmvector<_Ty, _Size>& xmv)
			{
				return xmv.v;
			};
			template <typename _Ty>
			inline XMVECTOR XM_CALLCONV xmfoward(const xmscalar<_Ty>& xms)
			{
				return xms.v;
			}
			template <typename _Ty>
			inline std::enable_if_t<traits::is_mermery_type<_Ty>::value, XMVECTOR> XM_CALLCONV xmfoward(const _Ty& mvector)
			{
				return load(mvector).v;
			}
			template <typename _Ty>
			inline std::enable_if_t<traits::is_expression<_Ty>::value, XMVECTOR> XM_CALLCONV xmfoward(const _Ty& mvector)
			{
				return mvector.eval().v;
			}
		}

		// Comparison functions
		template <size_t _Size>
		inline xmvector<uint, _Size> XM_CALLCONV equal(const xmvector<float, _Size> lhs, const xmvector<float, _Size> rhs)
		{
			xmvector<uint, _Size> ret;
			ret.v = XMVectorEqual(lhs.v, rhs.v);
			return ret;
		}

		template <size_t _Size>
		inline xmvector<uint, _Size> XM_CALLCONV greater_equal(const xmvector<float, _Size> lhs, const xmvector<float, _Size> rhs)
		{
			xmvector<uint, _Size> ret;
			ret.v = XMVectorGreaterOrEqual(lhs.v, rhs.v);
			return ret;
		}

		template <size_t _Size>
		inline xmvector<uint, _Size> XM_CALLCONV greater(const xmvector<float, _Size> lhs, const xmvector<float, _Size> rhs)
		{
			xmvector<uint, _Size> ret;
			ret.v = XMVectorGreater(lhs.v, rhs.v);
			return ret;
		}

		template <size_t _Size>
		inline xmvector<uint, _Size> XM_CALLCONV less_equal(const xmvector<float, _Size> lhs, const xmvector<float, _Size> rhs)
		{
			xmvector<uint, _Size> ret;
			ret.v = XMVectorLessOrEqual(lhs.v, rhs.v);
			return ret;
		}

		template <size_t _Size>
		inline xmvector<uint, _Size> XM_CALLCONV less(const xmvector<float, _Size> lhs, const xmvector<float, _Size> rhs)
		{
			xmvector<uint, _Size> ret;
			ret.v = XMVectorLess(lhs.v, rhs.v);
			return ret;
		}

		template <size_t _Size>
		inline xmvector<uint, _Size> XM_CALLCONV near_equal(const xmvector<float, _Size> lhs, const xmvector<float, _Size> rhs, const xmscalar<float> epsilon)
		{
			xmvector<uint, _Size> ret;
			ret.v = XMVectorNearEqual(lhs.v, rhs.v, epsilon.v);
			return ret;
		}

		template <size_t _Size>
		inline xmvector<uint, _Size> XM_CALLCONV near_equal(const xmvector<float, _Size> lhs, const xmvector<float, _Size> rhs, const xmvector<float, _Size> epsilon)
		{
			xmvector<uint, _Size> ret;
			ret.v = XMVectorNearEqual(lhs.v, rhs.v, epsilon.v);
			return ret;
		}

		// Bitwise operators for uint vectors
		// lhs & ~rhs
		template <size_t _Size>
		inline xmvector<uint, _Size> XM_CALLCONV andnot(const xmvector<uint, _Size> lhs, const xmvector<uint, _Size> rhs)
		{
			xmvector<uint, _Size> ret;
			ret.v = XMVectorAndCInt(lhs.v, rhs.v);
			return ret;
		}

		template <size_t _Size>
		inline xmvector<uint, _Size> XM_CALLCONV nor(const xmvector<uint, _Size> lhs, const xmvector<uint, _Size> rhs)
		{
			xmvector<uint, _Size> ret;
			ret.v = XMVectorNorInt(lhs.v, rhs.v);
			return ret;
		}

		template <size_t _Size>
		inline xmvector<uint, _Size> XM_CALLCONV equal(const xmvector<uint, _Size> lhs, const xmvector<uint, _Size> rhs)
		{
			xmvector<uint, _Size> ret;
			ret.v = XMVectorEqualInt(lhs.v, rhs.v);
			return ret;
		}

		template <size_t _Size>
		inline xmvector<uint, _Size> XM_CALLCONV not_equal(const xmvector<uint, _Size> lhs, const xmvector<uint, _Size> rhs)
		{
			xmvector<uint, _Size> ret;
			ret.v = XMVectorNotEqualInt(lhs.v, rhs.v);
			return ret;
		}

		template <size_t _Size>
		inline xmvector<uint, _Size> XM_CALLCONV and(const xmvector<uint, _Size> lhs, const xmvector<uint, _Size> rhs)
		{
			xmvector<uint, _Size> ret;
			ret.v = XMVectorAndInt(lhs.v, rhs.v);
			return ret;
		}

		template <size_t _Size>
		inline xmvector<uint, _Size> XM_CALLCONV or(const xmvector<uint, _Size> lhs, const xmvector<uint, _Size> rhs)
		{
			xmvector<uint, _Size> ret;
			ret.v = XMVectorOrInt(lhs.v, rhs.v);
			return ret;
		}

		template <size_t _Size>
		inline xmvector<uint, _Size> XM_CALLCONV xor(const xmvector<uint, _Size> lhs, const xmvector<uint, _Size> rhs)
		{
			xmvector<uint, _Size> ret;
			ret.v = XMVectorXorInt(lhs.v, rhs.v);
			return ret;
		}
	}
}