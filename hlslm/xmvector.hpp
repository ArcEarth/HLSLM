#pragma once

#include <DirectXMath.h>
#include "integer_sequence.hpp"

namespace DirectX
{
    namespace hlsl
    {
		using std::size_t;
		using index_t = size_t;

		using std::index_sequence;
		using std::index_sequence_for;
		using std::make_index_sequence;

        template <typename _T>
        struct xmscalar;

		template <typename _T, index_t _Size, index_t... _SwzArgs>
		struct xmvector_swizzer;
        
        namespace detail
        {
			inline xmscalar<int> rep_scalar(int s);
			inline xmscalar<float> rep_scalar(float s);
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
                v = detail::rep_scalar(s);
            }

			// vector<scalar, size-1> + scalar
			template <size_t vSize>
            inline explicit xmvector(xmvector<Scalar, vSize> v0, std::enable_if_t<Size == vSize + 1, Scalar> s){
                v = detail::set_element<Size-1>(v0,s);
            }
            
            template <int _NewSize, typename _NewType = Scalar>
            inline xmvector<_NewType,_NewSize> as() const {
                return retinterpret_cast<const xmvector<_NewType,_NewSize>&>(*this);
            }
            
            template <int _NewSize>
            inline explicit operator xmvector<Scalar,_NewSize>() const {
                return as<Scalar,_NewSize>();
            }
        };
        
        template <typename _T>
        struct xmscalar : public xmvector<_T,1>
        {
            typedef xmscalar SelfType;

            inline xmscalar() = default;

            inline xmscalar(Scalar s) {
                 v = detail::rep_scalar(s); 
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
			inline xmscalar<int> rep_scalar(int s)
			{
				xmscalar<int>(XMVectorReplicateInt(s));
			}

			inline xmscalar<float> rep_scalar(float s)
			{
				xmscalar<float>(XMVectorReplicate(s));
			}
		}
        
        using xmvector2f = xmvector<float,2>;
        using xmvector3f = xmvector<float,3>;
        using xmvector4f = xmvector<float,4>;

        using xmvector2i = xmvector<float,2>;
        using xmvector3i = xmvector<float,3>;
        using xmvector4i = xmvector<float,4>;

		namespace detail
		{
			template <typename IS>
			struct swizzle_impl;

			template <index_t... SwizzleArgs>
			struct swizzle_impl<index_sequence<SwizzleArgs...>>
			{
				inline static XMVECTOR XM_CALLCONV invoke(FXMVECTOR v)
				{
					return XMVectorSwizzle<uint32_t(SwizzleArgs)...>(v);
				}
			};
		}

		template <typename _T, index_t _Size, index_t... _SwzArgs>
		struct xmvector_swizzler
		{
			using SourceType = xmvector<_T, _Size>;
			using Scalar = _T;
			static constexpr size_t SourceSize = SourceType::Size;
			static constexpr size_t IndirectSize = sizeof...(_SwzArgs);
			static constexpr size_t Size = IndirectSize;
			using Indices = index_sequence<_SwzArgs...>;
			using IndirectType = xmvector<Scalar, IndirectSize>;
			using MergedIndices =
				std::conditional_t<IndirectSize == 4,
					Indices,
					std::merge_sequence_t<Indices, std::make_index_sequence<4>>>;

			SourceType xmv;

			static_assert(SourceSize >= IndirectSize, "Swizzle element out of index");

			inline IndirectType XM_CALLCONV eval() const
			{
				IndirectType ret;
				ret.v = detail::swizzle_impl<MergedIndices>::invoke(xmv.v);
				return ret;
			}

			//inline std::enable_if_t<IndirectSize < SourceSize,SourceType>
			//	XM_CALLCONV assign(const IndirectType rhv)
			//{
			//	SourceType ret;
			//	ret.v = XMVectorPermute<>(xmv.v, rhv.v);
			//	return ret;
			//}

			//inline std::enable_if_t<IndirectSize == SourceSize, SourceType>
			//	XM_CALLCONV assign(const IndirectType rhv)
			//{
			//	SourceType ret;
			//	ret.v = XMVectorSwizzle<>(xmv.v, rhv.v);
			//	return ret;
			//}

			inline XM_CALLCONV operator IndirectType() const
			{
				return eval();
			}
		};

		template <typename IS1, typename IS2>
		struct concat_swizzle_sequence;

		template <index_t... IS1, index_t... IS2>
		struct concat_swizzle_sequence <index_sequence<IS1...>, index_sequence<IS2...>>
		{
			using type = index_sequence<(std::get_element<IS2, index_sequence<IS1...>>::value)...>;
		};

		template <typename IS>
		struct is_permutation_sequence;

		template <typename IS>
		struct inverse_swizzle_sequence;

		// sort the sequence IS1, swizzling IS2 as the way to form IS1
		template <typename IS1, typename IS2>
		struct dual_sort_sequence;

		template <typename IS1, typename IS2>
		using concat_swizzle_sequence_t = typename concat_swizzle_sequence<IS1, IS2>::type;

		template <typename _T, typename IS>
		struct swizzle_from_indecis;

		template <typename _T, size_t _Size, index_t... _SwzArgs>
		struct swizzle_from_indecis<xmvector<_T, _Size>, index_sequence<_SwzArgs...>>
		{
			using type = xmvector_swizzler<_T, _Size, _SwzArgs...>;
		};

		template <typename _T, typename IS>
		using swizzle_from_indecis_t = typename swizzle_from_indecis<_T,IS>::type;

		template <typename XMV_S1, index_t... SW2>
		struct xmvector_swizzler_concat;

		template <typename _T, index_t _Size, index_t... SW1, index_t... SW2>
		struct xmvector_swizzler_concat<xmvector_swizzler<_T, _Size, SW1...>, SW2...>
		{
			using type = swizzle_from_indecis_t<
							xmvector<_T,_Size>,
							concat_swizzle_sequence_t<
								index_sequence<SW1...>,
								index_sequence<SW2...>>>;
		};

		template <typename XMV_S1, index_t... SW2>
		using xmvector_swizzler_concat_t = typename xmvector_swizzler_concat<XMV_S1, SW2...>::type;

		template <index_t... _SwzArgs, typename _T, size_t _Size>
		inline auto make_swizzler(const xmvector<_T, _Size>& xmv) {
			return reinterpret_cast<const xmvector_swizzler<_T, _Size, _SwzArgs...>&>(xmv);
		}

		template <index_t... _SwzArgs, typename _T, size_t _Size, index_t... _PrevSwzArgs>
		inline auto make_swizzler(const xmvector_swizzler<_T, _Size, _PrevSwzArgs...>& xmv) {
			using src_t = xmvector_swizzler<_T, _Size, _PrevSwzArgs...>;
			return reinterpret_cast<const xmvector_swizzler_concat_t<src_t, _SwzArgs...>&>(xmv);
		}

		template <index_t... _SwzArgs, typename _T, size_t _Size, index_t... _PrevSwzArgs>
		inline auto make_swizzler(xmvector_swizzler<_T, _Size, _PrevSwzArgs...>& xmv) {
			using src_t = xmvector_swizzler<_T, _Size, _PrevSwzArgs...>;
			return reinterpret_cast<xmvector_swizzler_concat_t<src_t, _SwzArgs...>&>(xmv);
		}

		template <index_t... _SwzArgs, typename _T, size_t _Size>
		inline auto make_swizzler(xmvector<_T, _Size>& xmv) {
			return reinterpret_cast<xmvector_swizzler<_T, _Size, _SwzArgs...>&>(xmv);
		}
    }
}