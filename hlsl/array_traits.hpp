#ifndef HLSL_ARRAY_TRAITS_HPP
#define HLSL_ARRAY_TRAITS_HPP

//  Copyright (c) 2012 Patrick Grison
//  Distributed under the MIT Software License


#pragma once

#include "traits.hpp"
#include <array>

namespace hlsl
{
	template<typename T,size_t N> struct is_vector_impl< T[N] > : std::true_type {};
	template<typename T,size_t N> struct get_dimensions_impl< T[N] > { enum { value=N };	};
	template<typename T,size_t N> struct get_element_impl< T[N] > { enum { value=true }; typedef T type; };
	template<typename T,size_t N> struct is_floating_point_impl< T[N] > : std::is_floating_point<T> {};
	
	template<size_t N> struct get_xvector_impl< float[N], typename std::enable_if<N>=2 && N<=4>::type >
	{
		enum { value=true };
		typedef float_xvector<N> type;
	};	
	
	template<typename T,size_t N> struct is_vector_impl< std::array<T,N> > : std::true_type {};
	template<typename T,size_t N> struct get_dimensions_impl< std::array<T,N> > { enum { value=N };	};
	template<typename T,size_t N> struct get_element_impl< std::array<T,N> > { enum { value=true }; typedef T type; };
	template<typename T,size_t N> struct is_floating_point_impl< std::array<T,N> > : std::is_floating_point<T> {};

	template<size_t N> struct get_xvector_impl< std::array<float,N>, typename std::enable_if<N>=2 && N<=4>::type >
	{
		enum { value=true };
		typedef float_xvector<N> type;
	};

}

#endif