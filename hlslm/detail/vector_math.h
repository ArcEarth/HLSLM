#pragma once
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
				static inline XMVECTOR identity() { return XMVectorZero(); }
			};

			template <size_t _Size>
			struct add<uint, _Size>
			{
				static inline XMVECTOR XM_CALLCONV invoke(FXMVECTOR lhs, FXMVECTOR rhs)
				{ return XMVectorAddInt(lhs, rhs); }
				inline auto XM_CALLCONV operator()(FXMVECTOR v0, FXMVECTOR v1) { return invoke(v0, v1); };
				static inline XMVECTOR identity() { return XMVectorZero(); }
			};

			template <size_t _Size>
			struct subtract<float, _Size>
			{
				static inline XMVECTOR XM_CALLCONV invoke(FXMVECTOR lhs, FXMVECTOR rhs)
				{
					return XMVectorSubtract(lhs, rhs);
				}
				inline auto XM_CALLCONV operator()(FXMVECTOR v0, FXMVECTOR v1) { return invoke(v0, v1); };
				static inline XMVECTOR identity() { return XMVectorZero(); }
			};

			template <size_t _Size>
			struct subtract<uint, _Size>
			{
				static inline XMVECTOR XM_CALLCONV invoke(FXMVECTOR lhs, FXMVECTOR rhs)
				{
					return XMVectorSubtractInt(lhs, rhs);
				}
				inline auto XM_CALLCONV operator()(FXMVECTOR v0, FXMVECTOR v1) { return invoke(v0, v1); };
				static inline XMVECTOR identity() { return XMVectorZero(); }
			};

			template <size_t _Size>
			struct multiply<float, _Size>
			{
				static inline XMVECTOR XM_CALLCONV invoke(FXMVECTOR lhs, FXMVECTOR rhs)
				{
					return XMVectorMultiply(lhs, rhs);
				}
				inline auto XM_CALLCONV operator()(FXMVECTOR v0, FXMVECTOR v1) { return invoke(v0, v1); };
				static inline XMVECTOR identity() { return XMVectorSplatOne(); }
			};

			template <size_t _Size>
			struct multiply<uint, _Size>
			{
				static inline XMVECTOR XM_CALLCONV invoke(FXMVECTOR lhs, FXMVECTOR rhs)
				{
					return XMVectorMultiplyInt(lhs, rhs);
				}
				inline auto XM_CALLCONV operator()(FXMVECTOR v0, FXMVECTOR v1) { return invoke(v0, v1); };
				static inline XMVECTOR identity() { return XMVectorSplatConstantInt(1); }
			};

			template <size_t _Size>
			struct divide<float, _Size>
			{
				static inline XMVECTOR XM_CALLCONV invoke(FXMVECTOR lhs, FXMVECTOR rhs)
				{
					return XMVectorDivide(lhs, rhs);
				}
				inline auto XM_CALLCONV operator()(FXMVECTOR v0, FXMVECTOR v1) { return invoke(v0, v1); };
				static inline XMVECTOR identity() { return XMVectorSplatOne(); }
			};

			template <size_t _Size>
			struct madd<float, _Size>
			{
				static inline XMVECTOR XM_CALLCONV invoke(FXMVECTOR v1, FXMVECTOR v2, FXMVECTOR v3)
				{
					return _DXMEXT XMVectorMultiplyAdd(v1, v2, v3);
				}
				inline auto XM_CALLCONV operator()(FXMVECTOR v1, FXMVECTOR v2, FXMVECTOR v3) { return invoke(v1, v2, v3); };
				static inline XMVECTOR identity() { return g_XMOne.v; }
			};
		}
	}
}