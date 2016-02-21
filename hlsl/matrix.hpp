#ifndef HLSL_SIMD_MATRIX_HPP
#define HLSL_SIMD_MATRIX_HPP

//  Copyright (c) 2012 Patrick Grison
//  Distributed under the MIT Software License


#pragma once

#include "float_xvector.hpp"
#include "vector.hpp"

namespace hlsl
{
	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	/// Matrix

	typedef DirectX::XMMATRIX matrix;

	inline matrix identity() 
	{
		return DirectX::XMMatrixIdentity();
	}

	inline matrix transpose( const matrix &m )
	{
		return DirectX::XMMatrixTranspose(m);
	}

	inline float_scalar_xvector determinant( const matrix &m )
	{
		return DirectX::XMMatrixDeterminant(m);
	}

	inline matrix invert( const matrix &m )
	{
		return DirectX::XMMatrixInverse(nullptr, m);
	}

	inline matrix scale( const float3x &vector )
	{
		return DirectX::XMMatrixScalingFromVector( vector );
	}

	inline matrix translate( const float3x &vector )
	{
		return DirectX::XMMatrixTranslationFromVector( vector );
	}

	inline bool decompose( const matrix &m, float3x &scale, float4x &quat, float3x &trans )
	{
		return DirectX::XMMatrixDecompose( &scale.vector, &quat.vector, &trans.vector, m);
	}	

	template<typename T> typename std::enable_if<is_xvector_convertible<T,float3x>::value, float3x>::type operator * ( const T &v, const matrix &m )
	{
		return DirectX::XMVector3Transform( float3x(v), m );
	}

	template<typename T> typename std::enable_if<is_xvector_convertible<T,float4x>::value, float4x>::type  operator * ( const T &v, const matrix &m )
	{
		return DirectX::XMVector4Transform( float4x(v), m );
	}	

	template<typename T> typename std::enable_if<is_xvector_convertible<T,float3x>::value, float3x>::type rotate( const T &v, const matrix &m )
	{
		return DirectX::XMVector3TransformNormal( float3x(v), m );
	}	

	template<typename T> typename std::enable_if<is_xvector_convertible<T,float3x>::value, float4x>::type project( const T &v, const matrix &m )
	{
		return DirectX::XMVector3TransformCoord( float3x(v), m );		
	}	

} // namespace

#endif