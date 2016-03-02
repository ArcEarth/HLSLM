#pragma once
#ifndef _HLSL_XM_VECTOR_H
#define _HLSL_XM_VECTOR_H

#define __AVX2__ 1
#define __AVX__  1
#define __SSE4__ 1
#define __SSE3__ 1

#include <DirectXMath.h>
#define _NO_SIMPLE_VECTORS
#include <DirectXMathExtend.h>
#include <DirectXMathIntrinsics.h>
#ifndef _DXMEXT
#define _DXMEXT
#endif

#define _XM_VECTOR_USE_LOAD_STORE_HELPER_
#include "detail/mpl.hpp"

#include "vector_traits.hpp"
#include "detail/vector_math.h"

#if defined(_M_IX86) && defined(_DEBUG)
#define XM_VECTOR_LOAD_CTOR explicit
#else
#define XM_VECTOR_LOAD_CTOR
#endif

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
		struct xmswizzler;


		// definition of special swizzle operators
		// like v2.xy, v2.yz, v2.xxyy etc...
		// swizzle_operator_base
		namespace detail
		{
			template <typename _TDerived, index_t _Size>
			struct swizzle_operator_base
			{
			};
#ifdef _SWIZZLE_DECL_
#pragma push_macro(_MAKE_SWS_CONST_)
#pragma push_macro(_SWIZZLE_DECL_)
#undef _SWIZZLE_DECL_
#undef _CONST_SWIZZLE_DECL_
#define _SHOULD_UNDEFINE_MAKE_SWS_SOMETHING_STRANGE_
#endif
#define _SWIZZLE_DECL_(name, ...) \
			inline auto& XM_CALLCONV name () { return static_cast< _TDerived * >( this )->swizzle<__VA_ARGS__ >(); } \
			inline const auto& XM_CALLCONV name() const { return static_cast< const _TDerived * >( this )->swizzle<__VA_ARGS__ >(); }
#define _CONST_SWIZZLE_DECL_(name, ...) \
			inline const auto& XM_CALLCONV name() const { return static_cast< const _TDerived * >( this )->swizzle<__VA_ARGS__ >(); }

			template <typename _TDerived>
			struct swizzle_operator_base<_TDerived, 2>
			{
				#include "detail/swizzles_def_2.h"
			};

			template <typename _TDerived>
			struct swizzle_operator_base<_TDerived, 3>
			{
				#include "detail/swizzles_def_3.h"
			};

			template <typename _TDerived>
			struct swizzle_operator_base<_TDerived, 4>
			{
				#include "detail/swizzles_def_4.h"
			};

#if defined(_SHOULD_UNDEFINE_MAKE_SWS_SOMETHING_STRANGE_)
#pragma pop_macro(_SWIZZLE_DECL_)
#pragma pop_macro(_CONST_SWIZZLE_DECL_)
#else
#undef _CONST_SWIZZLE_DECL_
#undef _SWIZZLE_DECL_
#endif
		}

		namespace detail
		{
			template <typename _TScalar, index_t _Size>
			struct logical_bitwise_operator_base
			{
			};
		};

		// bunch of helpers
		namespace detail
		{
			template <typename _Ty>
			inline const _Ty & as_const(_Ty& ref)
			{
				return const_cast<const _Ty&>(ref);
			}

			template <typename _Ty>
			inline const _Ty && as_const(_Ty&& ref)
			{
				return const_cast<const _Ty&&>(ref);
			}

			template <typename _Ty, bool aligned, size_t cols, size_t rows = 1>
			struct storage_helper;

			inline XMVECTOR XM_CALLCONV replicate_scalar(uint s)
			{
				return XMVectorReplicateInt(s);
			}

			inline XMVECTOR XM_CALLCONV replicate_scalar(float s)
			{
				return XMVectorReplicate(s);
			}

			inline XMVECTOR XM_CALLCONV set_vector(float x, float y = .0f, float z = .0f, float w = .0f)
			{
				return XMVectorSet(x, y, z, w);
			}

			inline XMVECTOR XM_CALLCONV set_vector(uint x, uint y = 0, uint z = 0, uint w = 0)
			{
				return XMVectorSetInt(x, y, z, w);
			}


			enum components_name_enums
			{
				_x = 0,
				_y = 1,
				_z = 2,
				_w = 3,
			};

			template <typename _Ty>
			inline _Ty XM_CALLCONV get(FXMVECTOR xmv, size_t elem_index);
			template <>
			inline float XM_CALLCONV get<float>(FXMVECTOR xmv, size_t elem_index)
			{ return XMVectorGetByIndex(xmv, elem_index); }
			template <>
			inline uint XM_CALLCONV get<uint>(FXMVECTOR xmv, size_t elem_index)
			{ return XMVectorGetIntByIndex(xmv, elem_index); }

			template <typename _Ty>
			inline XMVECTOR XM_CALLCONV set(FXMVECTOR xmv, size_t elem_index, _Ty value);
			template <>
			inline XMVECTOR XM_CALLCONV set<float>(FXMVECTOR xmv, size_t elem_index, float value)
			{ return XMVectorSetByIndex(xmv, value, elem_index); }
			template <>
			inline XMVECTOR XM_CALLCONV set<uint>(FXMVECTOR xmv, size_t elem_index, uint value)
			{ return XMVectorSetIntByIndex(xmv, value, elem_index); }


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
				//static_assert(sizeof...(_SwzArgs) <= 4, "Swizzle element count out of 4");
				//static_assert(conjunction < std::integral_constant<bool, (_SwzArgs < _Size || _SwzArgs == size_t(-1))>...>::value, "Swizzle index out of source vector size");

				static constexpr bool value = (sizeof...(_SwzArgs) <= 4) && (conjunction < std::integral_constant<bool, (_SwzArgs < _Size || _SwzArgs == size_t(-1))>...>::value);
			};

			using std::conditional;
			using std::conditional_t;
			using std::is_same;

			template <typename Scalar, size_t Size>
			struct intrinsic_vector
			{
				using type = void;
			};

			template <typename Scalar, size_t Size>
			struct __mvector
			{
				Scalar m[Size];
			};


#if defined(_XM_SSE_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
			template <>struct intrinsic_vector<float, 1> { using type = __m128; };
			template <>struct intrinsic_vector<float, 2> { using type = __m128; };
			template <>struct intrinsic_vector<float, 3> { using type = __m128; };
			template <>struct intrinsic_vector<float, 4> { using type = __m128; };
			template <>struct intrinsic_vector<float, 5> { using type = __m256; };
			template <>struct intrinsic_vector<float, 6> { using type = __m256; };
			template <>struct intrinsic_vector<float, 7> { using type = __m256; };
			template <>struct intrinsic_vector<float, 8> { using type = __m256; };
			template <>struct intrinsic_vector<double, 1> { using type = __m128d; };
			template <>struct intrinsic_vector<double, 2> { using type = __m128d; };
			template <>struct intrinsic_vector<double, 3> { using type = __m256d; };
			template <>struct intrinsic_vector<double, 4> { using type = __m256d; };
			template <size_t Size >struct intrinsic_vector<int32_t, Size> {
				using scalar = int32_t;
				using type = conditional_t<(Size*sizeof(scalar) <= 16), __m128i, conditional_t<(Size*sizeof(scalar) <= 32), __m256i, void>>;
			};
			template <size_t Size >struct intrinsic_vector<uint32_t, Size> {
				using scalar = uint32_t;
				using type = conditional_t<(Size*sizeof(scalar) <= 16), __m128i, conditional_t<(Size*sizeof(scalar) <= 32), __m256i, void>>;
			};
			template <size_t Size >struct intrinsic_vector<int64_t, Size> {
				using scalar = int64_t;
				using type = conditional_t<(Size*sizeof(scalar) <= 16), __m128i, conditional_t<(Size*sizeof(scalar) <= 32), __m256i, void>>;
			};
			template <size_t Size >struct intrinsic_vector<uint64_t, Size> {
				using scalar = uint64_t;
				using type = conditional_t<(Size*sizeof(scalar) <= 16), __m128i, conditional_t<(Size*sizeof(scalar) <= 32), __m256i, void>>;
			};
			template <size_t Size >struct intrinsic_vector<int16_t, Size> {
				using scalar = int16_t;
				using type = conditional_t<(Size*sizeof(scalar) <= 16), __m128i, conditional_t<(Size*sizeof(scalar) <= 32), __m256i, void>>;
			};
			template <size_t Size >struct intrinsic_vector<uint16_t, Size> {
				using scalar = uint16_t;
				using type = conditional_t<(Size*sizeof(scalar) <= 16), __m128i, conditional_t<(Size*sizeof(scalar) <= 32), __m256i, void>>;
			};
			template <size_t Size >struct intrinsic_vector<int8_t, Size> {
				using scalar = int8_t;
				using type = conditional_t<(Size*sizeof(scalar) <= 16), __m128i, conditional_t<(Size*sizeof(scalar) <= 32), __m256i, void>>;
			};
			template <size_t Size >struct intrinsic_vector<uint8_t, Size> {
				using scalar = uint8_t;
				using type = conditional_t<(Size*sizeof(scalar) <= 16), __m128i, conditional_t<(Size*sizeof(scalar) <= 32), __m256i, void>>;
			};
#elif defined(_XM_ARM_NEON_INTRINSICS_) && !defined(_XM_NO_INTRINSICS_)
			template <>struct intrinsic_vector<float, 1> { using type = float32x2_t; };
			template <>struct intrinsic_vector<float, 2> { using type = float32x2_t; };
			template <>struct intrinsic_vector<float, 3> { using type = float32x4_t; };
			template <>struct intrinsic_vector<float, 4> { using type = float32x4_t; };
			template <>struct intrinsic_vector<float, 5> { using type = float32x4x2_t; };
			template <>struct intrinsic_vector<float, 6> { using type = float32x4x2_t; };
			template <>struct intrinsic_vector<float, 7> { using type = float32x4x2_t; };
			template <>struct intrinsic_vector<float, 8> { using type = float32x4x2_t; };

			template <>struct intrinsic_vector<uint, 1> { using type = uint32x2_t; };
			template <>struct intrinsic_vector<uint, 2> { using type = uint32x2_t; };
			template <>struct intrinsic_vector<uint, 3> { using type = uint32x4_t; };
			template <>struct intrinsic_vector<uint, 4> { using type = uint32x4_t; };
			template <>struct intrinsic_vector<uint, 5> { using type = uint32x4x2_t; };
			template <>struct intrinsic_vector<uint, 6> { using type = uint32x4x2_t; };
			template <>struct intrinsic_vector<uint, 7> { using type = uint32x4x2_t; };
			template <>struct intrinsic_vector<uint, 8> { using type = uint32x4x2_t; };
#else
			template <typename Scalar, size_t Size>
			using intrinsic_vector_t = Scarlar[Size];
#endif

			template <typename Scalar, size_t Size>
			using intrinsic_vector_t = typename intrinsic_vector<Scalar, Size>::type;

			template <typename Scalar, size_t Size>
			using get_intrinsic_vector_t =
				conditional_t<
				is_same<intrinsic_vector_t<Scalar, Size>, void>::value,
				intrinsic_vector_t<Scalar, Size>,
				__mvector<Scalar, Size>
				>;
		}

		template <>
		struct is_mermery_type<DirectX::XMVECTOR> : public std::false_type {};

		template <typename _T, size_t _Size>
		struct XM_ALIGNATTR xmvector : public detail::swizzle_operator_base<xmvector<_T, _Size>,_Size>
		{
			//using components_name_enums = detail::components_name_enums;

			using this_type = xmvector<_T, _Size>;
			static constexpr size_t Size = _Size;
			static constexpr size_t size = Size;
			using Scalar = _T;
			using scalar_type = _T;
			typedef xmvector SelfType;
			using intrinsic_vector = detail::get_intrinsic_vector_t<_T, _Size>;

			static_assert(Size > 0 && sizeof(Scalar) * Size <= 16, "Instantiate xmvector of dimension 0 or greater than 4.");

			XMVECTOR v;

			inline xmvector() = default;

			inline explicit xmvector(CXMVECTOR xmv) { v = xmv; }

			inline explicit xmvector(Scalar s) {
				v = detail::replicate_scalar(s);
			}

			inline xmvector(Scalar _x, Scalar _y, Scalar _z = Scalar(0), Scalar _w = Scalar(0)) {
				v = detail::set_vector(_x, _y, _z, _w);
			}

			this_type& operator= (const this_type& rhs) { this->v = rhs.v; return *this; }

			inline operator XMVECTOR () const
			{
				return v;
			}

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
			inline const xmswizzler<_T, selectors...>& XM_CALLCONV swizzle() const;

			template <index_t... selectors>
			inline xmswizzler<_T, selectors...>& XM_CALLCONV swizzle();

			inline Scalar get(size_t elem_index) const
			{
				assert(elem_index < Size);
				return detail::get<Scalar>(this->v, elem_index);
			}
			inline void set(size_t elem_index, Scalar value)
			{
				assert(elem_index < Size);
				this->v = detail::set<Scalar>(this->v, elem_index, value);
			}
			inline Scalar operator[](size_t elem_index) const
			{ return get(elem_index); }

			// Dynamic swizzler
			inline auto XM_CALLCONV swizzle(uint _x, uint _y = 1, uint _z = 2, uint _w = 3) const
			{ return xmvector<Scalar, 4>(_DXMEXT XMVectorSwizzle(v, _x, _y, _z, _w)); }

			inline this_type& XM_CALLCONV operator += (const this_type rhs)
			{ this->v = vector_math::add<scalar_type, size>::invoke(this->v, rhs.v); return *this; }
			inline this_type& XM_CALLCONV operator -= (const this_type rhs)
			{ this->v = vector_math::subtract<scalar_type, size>::invoke(this->v, rhs.v); return *this; }
			inline this_type& XM_CALLCONV operator *= (const this_type rhs)
			{ this->v = vector_math::multiply<scalar_type, size>::invoke(this->v, rhs.v); return *this; }
			inline this_type& XM_CALLCONV operator /= (const this_type rhs)
			{ this->v = vector_math::divide<scalar_type, size>::invoke(this->v, rhs.v); return *this; }

			inline this_type XM_CALLCONV operator + (const this_type rhs) const
			{ this_type ret; ret.v = vector_math::add<scalar_type, size>::invoke(this->v, rhs.v); return ret; }
			inline this_type XM_CALLCONV operator - (const this_type rhs) const
			{ this_type ret; ret.v = vector_math::subtract<scalar_type, size>::invoke(this->v, rhs.v); return ret; }
			inline this_type XM_CALLCONV operator * (const this_type rhs) const
			{ this_type ret; ret.v = vector_math::multiply<scalar_type, size>::invoke(this->v, rhs.v); return ret; }
			inline this_type XM_CALLCONV operator / (const this_type rhs) const
			{ this_type ret; ret.v = vector_math::divide<scalar_type, size>::invoke(this->v, rhs.v); return ret; }


#if defined(_XM_VECTOR_USE_LOAD_STORE_HELPER_)

			template <typename _Ty, typename _TRet = void>
			using enable_if_loadable = std::enable_if_t<is_mermery_type<_Ty>::value && (memery_vector_traits<_Ty>::cols >= Size) && std::is_same<Scalar,typename memery_vector_traits<_Ty>::scalar>::value, _TRet>;

			// Load from storage types
			template <typename _Ty>
			inline enable_if_loadable<_Ty> operator=(const _Ty& memery_vector)
			{
				using traits = memery_vector_traits<_Ty>;
				using load_imple = detail::storage_helper<typename traits::scalar, is_aligned<_Ty>::value, Size>;
				this->v = load_imple::load(reinterpret_cast<const typename traits::scalar*>(&memery_vector));
			}

			// explicit is an walk around for compiler internal error in x86|Debug mode
			template <typename _Ty>
			inline XM_VECTOR_LOAD_CTOR xmvector(const _Ty& memery_vector, enable_if_loadable<_Ty> *junk = nullptr)
			{ this->operator=<_Ty>(memery_vector); }

			template <typename _Ty>
			inline typename enable_memery_traits_t<_Ty>::void_type XM_CALLCONV store(_Ty& storage) const
			{
				using traits = memery_vector_traits<_Ty>;
				using load_imple = detail::storage_helper<typename traits::scalar, is_aligned<_Ty>::value, traits::cols, traits::rows>;
				load_imple::store(reinterpret_cast<typename traits::scalar*>(&storage), this->v);
			}

			template <typename _Ty>
			inline typename enable_memery_traits_t<_Ty>::void_type XM_CALLCONV store_a(_Ty& storage) const
			{
				using traits = memery_vector_traits<_Ty>;
				using load_imple = detail::storage_helper<typename traits::scalar, true, traits::cols, traits::rows>;
				load_imple::store(reinterpret_cast<typename traits::scalar*>(&storage), this->v);
			}
#endif
		};

		template <typename _T>
		struct XM_ALIGNATTR xmscalar : public xmvector<_T, 1>
		{
			using base_type = xmvector<_T, 1>;
			using this_type = xmscalar<_T>;
			using typename base_type::scalar_type;
			using typename base_type::Scalar;

			static constexpr size_t impl_size = 4;

			inline xmscalar() = default;

			// x86 Debug mode crush by these type of implicit constructor
			inline XM_VECTOR_LOAD_CTOR xmscalar(Scalar s) {
				this->v = detail::replicate_scalar(s);
			}

			inline explicit xmscalar(CXMVECTOR xmv) {
				this->v = xmv;
			}

			inline xmscalar& operator=(const xmscalar& rhs) {
				this->v = rhs.v; return *this;
			}

			inline xmscalar& operator=(Scalar s) {
				this->v = detail::replicate_scalar(s);
				return *this;
			}

			template <size_t _Size>
			inline XM_CALLCONV operator const xmvector<Scalar, _Size>&() const
			{
				return reinterpret_cast<const xmvector<Scalar, _Size>&>(*this);
			}


			inline this_type& XM_CALLCONV operator += (const this_type rhs)
			{
				this->v = vector_math::add<scalar_type, impl_size>::invoke(this->v, rhs.v);
				return *this;
			}

			inline this_type& XM_CALLCONV operator -= (const this_type rhs)
			{
				this->v = vector_math::subtract<scalar_type, impl_size>::invoke(this->v, rhs.v);
				return *this;
			}

			inline this_type& XM_CALLCONV operator *= (const this_type rhs)
			{
				this->v = vector_math::multiply<scalar_type, impl_size>::invoke(this->v, rhs.v);
				return *this;
			}

			inline this_type& XM_CALLCONV operator /= (const this_type rhs)
			{
				this->v = vector_math::divide<scalar_type, impl_size>::invoke(this->v, rhs.v);
				return *this;
			}

			inline this_type XM_CALLCONV operator + (const this_type rhs) const
			{
				this_type ret = *this; ret.operator+=(rhs); return ret;
			}
			inline this_type XM_CALLCONV operator - (const this_type rhs) const
			{
				this_type ret = *this; ret.operator-=(rhs); return ret;
			}
			inline this_type XM_CALLCONV operator * (const this_type rhs) const
			{
				this_type ret = *this; ret.operator*=(rhs); return ret;
			}
			inline this_type XM_CALLCONV operator / (const this_type rhs) const
			{
				this_type ret = *this; ret.operator/=(rhs); return ret;
			}

			// Case of scalar + vector
			template <size_t _RhsSize>
			inline xmvector<Scalar, _RhsSize> XM_CALLCONV operator + (const xmvector<Scalar, _RhsSize> rhs) const
			{ return rhs.operator+(*this); }
			template <size_t _RhsSize>
			inline xmvector<Scalar, _RhsSize> XM_CALLCONV operator - (const xmvector<Scalar, _RhsSize> rhs) const
			{ return rhs.operator-(*this); }
			template <size_t _RhsSize>
			inline xmvector<Scalar, _RhsSize> XM_CALLCONV operator * (const xmvector<Scalar, _RhsSize> rhs) const
			{ return rhs.operator*(*this); }
			template <size_t _RhsSize>
			inline xmvector<Scalar, _RhsSize> XM_CALLCONV operator / (const xmvector<Scalar, _RhsSize> rhs) const
			{ return reinterpret_cast<const xmvector<Scalar, _RhsSize>&>(*this).operator/(rhs); }

			template <index_t... _SrcSwz> 
			inline xmvector<Scalar,sizeof...(_SrcSwz)> XM_CALLCONV operator+(const xmswizzler<Scalar, _SrcSwz...>& rhs) const
			{ return rhs.operator+(reinterpret_cast<const xmswizzler<Scalar, _SrcSwz...>&>(*this)); }
			template <index_t... _SrcSwz> 
			inline xmvector<Scalar, sizeof...(_SrcSwz)> XM_CALLCONV operator-(const xmswizzler<Scalar, _SrcSwz...>& rhs) const
			{ return rhs.operator-(reinterpret_cast<const xmswizzler<Scalar, _SrcSwz...>&>(*this)); }
			template <index_t... _SrcSwz> 
			inline xmvector<Scalar, sizeof...(_SrcSwz)> XM_CALLCONV operator*(const xmswizzler<Scalar, _SrcSwz...>& rhs) const
			{ return rhs.operator*(reinterpret_cast<const xmswizzler<Scalar, _SrcSwz...>&>(*this)); }
			template <index_t... _SrcSwz> 
			inline xmvector<Scalar, sizeof...(_SrcSwz)> XM_CALLCONV operator/(const xmswizzler<Scalar, _SrcSwz...>& rhs) const
			{ return (reinterpret_cast<const xmvector<Scalar, sizeof...(_SrcSwz)>&>(*this)).operator/(rhs.eval()); }

		};

		// Untyped Get Set implementation
		// struct storage_helper definition
		namespace detail
		{
			template <uint32_t Elem>
			// broadcast an element to all dimension, like xyzw -> xxxx
			XMVECTOR splat(FXMVECTOR xmv);

			template <>
			XMVECTOR splat<0>(FXMVECTOR xmv) { return _DXMEXT XMVectorSplatX(xmv); }
			template <>
			XMVECTOR splat<1>(FXMVECTOR xmv) { return _DXMEXT XMVectorSplatY(xmv); }
			template <>
			XMVECTOR splat<2>(FXMVECTOR xmv) { return _DXMEXT XMVectorSplatZ(xmv); }
			template <>
			XMVECTOR splat<3>(FXMVECTOR xmv) { return _DXMEXT XMVectorSplatW(xmv); }


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
				static XMVECTOR XM_CALLCONV load(const float* pSource) { return XMVectorReplicatePtr(pSource); }
				static void XM_CALLCONV store(float* pDst, FXMVECTOR xmv) { XMStoreFloat(pDst, xmv); }
			};

			template <>
			struct storage_helper <float, false, 2> {
				static XMVECTOR XM_CALLCONV load(const float* pSource) { return XMLoadFloat2(reinterpret_cast<const XMFLOAT2*>(pSource)); }
				static void XM_CALLCONV store(float* pDst, FXMVECTOR xmv) { XMStoreFloat2(reinterpret_cast<XMFLOAT2*>(pDst), xmv); }
			};

			template <>
			struct storage_helper <float, false, 3> {
				static XMVECTOR XM_CALLCONV load(const float* pSource) { return XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(pSource)); }
				static void XM_CALLCONV store(float* pDst, FXMVECTOR xmv) { XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(pDst), xmv); }
			};

			template <>
			struct storage_helper <float, false, 4> {
				static XMVECTOR XM_CALLCONV load(const float* pSource) { return XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(pSource)); }
				static void XM_CALLCONV store(float* pDst, FXMVECTOR xmv) { XMStoreFloat4(reinterpret_cast<XMFLOAT4*>(pDst), xmv); }
			};

			template <>
			struct storage_helper <float, true, 2> {
				static XMVECTOR XM_CALLCONV load(const float* pSource) { return XMLoadFloat2A(reinterpret_cast<const XMFLOAT2A*>(pSource)); }
				static void XM_CALLCONV store(float* pDst, FXMVECTOR xmv) { XMStoreFloat2A(reinterpret_cast<XMFLOAT2A*>(pDst), xmv); }
			};

			template <>
			struct storage_helper <float, true, 3> {
				static XMVECTOR XM_CALLCONV load(const float* pSource) { return XMLoadFloat3A(reinterpret_cast<const XMFLOAT3A*>(pSource)); }
				static void XM_CALLCONV store(float* pDst, FXMVECTOR xmv) { XMStoreFloat3A(reinterpret_cast<XMFLOAT3A*>(pDst), xmv); }
			};

			template <>
			struct storage_helper <float, true, 4> {
				static XMVECTOR XM_CALLCONV load(const float* pSource) { return XMLoadFloat4A(reinterpret_cast<const XMFLOAT4A*>(pSource)); }
				static void XM_CALLCONV store(float* pDst, FXMVECTOR xmv) { XMStoreFloat4A(reinterpret_cast<XMFLOAT4A*>(pDst), xmv); }
			};

			template <bool aligned>
			struct storage_helper <uint, aligned, false, 1> {
				static XMVECTOR XM_CALLCONV load(const uint* pSource) { return XMVectorReplicateIntPtr(pSource); }
				static void XM_CALLCONV store(uint* pDst, FXMVECTOR xmv) { XMStoreInt(pDst, xmv); }
			};

			template <>
			struct storage_helper <uint, false, 2> {
				static XMVECTOR XM_CALLCONV load(const uint* pSource) { return XMLoadInt2(pSource); }
				static void XM_CALLCONV store(uint* pDst, FXMVECTOR xmv) { XMStoreInt2(pDst, xmv); }
			};

			template <>
			struct storage_helper <uint, false, 3> {
				static XMVECTOR XM_CALLCONV load(const uint* pSource) { return XMLoadInt3(pSource); }
				static void XM_CALLCONV store(uint* pDst, FXMVECTOR xmv) { XMStoreInt3(pDst, xmv); }
			};

			template <>
			struct storage_helper <uint, false, 4> {
				static XMVECTOR XM_CALLCONV load(const uint* pSource) { XMLoadInt4(pSource); }
				static void XM_CALLCONV store(uint* pDst, FXMVECTOR xmv) { XMStoreInt4(pDst, xmv); }
			};
			template <>
			struct storage_helper <uint, true, 2> {
				static XMVECTOR XM_CALLCONV load(const uint* pSource) {XMLoadInt2A(pSource); }
				static void XM_CALLCONV store(uint* pDst, FXMVECTOR xmv) { XMStoreInt2A(pDst, xmv); }
			};

			template <>
			struct storage_helper <uint, true, 3> {
				static XMVECTOR XM_CALLCONV load(const uint* pSource) { return XMLoadInt3A(pSource); }
				static void XM_CALLCONV store(uint* pDst, FXMVECTOR xmv) { XMStoreInt3A(pDst, xmv); }
			};

			template <>
			struct storage_helper <uint, true, 4> {
				static auto XM_CALLCONV load(const uint* pSource) { return XMLoadInt4A(pSource); }
				static void XM_CALLCONV store(uint* pDst, FXMVECTOR xmv) { XMStoreInt4A(pDst, xmv); }
			};

		}

		using xmfloat	 = xmscalar<float>;
		using xmvector1f = xmvector<float, 1>;
		using xmvector2f = xmvector<float, 2>;
		using xmvector3f = xmvector<float, 3>;
		using xmvector4f = xmvector<float, 4>;

		using xmuint	 = xmscalar<uint>;
		using xmvector1i = xmvector<uint, 1>;
		using xmvector2i = xmvector<uint, 2>;
		using xmvector3i = xmvector<uint, 3>;
		using xmvector4i = xmvector<uint, 4>;
	}
}

#endif