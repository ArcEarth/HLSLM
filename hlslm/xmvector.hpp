#pragma once

#include <DirectXMath.h>

namespace DirectX
{
    namespace hlsl
    {
        template <typename _T>
        struct xmscalar;

        template <typename _T, int... _SwzArgs>
        struct xmvector_swizzer;
        
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
        
        template <typename _T, int _Size>
        struct xmvector
        {
            static constexpr int Size = _Size;
            using Scalar = _T;
            typedef xmvector SelfType;
            
            XMVECTOR v;  
            
            inline xmvector() = default;

            inline explicit XM_CALLCONV xmvector(FXMVECTOR xmv) {
                v = xmv;
            }

            inline explicit xmvector(Scalar s) {
                v = detail::rep_scalar(s);
            }
            
            inline xmvector(xmvector<Scalar, Size-1> v0, Scalar s){
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

            inline explicit XM_CALLCONV xmscalar(FXMVECTOR xmv) {
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
        
        
        template <typename _T>
        struct xmvector<_T,0>
        {
            static_assert(false,"Instantiate xmvector of dimension zero.");
        };
        
        using xmvector2f = xmvector<float,2>;
        using xmvector3f = xmvector<float,3>;
        using xmvector4f = xmvector<float,4>;

        using xmvector2i = xmvector<float,2>;
        using xmvector3i = xmvector<float,3>;
        using xmvector4i = xmvector<float,4>;
    }
}