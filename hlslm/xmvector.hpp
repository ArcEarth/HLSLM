#pragma once
#ifndef _HLSL_XM_VECTOR_H
#define _HLSL_XM_VECTOR_H

#define __AVX2__ 1
#define __AVX__  1
#define __SSE4__ 1
#define __SSE3__ 1

#include <DirectXMath.h>
#include "DirectXMathIntrinsics.h"
#ifndef _DXMEXT
#define _DXMEXT
#endif

#define _XM_VECTOR_USE_LOAD_STORE_HELPER_
#include "detail/mpl.hpp"
#include "detail/swizzle_macro.h"

#include "vector_traits.hpp"

namespace DirectX
{
	namespace hlsl
	{
		using index_t = size_t;
		using uint = uint32_t;
		using std::enable_if_t;

		using namespace mpl;

		template <typename _T>
		struct xmscalar;

		template <typename _T, size_t _Size>
		struct xmvector;

		template <typename _T, index_t... _SwzArgs>
		struct xmselector;

		namespace detail
		{
			template <typename _Ty, bool aligned, size_t cols, size_t rows = 1>
			struct storage_helper;

			inline XMVECTOR replicate_scalar(uint s)
			{
				return XMVectorReplicateInt(s);
			}

			inline XMVECTOR replicate_scalar(float s)
			{
				return XMVectorReplicate(s);
			}

			enum components_name_enums
			{
				_x = 0,
				_y = 1,
				_z = 2,
				_w = 3,
			};

			template <typename _Ty, uint32_t Elem>
			inline _Ty XM_CALLCONV get(FXMVECTOR xmv);

			template <typename _Ty, uint32_t Elem>
			inline void XM_CALLCONV get_ptr(_Ty* ptr, FXMVECTOR xmv);

			template <typename _Ty, uint32_t Elem>
			inline FXMVECTOR XM_CALLCONV set(FXMVECTOR xmv, _Ty val);

			template <typename _Ty, uint32_t Elem>
			inline FXMVECTOR XM_CALLCONV set_ptr(FXMVECTOR xmv, _Ty* val);

			template <size_t _Size, index_t... _SwzArgs>
			struct valiad_swizzle_args
			{
				static_assert(sizeof...(_SwzArgs) <= 4, "Swizzle element count out of 4");
				static_assert(conjunction < std::integral_constant<bool, _SwzArgs < _Size>...>::value, "Swizzle index out of source vector size");

				static constexpr bool value = (sizeof...(_SwzArgs) <= 4) && (conjunction < std::integral_constant<bool, _SwzArgs < _Size>...>::value);
			};

			using std::conditional;
			using std::conditional_t;
			using std::is_same;

			template <typename Scalar, size_t Size>
			using intrinsic_vector_t = 
				conditional_t< sizeof(Scalar)*Size <= 16,
				DirectX::XMVECTOR,
				conditional_t<is_same<float, Scalar>::value,
					__m256,
					conditional_t<is_same<double, Scalar>::value,
						__m256d,
						__m256i>
					>
				>;
		}

		using detail::valiad_swizzle_args;

		template <typename _T, size_t _Size>
		struct xmvector
		{
			//using components_name_enums = detail::components_name_enums;

			using this_type = xmvector<_T, _Size>;
			static constexpr size_t Size = _Size;
			using Scalar = _T;
			typedef xmvector SelfType;
			using intrinsic_vector = detail::intrinsic_vector_t<_T, _Size>;

			static_assert(Size > 0 && sizeof(Scalar) * Size <= 16, "Instantiate xmvector of dimension 0 or greater than 4.");

			XMVECTOR v;

			inline xmvector() = default;

			inline xmvector(FXMVECTOR xmv) {
				v = xmv;
			}

			inline explicit xmvector(Scalar s) {
				v = detail::replicate_scalar(s);
			}

			// vector<scalar, size-1> + scalar
			template <size_t vSize>
			inline explicit xmvector(xmvector<Scalar, vSize> v0, enable_if_t<Size == vSize + 1, Scalar> s) {
				v = detail::set_element<Size - 1>(v0, s);
			}

			inline operator XMVECTOR () const
			{ return v; }

			template <int _NewSize, typename _NewType = Scalar>
			inline xmvector<_NewType, _NewSize> as() const {
				return reinterpret_cast<const xmvector<_NewType, _NewSize>&>(*this);
			}

			template <int _NewSize, typename _NewType = Scalar>
			inline xmvector<_NewType, _NewSize>& as() {
				return reinterpret_cast<xmvector<_NewType, _NewSize>&>(*this);
			}

			template <int _NewSize>
			inline explicit operator xmvector<Scalar, _NewSize>() const {
				return as<Scalar, _NewSize>();
			}

			template <index_t... selectors>
			inline enable_if_t<valiad_swizzle_args<_Size, selectors...>::value, xmselector<_T, selectors...>>&& XM_CALLCONV swizzle();

			template <index_t... selectors>
			inline const enable_if_t<valiad_swizzle_args<_Size, selectors...>::value, xmselector<_T, selectors...>>&& XM_CALLCONV swizzle() const;

			// Dynamic swizzlers
			inline auto XM_CALLCONV swizzle(uint _x) const
			{ return xmscalar<Scalar>(_DXMEXT XMVectorSwizzle(v, _x, _x, _x, _x)); }

			inline auto XM_CALLCONV swizzle(uint _x, uint _y) const
			{ return xmvector<Scalar, 2>( _DXMEXT XMVectorSwizzle(v, _x, _y, 2, 3)); }

			inline auto XM_CALLCONV swizzle(uint _x, uint _y, uint _z) const
			{ return xmvector<Scalar, 3>( _DXMEXT XMVectorSwizzle(v, _x, _y, _z, 3)); }

			inline auto XM_CALLCONV swizzle(uint _x, uint _y, uint _z, uint _w) const
			{ return xmvector<Scalar, 4>( _DXMEXT XMVectorSwizzle(v,_x,_y,_z,_w)); }

#if defined(_XM_VECTOR_USE_LOAD_STORE_HELPER_)

			template <typename _Ty>
			inline xmvector(const _Ty& memery_vector, typename enable_memery_traits_t<_Ty>::void_type *junk = nullptr)
			{
				using traits = memery_vector_traits<_Ty>;
				using load_imple = detail::storage_helper<typename traits::scalar, is_aligned<_Ty>::value, traits::cols, traits::rows>;
				*this = load_imple::load(reinterpret_cast<const typename traits::scalar*>(&memery_vector));
			}


			// Load from storage types
			template <typename _Ty>
			inline typename enable_memery_traits_t<_Ty>::void_type operator=(const _Ty& memery_vector)
			{
				using traits = memery_vector_traits<_Ty>;
				using load_imple = detail::storage_helper<typename traits::scalar, is_aligned<_Ty>::value, traits::cols, traits::rows>;
				*this = load_imple::load(reinterpret_cast<const typename traits::scalar*>(&memery_vector));
			}

			template <typename _Ty>
			inline typename enable_memery_traits_t<_Ty>::void_type XM_CALLCONV store(_Ty& storage) const
			{
				using traits = memery_vector_traits<_Ty>;
				using load_imple = detail::storage_helper<typename traits::scalar, is_aligned<_Ty>::value, traits::cols, traits::rows>;
				load_imple::store(reinterpret_cast<typename traits::scalar*>(&storage), *this);
			}
#endif

			#include "detail/special_swizzle.h"
		};

		//template <typename _T>
		//// Specialization for Sizeless (Unkown) vector
		//struct xmvector<_T, 0>
		//{
		//	static constexpr size_t Size = 0;
		//	using Scalar = _T;
		//	typedef xmvector SelfType;
		//	XMVECTOR v;
		//};

		template <typename _T>
		struct xmscalar : public xmvector<_T, 1>
		{
			typedef xmscalar SelfType;

			inline xmscalar() = default;

			inline xmscalar(Scalar s) {
				v = detail::replicate_scalar(s);
			}

			inline explicit xmscalar(FXMVECTOR xmv) {
				v = xmv;
			}

			inline xmscalar& XM_CALLCONV operator=(xmscalar rhs) {
				v = rhs.v;
			}

			inline xmscalar& operator=(Scalar s) {
				v = detail::rep_scalar(s);
				return *this;
			}

			template <size_t _Size>
			inline XM_CALLCONV operator xmvector<Scalar, _Size>() const
			{
				return reinterpret_cast<const xmvector<Scalar, _Size>&>(*this);
			}
		};

		// Untyped Get Set implementation
		// struct storage_helper defination
		namespace detail
		{
			template <> inline float XM_CALLCONV get<float, _x>(FXMVECTOR xmv) { return _DXMEXT XMVectorGetX(xmv); }
			template <> inline float XM_CALLCONV get<float, _y>(FXMVECTOR xmv) { return _DXMEXT XMVectorGetY(xmv); }
			template <> inline float XM_CALLCONV get<float, _z>(FXMVECTOR xmv) { return _DXMEXT XMVectorGetZ(xmv); }
			template <> inline float XM_CALLCONV get<float, _w>(FXMVECTOR xmv) { return _DXMEXT XMVectorGetW(xmv); }

			template <> inline uint XM_CALLCONV	 get<uint, _x>(FXMVECTOR xmv) { return _DXMEXT XMVectorGetIntX(xmv); }
			template <> inline uint XM_CALLCONV	 get<uint, _y>(FXMVECTOR xmv) { return _DXMEXT XMVectorGetIntY(xmv); }
			template <> inline uint XM_CALLCONV	 get<uint, _z>(FXMVECTOR xmv) { return _DXMEXT XMVectorGetIntZ(xmv); }
			template <> inline uint XM_CALLCONV	 get<uint, _w>(FXMVECTOR xmv) { return _DXMEXT XMVectorGetIntW(xmv); }

			template <> inline void XM_CALLCONV  get_ptr<float, _x>(float* ptr, FXMVECTOR xmv) { return _DXMEXT XMVectorGetXPtr(ptr, xmv); }
			template <> inline void XM_CALLCONV  get_ptr<float, _y>(float* ptr, FXMVECTOR xmv) { return _DXMEXT XMVectorGetYPtr(ptr, xmv); }
			template <> inline void XM_CALLCONV  get_ptr<float, _z>(float* ptr, FXMVECTOR xmv) { return _DXMEXT XMVectorGetZPtr(ptr, xmv); }
			template <> inline void XM_CALLCONV  get_ptr<float, _w>(float* ptr, FXMVECTOR xmv) { return _DXMEXT XMVectorGetWPtr(ptr, xmv); }

			template <> inline void XM_CALLCONV	 get_ptr<uint, _x>(uint* ptr, FXMVECTOR xmv) { return _DXMEXT XMVectorGetIntXPtr(ptr, xmv); }
			template <> inline void XM_CALLCONV	 get_ptr<uint, _y>(uint* ptr, FXMVECTOR xmv) { return _DXMEXT XMVectorGetIntYPtr(ptr, xmv); }
			template <> inline void XM_CALLCONV	 get_ptr<uint, _z>(uint* ptr, FXMVECTOR xmv) { return _DXMEXT XMVectorGetIntZPtr(ptr, xmv); }
			template <> inline void XM_CALLCONV	 get_ptr<uint, _w>(uint* ptr, FXMVECTOR xmv) { return _DXMEXT XMVectorGetIntWPtr(ptr, xmv); }

			template <>	inline FXMVECTOR XM_CALLCONV set<float, _x>(FXMVECTOR xmv, float val) { return _DXMEXT XMVectorSetX(xmv, val); }
			template <>	inline FXMVECTOR XM_CALLCONV set<float, _y>(FXMVECTOR xmv, float val) { return _DXMEXT XMVectorSetY(xmv, val); }
			template <>	inline FXMVECTOR XM_CALLCONV set<float, _z>(FXMVECTOR xmv, float val) { return _DXMEXT XMVectorSetZ(xmv, val); }
			template <>	inline FXMVECTOR XM_CALLCONV set<float, _w>(FXMVECTOR xmv, float val) { return _DXMEXT XMVectorSetW(xmv, val); }

			template <>	inline FXMVECTOR XM_CALLCONV set<uint, _x>(FXMVECTOR xmv, uint val) { return _DXMEXT XMVectorSetIntX(xmv, val); }
			template <>	inline FXMVECTOR XM_CALLCONV set<uint, _y>(FXMVECTOR xmv, uint val) { return _DXMEXT XMVectorSetIntY(xmv, val); }
			template <>	inline FXMVECTOR XM_CALLCONV set<uint, _z>(FXMVECTOR xmv, uint val) { return _DXMEXT XMVectorSetIntZ(xmv, val); }
			template <>	inline FXMVECTOR XM_CALLCONV set<uint, _w>(FXMVECTOR xmv, uint val) { return _DXMEXT XMVectorSetIntW(xmv, val); }

			template <>	inline FXMVECTOR XM_CALLCONV set_ptr<float, _x>(FXMVECTOR xmv, float* ptr) { return _DXMEXT XMVectorSetXPtr(xmv, ptr); }
			template <>	inline FXMVECTOR XM_CALLCONV set_ptr<float, _y>(FXMVECTOR xmv, float* ptr) { return _DXMEXT XMVectorSetYPtr(xmv, ptr); }
			template <>	inline FXMVECTOR XM_CALLCONV set_ptr<float, _z>(FXMVECTOR xmv, float* ptr) { return _DXMEXT XMVectorSetZPtr(xmv, ptr); }
			template <>	inline FXMVECTOR XM_CALLCONV set_ptr<float, _w>(FXMVECTOR xmv, float* ptr) { return _DXMEXT XMVectorSetWPtr(xmv, ptr); }

			template <>	inline FXMVECTOR XM_CALLCONV set_ptr<uint, _x>(FXMVECTOR xmv, uint* ptr) { return _DXMEXT XMVectorSetIntXPtr(xmv, ptr); }
			template <>	inline FXMVECTOR XM_CALLCONV set_ptr<uint, _y>(FXMVECTOR xmv, uint* ptr) { return _DXMEXT XMVectorSetIntYPtr(xmv, ptr); }
			template <>	inline FXMVECTOR XM_CALLCONV set_ptr<uint, _z>(FXMVECTOR xmv, uint* ptr) { return _DXMEXT XMVectorSetIntZPtr(xmv, ptr); }
			template <>	inline FXMVECTOR XM_CALLCONV set_ptr<uint, _w>(FXMVECTOR xmv, uint* ptr) { return _DXMEXT XMVectorSetIntWPtr(xmv, ptr); }

			template <bool aligned>
			struct storage_helper <float, aligned, false, 1> {
				static auto load(const float* pSource) { return xmscalar<float>(XMVectorReplicatePtr(pSource)); }
				static void store(float* pDst, const xmscalar<float> xmv) { XMStoreFloat(pDst, xmv.v); }
			};

			template <>
			struct storage_helper <float, false, 2> {
				static auto load(const float* pSource) { return xmvector<float, 2>(XMLoadFloat2(reinterpret_cast<const XMFLOAT2*>(pSource))); }
				static void store(float* pDst, const xmvector<float, 2> xmv) { XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(pDst), xmv.v); }
			};

			template <>
			struct storage_helper <float, false, 3> {
				static auto load(const float* pSource) { return xmvector<float, 3>(XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(pSource))); }
				static void store(float* pDst, const xmvector<float, 3> xmv) { XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(pDst), xmv.v); }
			};

			template <>
			struct storage_helper <float, false, 4> {
				static auto load(const float* pSource) { return xmvector<float, 4>(XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(pSource))); }
				static void store(float* pDst, const xmvector<float, 4> xmv) { XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(pDst), xmv.v); }
			};

			template <>
			struct storage_helper <float, true, 2> {
				static auto load(const float* pSource) { return xmvector<float, 2>(XMLoadFloat2A(reinterpret_cast<const XMFLOAT2A*>(pSource))); }
				static void store(float* pDst, const xmvector<float, 2> xmv) { XMStoreFloat2A(reinterpret_cast<XMFLOAT2A*>(pDst), xmv.v); }
			};

			template <>
			struct storage_helper <float, true, 3> {
				static auto load(const float* pSource) { return xmvector<float, 3>(XMLoadFloat3A(reinterpret_cast<const XMFLOAT3A*>(pSource))); }
				static void store(float* pDst, const xmvector<float, 3> xmv) { XMStoreFloat3A(reinterpret_cast<XMFLOAT3A*>(pDst), xmv.v); }
			};

			template <>
			struct storage_helper <float, true, 4> {
				static auto load(const float* pSource) { return xmvector<float, 4>(XMLoadFloat4A(reinterpret_cast<const XMFLOAT4A*>(pSource))); }
				static void store(float* pDst, const xmvector<float, 4> xmv) { XMStoreFloat4A(reinterpret_cast<XMFLOAT4A*>(pDst), xmv.v); }
			};

			template <bool aligned>
			struct storage_helper <uint, aligned, false, 1> {
				static auto load(const uint* pSource) { return xmscalar<uint>(XMVectorReplicateIntPtr(pSource)); }
				static void store(uint* pDst, const xmscalar<uint> xmv) { XMStoreInt(pDst, xmv.v); }
			};

			template <>
			struct storage_helper <uint, false, 2> {
				static auto load(const uint* pSource) { return xmvector<uint, 2>(XMLoadInt2(pSource)); }
				static void store(uint* pDst, const xmvector<uint, 2> xmv) { XMStoreInt2(pDst, xmv.v); }
			};

			template <>
			struct storage_helper <uint, false, 3> {
				static auto load(const uint* pSource) { return xmvector<uint, 3>(XMLoadInt3(pSource)); }
				static void store(uint* pDst, const xmvector<uint, 3> xmv) { XMStoreInt3(pDst, xmv.v); }
			};

			template <>
			struct storage_helper <uint, false, 4> {
				static auto load(const uint* pSource) { return xmvector<uint, 4>(XMLoadInt4(pSource)); }
				static void store(uint* pDst, const  xmvector<uint, 4> xmv) { XMStoreInt4(pDst, xmv.v); }
			};
			template <>
			struct storage_helper <uint, true, 2> {
				static auto load(const uint* pSource) { return xmvector<uint, 2>(XMLoadInt2A(pSource)); }
				static void store(uint* pDst, const  xmvector<uint, 2> xmv) { XMStoreInt2A(pDst, xmv.v); }
			};

			template <>
			struct storage_helper <uint, true, 3> {
				static auto load(const uint* pSource) { return xmvector<uint, 3>(XMLoadInt3A(pSource)); }
				static void store(uint* pDst, const xmvector<uint, 3> xmv) { XMStoreInt3A(pDst, xmv.v); }
			};

			template <>
			struct storage_helper <uint, true, 4> {
				static auto load(const uint* pSource) { return xmvector<uint, 4>(XMLoadInt4A(pSource)); }
				static void store(uint* pDst, const  xmvector<uint, 4> xmv) { XMStoreInt4A(pDst, xmv.v); }
			};

		}

		using xmvector1f = xmvector<float, 1>;
		using xmvector2f = xmvector<float, 2>;
		using xmvector3f = xmvector<float, 3>;
		using xmvector4f = xmvector<float, 4>;

		using xmvector1i = xmvector<uint, 1>;
		using xmvector2i = xmvector<uint, 2>;
		using xmvector3i = xmvector<uint, 3>;
		using xmvector4i = xmvector<uint, 4>;
}
}

#endif