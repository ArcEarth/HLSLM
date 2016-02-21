#ifndef HLSL_LOAD_STORE_HPP
#define HLSL_LOAD_STORE_HPP

//  Copyright (c) 2012 Patrick Grison
//  Distributed under the MIT Software License


#pragma once

#include <DirectXMath.h>

#include "traits.hpp"

namespace hlsl
{	
	typedef DirectX::XMVECTOR xmvector;	
	
	// load/store 2D
	inline xmvector load( const DirectX::XMFLOAT2 &mvector )
	{
		return DirectX::XMLoadFloat2( &mvector );
	}

	inline void store( DirectX::XMFLOAT2 &mvector, xmvector xvector )
	{
		DirectX::XMStoreFloat2( &mvector, xvector ); 
	}

	inline xmvector load( const DirectX::XMFLOAT2A &mvector )
	{
		return DirectX::XMLoadFloat2A( &mvector );
	}

	inline void store( DirectX::XMFLOAT2A &mvector, xmvector xvector )
	{
		DirectX::XMStoreFloat2A( &mvector, xvector ); 
	}

	// load/store 3D
	inline xmvector load( const DirectX::XMFLOAT3 &mvector )
	{
		return DirectX::XMLoadFloat3( &mvector );
	}

	inline void store( DirectX::XMFLOAT3 &mvector, xmvector xvector )
	{
		DirectX::XMStoreFloat3( &mvector, xvector ); 
	}

	inline xmvector load( const DirectX::XMFLOAT3A &mvector )
	{
		return DirectX::XMLoadFloat3A( &mvector );
	}

	inline void store( DirectX::XMFLOAT3A &mvector, xmvector xvector )
	{
		DirectX::XMStoreFloat3A( &mvector, xvector ); 
	}

	// load/store 4D
	inline xmvector load( const DirectX::XMFLOAT4 &mvector )
	{
		return DirectX::XMLoadFloat4( &mvector );
	}

	inline void store( DirectX::XMFLOAT4 &mvector, xmvector xvector )
	{
		DirectX::XMStoreFloat4( &mvector, xvector ); 
	}

	inline xmvector load( const DirectX::XMFLOAT4A &mvector )
	{
		return DirectX::XMLoadFloat4A( &mvector );
	}

	inline void store( DirectX::XMFLOAT4A &mvector, xmvector xvector )
	{
		DirectX::XMStoreFloat4A( &mvector, xvector ); 
	}


	template<typename T, size_t N, bool Aligned> struct get_xmtype { enum { value = false }; };

	// float2 unaligned
	template<> struct get_xmtype<float,2,false> {
		enum { value = true };				
		typedef DirectX::XMFLOAT2 type;
	};

	// float2 aligned
	template<> struct get_xmtype<float,2,true> {
		enum { value = true };				
		typedef DirectX::XMFLOAT2A type;
	};

	// float3 unaligned
	template<> struct get_xmtype<float,3,false> {
		enum { value = true };				
		typedef DirectX::XMFLOAT3 type;
	};

	// float3 aligned
	template<> struct get_xmtype<float,3,true> {
		enum { value = true };				
		typedef DirectX::XMFLOAT3A type;
	};

	// float4 unaligned
	template<> struct get_xmtype<float,4,false> {
		enum { value = true };				
		typedef DirectX::XMFLOAT4 type;
	};

	// float4 aligned
	template<> struct get_xmtype<float,4,true> {
		enum { value = true };				
		typedef DirectX::XMFLOAT4A type;
	};	

	////////////////////////

	template<typename T> xmvector load( const T &mvector )
	{
		return load( reinterpret_cast< const get_xmtype<typename get_element<T>::type, get_dimensions<T>::value, is_aligned<T>::value>::type &>( mvector ) );
	}

	template<typename T> void store( T &mvector, xmvector xvector )
	{
		store( reinterpret_cast< get_xmtype<typename get_element<T>::type, get_dimensions<T>::value, is_aligned<T>::value>::type &>( mvector ), xvector );		
	}

	////////////////////////
	// operator << : store() wrapper

	template<typename T, typename X> typename std::enable_if<is_xvector_compatible<T,X>::value, T>::type &operator << ( T &mvector, const X &xvector )
	{
		store( mvector, xvector );
		return mvector;
	}
};

#endif