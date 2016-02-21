#ifndef HLSL_XM_TRAITS_HPP
#define HLSL_XM_TRAITS_HPP

//  Copyright (c) 2012 Patrick Grison
//  Distributed under the MIT Software License


#pragma once

#include "float_xvector.hpp"

namespace hlsl
{	
	template<> struct get_xvector_impl<DirectX::XMFLOAT2>
	{
		enum { value=true };
		typedef float_xvector<2> type;
	};		

	template<> struct get_xvector_impl<DirectX::XMFLOAT3>
	{
		enum { value=true };
		typedef float_xvector<3> type;
	};	
	
	template<> struct get_xvector_impl<DirectX::XMFLOAT4>
	{
		enum { value=true };
		typedef float_xvector<4> type;
	};	

	template<> struct get_xvector_impl<DirectX::XMFLOAT2A> : get_xvector_impl<DirectX::XMFLOAT2>{};
	template<> struct get_xvector_impl<DirectX::XMFLOAT3A> : get_xvector_impl<DirectX::XMFLOAT3>{};
	template<> struct get_xvector_impl<DirectX::XMFLOAT4A> : get_xvector_impl<DirectX::XMFLOAT4>{};
}

#endif