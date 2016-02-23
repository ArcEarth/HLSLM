#pragma once

#define __AVX2__ 1
#define __AVX__  1
#define __SSE4__ 1
#define __SSE3__ 1

#include <DirectXMath.h>

//#define _DXMEXT ::DirectX::_DXMEXT 
//#include <DirectXMathIntrinsics.h>

#include "integer_sequence.hpp"
#define _DXMEXT

namespace DirectX
{
    namespace hlsl
    {
		using index_t = size_t;

		using namespace mpl;

        template <typename _T>
        struct xmscalar;

		template <typename _T, index_t _Size, index_t... _SwzArgs>
		struct xmvector_swizzer;
        
        namespace detail
        {
			inline XMVECTOR replicate_scalar(int s)
			{
				return XMVectorReplicateInt(s);
			}

			inline XMVECTOR replicate_scalar(float s)
			{
				return XMVectorReplicate(s);
			}
		}
        
        template <typename _T, size_t _Size>
        struct xmvector
        {
            static constexpr size_t Size = _Size;
            using Scalar = _T;
            typedef xmvector SelfType;

			static_assert(Size > 0 && Size <= 4, "Instantiate xmvector of dimension 0 or greater than 4.");
            
            XMVECTOR v;  
            
            inline xmvector() = default;

            inline explicit xmvector(FXMVECTOR xmv) {
                v = xmv;
            }

            inline explicit xmvector(Scalar s) {
                v = detail::replicate_scalar(s);
            }

			// vector<scalar, size-1> + scalar
			template <size_t vSize>
            inline explicit xmvector(xmvector<Scalar, vSize> v0, std::enable_if_t<Size == vSize + 1, Scalar> s){
                v = detail::set_element<Size-1>(v0,s);
            }
            
            template <int _NewSize, typename _NewType = Scalar>
            inline xmvector<_NewType,_NewSize> as() const {
                return reinterpret_cast<const xmvector<_NewType,_NewSize>&>(*this);
            }

			template <int _NewSize, typename _NewType = Scalar>
			inline xmvector<_NewType, _NewSize>& as() {
				return reinterpret_cast<xmvector<_NewType, _NewSize>&>(*this);
			}

            template <int _NewSize>
            inline explicit operator xmvector<Scalar,_NewSize>() const {
                return as<Scalar,_NewSize>();
            }
        };

		template <typename _T>
		// Specialization for Sizeless (Unkown) vector
		struct xmvector<_T, 0>
		{
			static constexpr size_t Size = 0;
			using Scalar = _T;
			typedef xmvector SelfType;
			XMVECTOR v;
		};
        
        template <typename _T>
        struct xmscalar : public xmvector<_T,1>
        {
            typedef xmscalar SelfType;

            inline xmscalar() = default;

            inline xmscalar(Scalar s) {
                 v = detail::replicate_scalar(s);
            }

            inline explicit xmscalar(FXMVECTOR xmv) {
                v = xmv;
            }
            
            inline xmscalar& XM_CALLCONV operator=(xmscalar rhs){
                v = rhs.v;
            }

            inline xmscalar& operator=(Scalar s) {
                 v = detail::rep_scalar(s); 
                 return *this;
            }
        };

		namespace detail
		{
			enum components_name_enums
			{
				_x = 0,
				_y = 1,
				_z = 2,
				_w = 3
			};
		}

#define _XX_COMP_GET(type,elem,Func) \
		template <size_t Size> \
		inline std::enable_if_t< (Size > detail:: elem), type> XM_CALLCONV get##elem(const xmvector<type, Size> xmv) \
		{ return _DXMEXT XMVector##Func(xmv.v); }

#define _XX_COMP_SET(type,elem,Func) \
		template <size_t Size> \
		inline std::enable_if_t< (Size > detail:: elem), xmvector<type, Size>> XM_CALLCONV set##elem(const xmvector<type, Size> xmv ,type value) \
		{ \
			xmvector<type, Size> ret; \
			ret.v = _DXMEXT XMVector##Func(xmv.v, value); \
			return ret; \
		}

		_XX_COMP_GET(float, _x, GetX);
		_XX_COMP_GET(float, _y, GetY);
		_XX_COMP_GET(float, _z, GetZ);
		_XX_COMP_GET(float, _w, GetW);

		_XX_COMP_GET(int, _x, GetXInt);
		_XX_COMP_GET(int, _y, GetYInt);
		_XX_COMP_GET(int, _z, GetZInt);
		_XX_COMP_GET(int, _w, GetWInt);

		_XX_COMP_SET(float, _x, SetX);
		_XX_COMP_SET(float, _y, SetY);
		_XX_COMP_SET(float, _z, SetZ);
		_XX_COMP_SET(float, _w, SetW);

		_XX_COMP_SET(int, _x, SetXInt);
		_XX_COMP_SET(int, _y, SetYInt);
		_XX_COMP_SET(int, _z, SetZInt);
		_XX_COMP_SET(int, _w, SetWInt);

		using xmvector1f = xmvector<float, 1>;
        using xmvector2f = xmvector<float, 2>;
        using xmvector3f = xmvector<float, 3>;
        using xmvector4f = xmvector<float, 4>;

		using xmvector1i = xmvector<int, 1>;
		using xmvector2i = xmvector<int, 2>;
        using xmvector3i = xmvector<int, 3>;
        using xmvector4i = xmvector<int, 4>;

		namespace detail
		{
			using std::conditional;

			template <typename IS>
			struct swizzle_impl;

			template <index_t... SwizzleArgs>
			struct swizzle_impl<index_sequence<SwizzleArgs...>>
			{
				inline static XMVECTOR XM_CALLCONV invoke(FXMVECTOR v)
				{
					return _DXMEXT XMVectorSwizzle<uint32_t(SwizzleArgs)...>(v);
				}
			};

			template <typename IS>
			struct permute_impl;

			template <index_t... PermutesArgs>
			struct permute_impl<index_sequence<PermutesArgs...>>
			{
				inline static XMVECTOR XM_CALLCONV invoke(FXMVECTOR v0, FXMVECTOR v1)
				{
					return _DXMEXT XMVectorPermute<uint32_t(PermutesArgs)...>(v0,v1);
				}
			};

			template <typename IS>
			struct select_impl;

			template <index_t... Masks>
			struct select_impl<index_sequence<Masks...>>
			{
				static constexpr uint32_t zero_or_all(index_t val)
				{
					return val ? 0xFFFFFFFF : 0;
				}

				inline static XMVECTOR XM_CALLCONV invoke(FXMVECTOR v0, FXMVECTOR v1)
				{
					XMVECTOR control = XMVectorSetInt(zero_or_all(Masks)...);
					return XMVectorSelect(v0, v1, control);
				}
			};

			template <typename Mask, typename IS>
			struct sequence_to_mask_impl;

			template <index_t... Masks, index_t Head, index_t... Indices>
			struct sequence_to_mask_impl<index_sequence<Masks...>, index_sequence<Head, Indices...>>
				: public sequence_to_mask_impl< set_element_t<Head, 1, index_sequence<Masks...>>, index_sequence<Indices...>>
			{};

			template <index_t... Masks>
			struct sequence_to_mask_impl<index_sequence<Masks...>, index_sequence<>>
			{
				using type = index_sequence<Masks...>;
			};

			template <typename IS>
			struct sequence_to_mask : public sequence_to_mask_impl<index_sequence<0, 0, 0, 0>, IS>
			{};

			template <typename IS1, typename IS2>
			struct concat_swizzle_sequence;

			template <index_t... IS1, index_t... IS2>
			struct concat_swizzle_sequence <index_sequence<IS1...>, index_sequence<IS2...>>
			{
				using type = index_sequence<(get_element<IS2, index_sequence<IS1...>>::value)...>;
			};

			template <typename IS>
			struct is_permutation_sequence;

			template <index_t Divisor, typename IS1>
			struct div_sequence;

			template <index_t Divisor,index_t... Vals>
			struct div_sequence<Divisor, index_sequence<Vals...>>
			{
				using quotient = index_sequence<(Vals / Divisor)...>;
				using remainder = index_sequence<(Vals % Divisor)...>;
			};

			// sort the sequence IS1, swizzling IS2 as the way to form IS1
			template <typename IS1, typename IS2>
			struct dual_sort_sequence;

			template <size_t... IS1, size_t... IS2>
			struct dual_sort_sequence<index_sequence<IS1...>, index_sequence<IS2...>>
			{
				static_assert(sizeof...(IS1) == sizeof...(IS2), "Sequence length must agree");
				using mad_seq = typename index_sequence<(IS1*4 + IS2)...>::type;
				using sorted_mad = typename sort_sequence<mad_seq>::type;
				using type = typename div_sequence<4,sorted_mad>::remainder;
			};


			template <typename IS>
			struct inverse_swizzle_sequence;

			template <typename IS1, typename IS2>
			using concat_swizzle_sequence_t = typename concat_swizzle_sequence<IS1, IS2>::type;

		}

		// This type is a wrapper for containning the swizzle and mask information of a xmvector
		// Your should never construct this type like xmvector_swizzler<float,_y,_z> xmv;
		// instead, use function swizzle on an existed xmvector to construct
		// helps to optimize in the case case of masked/swizzled assignment: v.yzw = p, v.wyz = p.xzw;
		template <typename _T, index_t... _SwzArgs>
		struct xmvector_swizzler
		{
			XMVECTOR v;

			xmvector_swizzler() = delete;
			xmvector_swizzler(const xmvector_swizzler& rhs) = delete;

			using Scalar = _T;
			static constexpr size_t Size = sizeof...(_SwzArgs);

			static_assert(Size <= 4, "Swizzle element count out of 4");
			static_assert(std::conjunction <std::integral_constant<bool,_SwzArgs < 4>...>::value, "Swizzle index out of [0,3]");

			using Indices = index_sequence<_SwzArgs...>;
			using IndirectType = xmvector<Scalar, Size>;
			using MergedIndices = conditional_t<Size == 4, Indices, overwrite_sequence_t<Indices, index_sequence<0,1,2,3>>>;

			inline IndirectType XM_CALLCONV eval() const
			{
				IndirectType ret;
				ret.v = detail::swizzle_impl<MergedIndices>::invoke(v);
				return ret;
			}

			inline XM_CALLCONV operator IndirectType() const
			{
				return eval();
			}

			template <index_t... _SrcSwzArgs>
			inline std::enable_if_t<sizeof...(_SrcSwzArgs) == sizeof...(_SwzArgs)> XM_CALLCONV assign_by(const xmvector_swizzler<_T, _SrcSwzArgs...> src);
		};

		// Indentiy swizzlers
		// template <typename _T> struct xmvector_swizzler<_T, 0, 1, 2, 3> : public xmvector<_T, 4> {};
		// template <typename _T> struct xmvector_swizzler<_T, 0, 1, 2> : public xmvector<_T, 3> {};
		// template <typename _T> struct xmvector_swizzler<_T, 0, 1> : public xmvector<_T, 2>	{};
		// template <typename _T> struct xmvector_swizzler<_T, 0> : public xmvector<_T, 1> {};


		namespace detail
		{
			template <typename _T, typename IS>
			struct swizzle_from_indecis;

			template <typename _T, size_t _Size, index_t... _SwzArgs>
			struct swizzle_from_indecis<xmvector<_T, _Size>, index_sequence<_SwzArgs...>>
			{
				using type = xmvector_swizzler<_T, _SwzArgs...>;
			};

			template <typename _T, typename IS>
			using swizzle_from_indecis_t = typename swizzle_from_indecis<_T, IS>::type;

			template <typename XMV_S1, index_t... SW2>
			struct xmvector_swizzler_concat;

			template <typename _T, index_t... SW1, index_t... SW2>
			struct xmvector_swizzler_concat<xmvector_swizzler<_T, SW1...>, SW2...>
			{
				using type = swizzle_from_indecis_t<
					xmvector<_T, sizeof...(SW1)>,
					concat_swizzle_sequence_t<
						index_sequence<SW1...>,
						index_sequence<SW2... >> >;
			};

			template <typename XMV_S1, index_t... SW2>
			using xmvector_swizzler_concat_t = typename xmvector_swizzler_concat<XMV_S1, SW2...>::type;
		}

		// any to any assignment
		// v4.yz = v3.zy;
		template <typename _Ty, index_t... _DstSwz, index_t... _SrcSwz>
		inline std::enable_if_t<sizeof...(_DstSwz) == sizeof...(_SrcSwz), XMVECTOR>
			XM_CALLCONV
			swizzle_assign(xmvector_swizzler<_Ty, _DstSwz...>&& dst, xmvector_swizzler<_Ty, _SrcSwz...>&& src)
		{
			using permute_sequence = typename indirect_assign<index_sequence<0, 1, 2, 3>, index_sequence<_DstSwz...>, index_sequence<(_SrcSwz+4)...>>::type;

			return detail::permute_impl<permute_sequence>::invoke(dst.v, src.v);
		}

		// 4 to 4 assignment
		// v4.wyxz = v4.yxwz;
		template <typename _Ty, index_t... _DstSwz, index_t... _SrcSwz>
		inline std::enable_if_t<sizeof...(_DstSwz) == 4 && sizeof...(_DstSwz) == 4, XMVECTOR>
			XM_CALLCONV
			swizzle_assign(xmvector_swizzler<_Ty, _DstSwz...>&& dst, xmvector_swizzler<_Ty, _SrcSwz...>&& src)
		{
			xmvector<_Ty, 0> ret;
			using permute_sequence = typename indirect_assign<index_sequence<-1, -1, -1, -1>, index_sequence<_DstSwz...>, index_sequence<_SrcSwz...>>::type;

			return detail::swizzle_impl<permute_sequence>::invoke(src.v);
		}

		// identity to any assignment
		// v4.yxz = v3;
		template <typename _Ty, index_t... _DstSwz, index_t SrcSize>
		inline std::enable_if_t<sizeof...(_DstSwz) == SrcSize, XMVECTOR>
			XM_CALLCONV
			swizzle_assign(xmvector_swizzler<_Ty, _DstSwz...>&&  dst, xmvector<_Ty, SrcSize>&& src)
		{
			using src_type = detail::swizzle_from_indecis_t<_Ty, std::make_index_sequence<SrcSize>>;
			auto&& srcv = reinterpret_cast<src_type&&>(src);
			return swizzle_assign(std::move(dst), std::move(srcv));
		}

		// When src and dst swzzle are same, this becomes an XMVectorSelect Masked Assignment Problem
		// v4.zxy = v4.zxy
		template <typename _Ty, index_t... _SwzzleArgs>
		inline XMVECTOR
			XM_CALLCONV
			swizzle_assign(xmvector_swizzler<_Ty, _SwzzleArgs...>&& dst, xmvector_swizzler<_Ty, _SwzzleArgs...>&& src)
		{
			using ret_type = xmvector<_Ty, 0>;
			ret_type ret;
			using mask_seq = typename detail::sequence_to_mask<index_sequence<_SwzzleArgs...>>::type;
			return detail::select_impl<mask_seq>::invoke(src.v, dst.v);
		}

		template <index_t... _SwzArgs, typename _T, size_t _Size>
		inline auto&& XM_CALLCONV swizzle(const xmvector<_T, _Size>& xmv) {
			static_assert(sizeof...(_SwzArgs) <= 4, "Swizzle element count out of 4");
			static_assert(std::conjunction < std::integral_constant<bool,_SwzArgs < _Size>...>::value, "Swizzle index out of source vector size");
			return reinterpret_cast<const xmvector_swizzler<_T, _SwzArgs...>&&>(xmv);
		}

		template <index_t... _SwzArgs, typename _T, size_t _Size>
		inline auto&& XM_CALLCONV swizzle(xmvector<_T, _Size>& xmv) {
			static_assert(sizeof...(_SwzArgs) <= 4, "Swizzle element count out of 4");
			static_assert(std::conjunction < std::integral_constant<bool,_SwzArgs < _Size>...>::value, "Swizzle index out of source vector size");
			return reinterpret_cast<xmvector_swizzler<_T, _SwzArgs...>&&>(xmv);
		}

		template <index_t... _SwzArgs, typename _T, index_t... _PrevSwzArgs>
		inline auto&& XM_CALLCONV swizzle(xmvector_swizzler<_T, _PrevSwzArgs...>&& xmv) {
			static constexpr size_t _Size = sizeof...(_PrevSwzArgs);
			static_assert(sizeof...(_SwzArgs) <= 4, "Swizzle element count out of 4");
			static_assert(std::conjunction < std::integral_constant<bool,_SwzArgs < _Size>...>::value, "Swizzle index out of source vector size");
			using src_t = xmvector_swizzler<_T, _PrevSwzArgs...>;
			return reinterpret_cast<const detail::xmvector_swizzler_concat_t<src_t, _SwzArgs...>&&>(xmv);
		}

		template<typename _T, index_t ..._SwzArgs>
		template<index_t... _SrcSwzArgs>
		inline std::enable_if_t<sizeof...(_SrcSwzArgs) == sizeof...(_SwzArgs)> XM_CALLCONV xmvector_swizzler<_T, _SwzArgs...>::assign_by(const xmvector_swizzler<_T, _SrcSwzArgs...> src)
		{
			this->v = swizzle_assign(*this, src);
		}

}
}