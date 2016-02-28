#pragma once

#include "vector_traits.hpp"
#include "xmvector.hpp"
// For XMVectorAddInt etc...
#include <DirectXMathExtend.h>

namespace DirectX
{
	namespace hlsl
	{
		namespace vector_math
		{
			template <typename _TScalar, size_t _Size>
			struct add;
			//{ static_assert(false, "This vector math is not implemented yet"); };
			template <typename _TScalar, size_t _Size>
			struct subtract;
			//{ static_assert(false, "This vector math is not implemented yet"); };
			template <typename _TScalar, size_t _Size>
			struct multiply;
			//{ static_assert(false, "This vector math is not implemented yet"); };
			template <typename _TScalar, size_t _Size>
			struct divide;
			//{ static_assert(false, "This vector math is not implemented yet"); };
			template <typename _TScalar, size_t _Size>
			struct madd;
			//{ static_assert(false, "This vector math is not implemented yet"); };


			template <size_t _Size>
			struct add<float, _Size>
			{
				static inline XMVECTOR XM_CALLCONV invoke(FXMVECTOR lhs, FXMVECTOR rhs)
				{ return XMVectorAdd(lhs, rhs); }
				inline auto XM_CALLCONV operator()(FXMVECTOR v0, FXMVECTOR v1) { return invoke(v0, v1); };
			};

			template <size_t _Size>
			struct add<uint, _Size>
			{
				static inline XMVECTOR XM_CALLCONV invoke(FXMVECTOR lhs, FXMVECTOR rhs)
				{
					return XMVectorAddInt(lhs, rhs);
				}
				inline auto XM_CALLCONV operator()(FXMVECTOR v0, FXMVECTOR v1) { return invoke(v0, v1); };
			};

			template <size_t _Size>
			struct subtract<float, _Size>
			{
				static inline XMVECTOR XM_CALLCONV invoke(FXMVECTOR lhs, FXMVECTOR rhs)
				{
					return XMVectorSubtract(lhs, rhs);
				}
				inline auto XM_CALLCONV operator()(FXMVECTOR v0, FXMVECTOR v1) { return invoke(v0, v1); };
			};

			template <size_t _Size>
			struct subtract<uint, _Size>
			{
				static inline XMVECTOR XM_CALLCONV invoke(FXMVECTOR lhs, FXMVECTOR rhs)
				{
					return XMVectorSubtractInt(lhs, rhs);
				}
				inline auto XM_CALLCONV operator()(FXMVECTOR v0, FXMVECTOR v1) { return invoke(v0, v1); };
			};

			template <size_t _Size>
			struct multiply<float, _Size>
			{
				static inline XMVECTOR XM_CALLCONV invoke(FXMVECTOR lhs, FXMVECTOR rhs)
				{
					return XMVectorMultiply(lhs, rhs);
				}
				inline auto XM_CALLCONV operator()(FXMVECTOR v0, FXMVECTOR v1) { return invoke(v0, v1); };

			};

			template <size_t _Size>
			struct multiply<uint, _Size>
			{
				static inline XMVECTOR XM_CALLCONV invoke(FXMVECTOR lhs, FXMVECTOR rhs)
				{
					return XMVectorMultiplyInt(lhs, rhs);
				}
				inline auto XM_CALLCONV operator()(FXMVECTOR v0, FXMVECTOR v1) { return invoke(v0, v1); };
			};

			template <size_t _Size>
			struct divide<float,_Size>
			{
				static inline XMVECTOR XM_CALLCONV invoke(FXMVECTOR lhs, FXMVECTOR rhs)
				{
					return XMVectorDivide(lhs, rhs);
				}
				inline auto XM_CALLCONV operator()(FXMVECTOR v0, FXMVECTOR v1) { return invoke(v0, v1); };
			};

			template <size_t _Size>
			struct madd<float, _Size>
			{
				static inline XMVECTOR XM_CALLCONV invoke(FXMVECTOR v1, FXMVECTOR v2, FXMVECTOR v3)
				{
					return _DXMEXT XMVectorMultiplyAdd(v1,v2,v3);
				}
				inline auto XM_CALLCONV operator()(FXMVECTOR v1, FXMVECTOR v2, FXMVECTOR v3) { return invoke(v1, v2, v3); };
			};
		}

		template <size_t _Size>
		inline xmvector<float, _Size> XM_CALLCONV operator-(const xmvector<float, _Size> xmv)
		{
			xmvector<float, _Size> ret;
			ret.v = XMVectorNegate(xmv.v);
			return ret;
		}

		template <typename lhs_t, typename rhs_t>
		struct float_vector_binary_operator_type : public std::enable_if<
			binary_operator_traits<lhs_t, rhs_t>::overload &&
			std::is_same < float, typename binary_operator_traits<lhs_t, rhs_t>::scalar_type>::value,
			typename binary_operator_traits<lhs_t, rhs_t>::return_type >
		{
		};

		template <typename lhs_t, typename rhs_t>
		struct enable_if_binary_operator_type : public std::enable_if<
			binary_operator_traits<lhs_t, rhs_t>::overload , typename binary_operator_traits<lhs_t, rhs_t>::return_type >
		{};

		template <typename lhs_t, typename rhs_t>
		using enable_if_binary_operator_t = typename enable_if_binary_operator_type<lhs_t, rhs_t>::type;

		template <typename lhs_t, typename rhs_t>
		using float_vector_binary_operator_t = typename float_vector_binary_operator_type<lhs_t, rhs_t>::type;

		template <typename _Ty, size_t _Size>
		inline XMVECTOR XM_CALLCONV xmfoward(const xmvector<_Ty, _Size>& xmv)
		{ return xmv.v; };
		template <typename _Ty>
		inline XMVECTOR XM_CALLCONV xmfoward(const xmscalar<_Ty>& xms)
		{ return xms.v; }
		template <typename _Ty>
		inline std::enable_if_t<is_mermery_type<_Ty>::value,XMVECTOR> XM_CALLCONV xmfoward(const _Ty& mvector)
		{ return load(mvector).v; }
		template <typename _Ty>
		inline std::enable_if_t<is_expression<_Ty>::value, XMVECTOR> XM_CALLCONV xmfoward(const _Ty& mvector)
		{ return mvector.eval().v; }


		template <typename lhs_t, typename rhs_t>
		inline float_vector_binary_operator_t<lhs_t, rhs_t> XM_CALLCONV operator<<(const lhs_t& lhs, const rhs_t& rhs)
		{
			using ret_type = float_vector_binary_operator_t<lhs_t, rhs_t>;
			ret_type ret;
			ret.v = XMVectorAdd(xmfoward(lhs), xmfoward(rhs));
			return ret;
		}

		template <typename lhs_t, typename rhs_t>
		inline enable_if_binary_operator_t<lhs_t,rhs_t> XM_CALLCONV operator+(const lhs_t& lhs, const rhs_t& rhs)
		{
			using traits = binary_operator_traits<lhs_t, rhs_t>;
			typename traits::return_type ret;
			ret.v = vector_math::add<typename traits::scalar_type, traits::size>::invoke(xmfoward(lhs), xmfoward(rhs));
			return ret;
		}

		template <typename lhs_t, typename rhs_t>
		inline enable_if_binary_operator_t<lhs_t, rhs_t> XM_CALLCONV operator-(const lhs_t& lhs, const rhs_t& rhs)
		{
			using traits = binary_operator_traits<lhs_t, rhs_t>;
			typename traits::return_type ret;
			ret.v = vector_math::subtract<typename traits::scalar_type, traits::size>::invoke(xmfoward(lhs), xmfoward(rhs));
			return ret;
		}

		template <typename lhs_t, typename rhs_t>
		inline enable_if_binary_operator_t<lhs_t, rhs_t> XM_CALLCONV operator*(const lhs_t& lhs, const rhs_t& rhs)
		{
			using traits = binary_operator_traits<lhs_t, rhs_t>;
			typename traits::return_type ret;
			ret.v = vector_math::multiply<typename traits::scalar_type, traits::size>::invoke(xmfoward(lhs), xmfoward(rhs));
			return ret;
		}

		template <typename lhs_t, typename rhs_t>
		inline enable_if_binary_operator_t<lhs_t, rhs_t> XM_CALLCONV operator/(const lhs_t& lhs, const rhs_t& rhs)
		{
			using traits = binary_operator_traits<lhs_t, rhs_t>;
			typename traits::return_type ret;
			ret.v = vector_math::divide<typename traits::scalar_type, traits::size>::invoke(xmfoward(lhs), xmfoward(rhs));
			return ret;
		}

		//template <typename lhs_t, typename rhs_t>
		//std::enable_if_t<!is_mermery_type<lhs_t>::value && binary_operator_traits<lhs_t,rhs_t>::overload_assign>
		//	operator += (lhs_t& lhs, const rhs_t& rhs)
		//{
		//	lhs = lhs + rhs;
		//}


		//template <typename lhs_t, typename rhs_t>
		//inline float_vector_binary_operator_t<lhs_t, rhs_t> XM_CALLCONV operator-(const lhs_t lhs, const rhs_t rhs)
		//{
		//	using ret_type = float_vector_binary_operator_t<lhs_t, rhs_t>;
		//	ret_type ret;
		//	ret.v = XMVectorSubtract(lhs.v, rhs.v);
		//	return ret;
		//}

		//template <typename lhs_t, typename rhs_t>
		//inline float_vector_binary_operator_t<lhs_t, rhs_t> XM_CALLCONV operator*(const lhs_t lhs, const rhs_t rhs)
		//{
		//	using ret_type = float_vector_binary_operator_t<lhs_t, rhs_t>;
		//	ret_type ret;
		//	ret.v = XMVectorMultiply(lhs.v, rhs.v);
		//	return ret;
		//}

		//template <typename lhs_t, typename rhs_t>
		//inline float_vector_binary_operator_t<lhs_t, rhs_t> XM_CALLCONV operator/(const lhs_t lhs, const rhs_t rhs)
		//{
		//	using ret_type = float_vector_binary_operator_t<lhs_t, rhs_t>;
		//	ret_type ret;
		//	ret.v = XMVectorDivide(lhs.v, rhs.v);
		//	return ret;
		//}

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