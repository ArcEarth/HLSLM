#pragma once

#include "xmvector.hpp"
#include "vector_traits.hpp"

namespace DirectX
{
	namespace hlsl
	{
		template <size_t _Size>
		inline xmvector<float, _Size> XM_CALLCONV operator-(const xmvector<float, _Size> xmv)
		{
			xmvector<float, _Size> ret;
			ret.v = XMVectorNegate(xmv.v);
			return ret;
		}


		template <typename lhs_t, typename rhs_t>
		struct float_vector_binary_operator_type : public std::enable_if<
			std::is_same < float, typename binary_operator_traits<lhs_t, rhs_t>::scalar_type>::value,
			typename binary_operator_traits<lhs_t, rhs_t>::return_type >
		{
		};

		template <typename lhs_t, typename rhs_t>
		using float_vector_binary_operator_t = typename float_vector_binary_operator_type<lhs_t, rhs_t>::type;

		//template <size_t _Size>
		//inline xmvector<float, _Size> XM_CALLCONV operator+(const xmvector<float, _Size> lhs, const xmvector<float, _Size> rhs)
		//{
		//	xmvector<float, _Size> ret;
		//	ret.v = XMVectorAdd(lhs.v, rhs.v);
		//	return ret;
		//}

		//template <size_t _Size>
		//inline xmvector<float, _Size> XM_CALLCONV operator-(const xmvector<float, _Size> lhs, const xmvector<float, _Size> rhs)
		//{
		//	xmvector<float, _Size> ret;
		//	ret.v = XMVectorSubtract(lhs.v, rhs.v);
		//	return ret;
		//}

		//template <size_t _Size>
		//inline xmvector<float, _Size> XM_CALLCONV operator*(const xmvector<float, _Size> lhs, const xmvector<float, _Size> rhs)
		//{
		//	xmvector<float, _Size> ret;
		//	ret.v = XMVectorMultiply(lhs.v, rhs.v);
		//	return ret;
		//}

		//template <size_t _Size>
		//inline xmvector<float, _Size> XM_CALLCONV operator/(const xmvector<float, _Size> lhs, const xmvector<float, _Size> rhs)
		//{
		//	xmvector<float, _Size> ret;
		//	ret.v = XMVectorDivide(lhs.v, rhs.v);
		//	return ret;
		//}

		template <typename lhs_t, typename rhs_t>
		inline float_vector_binary_operator_t<lhs_t,rhs_t> XM_CALLCONV operator+(const lhs_t lhs, const rhs_t rhs)
		{
			using ret_type = float_vector_binary_operator_t<lhs_t, rhs_t>;
			ret_type ret;
			ret.v = XMVectorAdd(ret_type(lhs).v, ret_type(rhs.v).v);
			return ret;
		}

		template <typename lhs_t, typename rhs_t>
		inline float_vector_binary_operator_t<lhs_t, rhs_t> XM_CALLCONV operator-(const lhs_t lhs, const rhs_t rhs)
		{
			float_vector_binary_operator_t<lhs_t, rhs_t> ret;
			ret.v = XMVectorSubtract(lhs.v, rhs.v);
			return ret;
		}

		template <typename lhs_t, typename rhs_t>
		inline float_vector_binary_operator_t<lhs_t, rhs_t> XM_CALLCONV operator*(const lhs_t lhs, const rhs_t rhs)
		{
			float_vector_binary_operator_t<lhs_t, rhs_t> ret;
			ret.v = XMVectorMultiply(lhs.v, rhs.v);
			return ret;
		}

		template <typename lhs_t, typename rhs_t>
		inline float_vector_binary_operator_t<lhs_t, rhs_t> XM_CALLCONV operator/(const lhs_t lhs, const rhs_t rhs)
		{
			float_vector_binary_operator_t<lhs_t, rhs_t> ret;
			ret.v = XMVectorDivide(lhs.v, rhs.v);
			return ret;
		}

		// Comparision functions
		template <size_t _Size>
		inline xmvector<uint, _Size> XM_CALLCONV operator==(const xmvector<float, _Size> lhs, const xmvector<float, _Size> rhs)
		{
			xmvector<uint, _Size> ret;
			ret.v = XMVectorEqual(lhs.v, rhs.v);
			return ret;
		}

		template <size_t _Size>
		inline xmvector<uint, _Size> XM_CALLCONV operator>=(const xmvector<float, _Size> lhs, const xmvector<float, _Size> rhs)
		{
			xmvector<uint, _Size> ret;
			ret.v = XMVectorGreaterOrEqual(lhs.v, rhs.v);
			return ret;
		}

		template <size_t _Size>
		inline xmvector<uint, _Size> XM_CALLCONV operator>(const xmvector<float, _Size> lhs, const xmvector<float, _Size> rhs)
		{
			xmvector<uint, _Size> ret;
			ret.v = XMVectorGreater(lhs.v, rhs.v);
			return ret;
		}

		template <size_t _Size>
		inline xmvector<uint, _Size> XM_CALLCONV operator<=(const xmvector<float, _Size> lhs, const xmvector<float, _Size> rhs)
		{
			xmvector<uint, _Size> ret;
			ret.v = XMVectorLessOrEqual(lhs.v, rhs.v);
			return ret;
		}

		template <size_t _Size>
		inline xmvector<uint, _Size> XM_CALLCONV operator<(const xmvector<float, _Size> lhs, const xmvector<float, _Size> rhs)
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
		inline xmvector<uint, _Size> XM_CALLCONV operator==(const xmvector<uint, _Size> lhs, const xmvector<uint, _Size> rhs)
		{
			xmvector<uint, _Size> ret;
			ret.v = XMVectorEqualInt(lhs.v, rhs.v);
			return ret;
		}

		template <size_t _Size>
		inline xmvector<uint, _Size> XM_CALLCONV operator!=(const xmvector<uint, _Size> lhs, const xmvector<uint, _Size> rhs)
		{
			xmvector<uint, _Size> ret;
			ret.v = XMVectorNotEqualInt(lhs.v, rhs.v);
			return ret;
		}

		template <size_t _Size>
		inline xmvector<uint, _Size> XM_CALLCONV operator&(const xmvector<uint, _Size> lhs, const xmvector<uint, _Size> rhs)
		{
			xmvector<uint, _Size> ret;
			ret.v = XMVectorAndInt(lhs.v, rhs.v);
			return ret;
		}

		template <size_t _Size>
		inline xmvector<uint, _Size> XM_CALLCONV operator|(const xmvector<uint, _Size> lhs, const xmvector<uint, _Size> rhs)
		{
			xmvector<uint, _Size> ret;
			ret.v = XMVectorOrInt(lhs.v, rhs.v);
			return ret;
		}

		template <size_t _Size>
		inline xmvector<uint, _Size> XM_CALLCONV operator^(const xmvector<uint, _Size> lhs, const xmvector<uint, _Size> rhs)
		{
			xmvector<uint, _Size> ret;
			ret.v = XMVectorXorInt(lhs.v, rhs.v);
			return ret;
		}
	}
}