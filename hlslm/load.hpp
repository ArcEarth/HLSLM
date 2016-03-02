#pragma once
#ifndef _HLSL_XM_LOAD_STORE_H
#define _HLSL_XM_LOAD_STORE_H
#ifndef _HLSL_XM_VECTOR_H
#include "xmvector.hpp"
#endif

#include "storage_classes.hpp"

namespace DirectX
{
namespace hlsl
{
	// the corresponding xmvector type of a memory vector
	template <typename _Ty>
	using if_memery_vector_type = typename enable_memery_traits_t<_Ty>::type;

	template <typename _Ty>
	inline if_memery_vector_type<_Ty> load(const _Ty& memery_vector)
	{
		using traits = memery_vector_traits<_Ty>;
		using load_imple = detail::storage_helper<typename traits::scalar, is_aligned<_Ty>::value, traits::cols, traits::rows>;
		typename traits::type ret;
		ret.v = load_imple::load(reinterpret_cast<const typename traits::scalar*>(&memery_vector));
		return ret;
	}

	template <typename _Ty>
	inline if_memery_vector_type<_Ty> load_a(const _Ty& memery_vector)
	{
		using traits = memery_vector_traits<_Ty>;
		using load_imple = detail::storage_helper<typename traits::scalar, true, traits::cols, traits::rows>;
		typename traits::type ret;
		ret.v = load_imple::load(reinterpret_cast<const typename traits::scalar*>(&memery_vector));
		return ret;
	}

	template <typename _Ty>
	inline void XM_CALLCONV store(_Ty& memery_vector, const if_memery_vector_type<_Ty> v)
	{
		using traits = memery_vector_traits<_Ty>;
		using load_imple = detail::storage_helper<typename traits::scalar, is_aligned<_Ty>::value, traits::cols, traits::rows>;
		load_imple::store(reinterpret_cast<typename traits::scalar*>(&memery_vector),v.v);
	}

	template <typename _Ty>
	inline void XM_CALLCONV store_a(_Ty& memery_vector, const if_memery_vector_type<_Ty> v)
	{
		using traits = memery_vector_traits<_Ty>;
		using load_imple = detail::storage_helper<typename traits::scalar, true, traits::cols, traits::rows>;
		load_imple::store(reinterpret_cast<typename traits::scalar*>(&memery_vector),v.v);
	}

	template <typename _Scalar>
	inline std::enable_if_t<scalar_traits<_Scalar>::value, xmscalar<_Scalar>> load(_Scalar scalar)
	{ return xmscalar<_Scalar>(detail::replicate_scalar(scalar)); }
	//template <typename _Ty, index_t... _SwzArgs>
	//typename xmvector<_Ty,sizeof...(_SwzArgs)> load(const xmswizzler<_Ty, _SwzArgs...>&& swzizzler)
	//{ return swzizzler.eval(); }
	template <typename _Ty, index_t... _SwzArgs>
	typename xmvector<_Ty, sizeof...(_SwzArgs)> load(const xmswizzler<_Ty, _SwzArgs...>& swzizzler)
	{ return swzizzler.eval(); }
	template <typename _Ty, index_t... _SwzArgs>
	typename void store(xmswizzler<_Ty, _SwzArgs...>&& swzizzler, const xmvector<_Ty, sizeof...(_SwzArgs)> v)
	{
		swzizzler.assign(v);
	}

	// load function pass by 
	template <typename _VectorType>
	inline std::enable_if_t<is_xmvector<_VectorType>::value, _VectorType&> load(_VectorType& v) { return v; }

	template <typename _VectorType>
	inline std::enable_if_t<is_xmvector<_VectorType>::value, _VectorType&&> load(_VectorType&& v) { return v; }

	template <typename _VectorType>
	inline std::enable_if_t<is_xmvector<_VectorType>::value, const _VectorType&> load(const _VectorType& v) { return v; }


}
}
#endif