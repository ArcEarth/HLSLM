#pragma once

#include "vector_traits.hpp"
#include "xmvector.hpp"

namespace DirectX
{
	namespace hlsl
	{
		namespace vector_math
		{
			template <typename _TScalar, typename _TVector, size_t _Size>
			struct add;

			template <size_t _Size>
			struct add<float, XMVECTOR, _Size>
			{
				static inline XMVECTOR XM_CALLCONV invoke(FXMVECTOR lhs, FXMVECTOR rhs)
				{ return XMVectorAdd(lhs, rhs); }
				inline auto XM_CALLCONV operator()(FXMVECTOR v0, FXMVECTOR v1) { return invoke(v0, v1); };
			};

			template <size_t _Size>
			struct add<uint, XMVECTOR, _Size>
			{
				static inline XMVECTOR XM_CALLCONV invoke(FXMVECTOR lhs, FXMVECTOR rhs)
				{
					return XMVectorAddInt(lhs, rhs);
				}
				inline auto XM_CALLCONV operator()(FXMVECTOR v0, FXMVECTOR v1) { return invoke(v0, v1); };
			};

			template <typename _TScalar, typename _TVector, size_t _Size>
			struct subtract
			{
				static inline XMVECTOR XM_CALLCONV invoke(FXMVECTOR lhs, FXMVECTOR rhs)
				{
					return XMVectorSubtract(lhs, rhs);
				}
				inline auto XM_CALLCONV operator()(FXMVECTOR v0, FXMVECTOR v1) { return invoke(v0, v1); };
			};

			template <size_t _Size>
			struct add<uint, XMVECTOR, _Size>
			{
				static inline XMVECTOR XM_CALLCONV invoke(FXMVECTOR lhs, FXMVECTOR rhs)
				{
					return XMVectorSubtractInt(lhs, rhs);
				}
				inline auto XM_CALLCONV operator()(FXMVECTOR v0, FXMVECTOR v1) { return invoke(v0, v1); };
			};

			template <typename _TScalar, typename _TVector, size_t _Size>
			struct multiply
			{
				static inline XMVECTOR XM_CALLCONV invoke(FXMVECTOR lhs, FXMVECTOR rhs)
				{
					return XMVectorMultiply(lhs, rhs);
				}
				inline auto XM_CALLCONV operator()(FXMVECTOR v0, FXMVECTOR v1) { return invoke(v0, v1); };

			};

			template <size_t _Size>
			struct add<uint, XMVECTOR, _Size>
			{
				static inline XMVECTOR XM_CALLCONV invoke(FXMVECTOR lhs, FXMVECTOR rhs)
				{
					return XMVectorMultiplyInt(lhs, rhs);
				}
				inline auto XM_CALLCONV operator()(FXMVECTOR v0, FXMVECTOR v1) { return invoke(v0, v1); };
			};

			template <typename _TScalar, typename _TVector, size_t _Size>
			struct divide
			{
				static inline XMVECTOR XM_CALLCONV invoke(FXMVECTOR lhs, FXMVECTOR rhs)
				{
					return XMVectorDivide(lhs, rhs);
				}
				inline auto XM_CALLCONV operator()(FXMVECTOR v0, FXMVECTOR v1) { return invoke(v0, v1); };
			};

		}

		namespace detail
		{
			template <typename _TInvoker, typename lhs_t, typename rhs_t>
			struct binary_operator;

			template <typename _TInvoker, typename _Ty, size_t _Size>
			struct binary_operator<_TInvoker,xmvector<_Ty, _Size>, xmvector<_Ty, _Size>>
			{
				static constexpr bool left_v = true;
				static constexpr bool right_v = true;

				using scalar_type = _Ty;
				using return_type = xmvector<_Ty, _Size>;

				using left_type = xmvector<_Ty, _Size>;
				using right_type = xmvector<_Ty, _Size>;

				static inline return_type XM_CALLCONV invoke(const left_type lhs, const right_type rhs)
				{
					return_type ret;
					ret.v = _TInvoker::invoke(lhs.v, rhs.v);
					return ret;
				}
			};

			template <typename _TInvoker, typename _Ty, index_t... _SwzIdx>
			struct binary_operator<_TInvoker,xmvector<_Ty, sizeof...(_SwzIdx)>, xmselector<_Ty, _SwzIdx...>>
			{
				static constexpr size_t _Size = sizeof...(_SwzIdx);
				static constexpr bool left_v = true;
				static constexpr bool right_v = false;
				using scalar_type = _Ty;
				using return_type = xmvector<_Ty, _Size>;
				using left_type = xmvector<_Ty, _Size>;
				using right_type = xmselector<_Ty, _SwzIdx...>;

				static inline return_type XM_CALLCONV invoke(const left_type lhs, const right_type rhs)
				{
					return_type ret;
					ret.v = _TInvoker::invoke(lhs.v, rhs.eval().v);
					return ret;
				}
			};

			template <typename _TInvoker, typename _Ty, index_t... _SwzIdx>
			struct binary_operator<_TInvoker,xmselector<_Ty, _SwzIdx...>, xmvector<_Ty, sizeof...(_SwzIdx)>>
			{
				static constexpr size_t _Size = sizeof...(_SwzIdx);
				static constexpr bool left_v = false;
				static constexpr bool right_v = true;
				using scalar_type = _Ty;
				using return_type = xmvector<_Ty, _Size>;
				using right_type = xmvector<_Ty, _Size>;
				using left_type = xmselector<_Ty, _SwzIdx...>;

				static inline return_type XM_CALLCONV invoke(const left_type lhs, const right_type rhs)
				{
					return_type ret;
					ret.v = _TInvoker::invoke(lhs.eval().v, rhs.v);
					return ret;
				}
			};

			template <typename _TInvoker, typename _Ty, index_t... _SwzIdx, index_t... _RSwzIdx>
			struct binary_operator<_TInvoker,xmselector<_Ty, _SwzIdx...>, xmselector<_Ty, _RSwzIdx...>>
			{
				static_assert(sizeof...(_RSwzIdx) == sizeof...(_SwzIdx), "vector dimension must agree");
				static constexpr bool left_v = false;
				static constexpr bool right_v = false;
				using scalar_type = _Ty;
				using return_type = xmvector<_Ty, sizeof...(_RSwzIdx)>;
				using right_type = xmselector<_Ty, _RSwzIdx...>;
				using left_type = xmselector<_Ty, _SwzIdx...>;

				static inline return_type XM_CALLCONV invoke(const left_type lhs, const right_type rhs)
				{
					return_type ret;
					ret.v = _TInvoker::invoke(lhs.eval().v, rhs.eval().v);
					return ret;
				}

			};

			template <typename _TInvoker, typename _Ty, size_t _Size>
			struct binary_operator<_TInvoker,xmvector<_Ty, _Size>, xmscalar<_Ty>>
			{
				static constexpr bool left_v = true;
				static constexpr bool right_v = true;
				using scalar_type = _Ty;
				using return_type = xmvector<_Ty, _Size>;

				using left_type = xmvector<_Ty, _Size>;
				using right_type = xmscalar<_Ty>;

				static inline return_type XM_CALLCONV invoke(const left_type lhs, const right_type rhs)
				{
					return_type ret;
					ret.v = _TInvoker::invoke(lhs.v, rhs.v);
					return ret;
				}
			};

			template <typename _TInvoker, typename _Ty, size_t _Size>
			struct binary_operator<_TInvoker,xmscalar<_Ty>, xmvector<_Ty, _Size>>
			{
				static constexpr bool left_v = true;
				static constexpr bool right_v = true;

				using scalar_type = _Ty;
				using return_type = xmvector<_Ty, _Size>;
				using left_type = xmscalar<_Ty>;
				using right_type = xmvector<_Ty, _Size>;

				static inline return_type XM_CALLCONV invoke(const left_type lhs, const right_type rhs)
				{
					return_type ret;
					ret.v = _TInvoker::invoke(lhs.v, rhs.v);
					return ret;
				}
			};

			template <typename _TInvoker, typename _Ty>
			struct binary_operator<_TInvoker,xmscalar<_Ty>, xmscalar<_Ty>>
			{
				static constexpr bool left_v = true;
				static constexpr bool right_v = true;
				using scalar_type = _Ty;
				using return_type = xmscalar<_Ty>;
				using left_type = xmscalar<_Ty>;
				using right_type = xmscalar<_Ty>;

				static inline return_type XM_CALLCONV invoke(const left_type lhs, const right_type rhs)
				{
					return_type ret;
					ret.v = _TInvoker::invoke(lhs.v, rhs.v);
					return ret;
				}
			};

			struct getlhs
			{
				static inline XMVECTOR XM_CALLCONV invoke(FXMVECTOR v0, FXMVECTOR v1) { return v0; };
				inline auto XM_CALLCONV operator()(FXMVECTOR v0, FXMVECTOR v1) { return invoke(v0, v1); };
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
		using float_vector_binary_operator_t = typename float_vector_binary_operator_type<lhs_t, rhs_t>::type;

		template <typename _Ty, size_t _Size>
		inline XMVECTOR XM_CALLCONV xmfoward(const xmvector<_Ty, _Size>& xmv)
		{ return xmv.v; };
		template <typename _Ty>
		inline XMVECTOR XM_CALLCONV xmfoward(const xmscalar<_Ty>& xms)
		{ return xms.v; }
		template <typename _Ty>
		inline XMVECTOR XM_CALLCONV xmfoward(const _Ty& mvector)
		{ return load(mvector).v; }


		template <typename lhs_t, typename rhs_t>
		inline float_vector_binary_operator_t<lhs_t, rhs_t> XM_CALLCONV operator<<(const lhs_t& lhs, const rhs_t& rhs)
		{
			using ret_type = float_vector_binary_operator_t<lhs_t, rhs_t>;
			ret_type ret;
			ret.v = XMVectorAdd(xmfoward(lhs), xmfoward(rhs));
			return ret;
		}

		template <typename lhs_t, typename rhs_t>
		inline float_vector_binary_operator_t<lhs_t,rhs_t> XM_CALLCONV operator+(const lhs_t& lhs, const rhs_t& rhs)
		{
			using ret_type = float_vector_binary_operator_t<lhs_t, rhs_t>;
			ret_type ret;
			ret.v = XMVectorAdd(xmfoward(lhs), xmfoward(rhs));
			return ret;
		}
		template <typename lhs_t, typename rhs_t>
		inline float_vector_binary_operator_t<lhs_t, rhs_t> XM_CALLCONV operator-(const lhs_t& lhs, const rhs_t& rhs)
		{
			using ret_type = float_vector_binary_operator_t<lhs_t, rhs_t>;
			ret_type ret;
			ret.v = XMVectorSubtract(xmfoward(lhs), xmfoward(rhs));
			return ret;
		}
		template <typename lhs_t, typename rhs_t>
		inline float_vector_binary_operator_t<lhs_t, rhs_t> XM_CALLCONV operator*(const lhs_t& lhs, const rhs_t& rhs)
		{
			using ret_type = float_vector_binary_operator_t<lhs_t, rhs_t>;
			ret_type ret;
			ret.v = XMVectorMultiply(xmfoward(lhs), xmfoward(rhs));
			return ret;
		}

		template <typename lhs_t, typename rhs_t>
		inline float_vector_binary_operator_t<lhs_t, rhs_t> XM_CALLCONV operator/(const lhs_t& lhs, const rhs_t& rhs)
		{
			using ret_type = float_vector_binary_operator_t<lhs_t, rhs_t>;
			ret_type ret;
			ret.v = XMVectorDivide(xmfoward(lhs), xmfoward(rhs));
			return ret;
		}

		template <typename lhs_t, typename rhs_t>
		std::enable_if_t<!is_mermery_type<lhs_t>::value && binary_operator_traits<lhs_t,rhs_t>::overload_assign>
			operator += (lhs_t& lhs, const rhs_t& rhs)
		{
			lhs = lhs + rhs;
		}


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