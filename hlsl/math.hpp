#ifndef HLSL_SIMD_MATH_HPP
#define HLSL_SIMD_MATH_HPP

//  Copyright (c) 2012 Patrick Grison
//  Distributed under the MIT Software License

#pragma once

#include "float_math.hpp"
#include <boost/math/constants/constants.hpp>

namespace hlsl
{
	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////	
	// Math structure resolution

	template<typename T, typename Enable = void> struct math { enum { vectorizable = false }; };

	// float2x
	template<typename T> struct math<T,typename std::enable_if<is_xvector_convertible<T,float2x>::value>::type> : float2x_math { enum { vectorizable = true }; };

	// float3x
	template<typename T> struct math<T,typename std::enable_if<is_xvector_convertible<T,float3x>::value>::type> : float3x_math { enum { vectorizable = true }; };

	// float4x
	template<typename T> struct math<T,typename std::enable_if<is_xvector_convertible<T,float4x>::value>::type> : float4x_math { enum { vectorizable = true }; };


	template<typename T1, typename T2, typename Enable = void> struct math2 { enum { vectorizable = false }; };

	template<typename T1, typename T2> struct math2<T1, T2, typename std::enable_if<std::is_same<typename math<T1>::vector_type,typename math<T2>::vector_type>::value>::type > : math<T1> {};

	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////	

	template<typename T1, typename T2> auto mul( const T1 &first, const T2 &second  ) -> decltype( first * second )
	{
		return first * second;
	}

	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////	

	template<typename T> typename math<T>::vector_type abs( const T &vector )
	{
		return math<T>::abs( vector );
	}

	template<typename T> typename math<T>::vector_type floor( const T &vector )
	{
		return math<T>::floor( vector );
	}

	template<typename T> typename math<T>::vector_type ceil( const T &vector )
	{
		return math<T>::ceil( vector );
	}

	template<typename T> typename math<T>::vector_type min( const T &vector )
	{
		return math<T>::min( vector );
	}

	template<typename T> typename math<T>::vector_type max( const T &vector )
	{
		return math<T>::max( vector );
	}

	template<typename T1, typename T2> typename math2<T1,T2>::vector_type dst( const T1 &first, const T2 &second )
	{
		return math<T1>::dst( first, second, s );
	}

	template<typename T> typename math<T>::vector_type rcp( const T &vector )
	{
		return math<T>::rcp( vector );
	}

	template<typename T> typename math<T>::vector_type rsqrt( const T &vector )
	{
		return math<T>::rsqrt( vector );		
	}

	template<typename T1, typename T2> typename math2<T1,T2>::vector_type lerp( const T1 &first, const T2 &second, float s )
	{
		return math<T1>::lerp( first, second, s );
	}	

	template<typename T1, typename T2> typename math2<T1,T2>::vector_type fmod( const T1 &first, const T2 &second )
	{
		return math<T1>::fmod( first, second );
	}	

	template<typename T> typename math<T>::vector_type pow( const T &first, const typename math<T>::scalar_type &second )
	{
		return math<T>::pow( first, second );
	}	

	template<typename T> typename math<T>::vector_type exp( const T &vector )
	{
		return math<T>::exp( vector );
	}

	template<typename T> typename math<T>::vector_type exp2( const T &vector )
	{
		return math<T>::exp2( vector );
	}

	template<typename T> typename math<T>::vector_type log( const T &vector )
	{
		return math<T>::log( vector );
	}

	template<typename T> typename math<T>::vector_type saturate( const T &vector )
	{
		return math<T>::saturate( vector );
	}	

	template<typename T> T saturate( const T &vector, typename std::enable_if<is_scalar_xvector<T>::value>::type *p=nullptr )
	{
		return all_float_dimensions_math<T>::saturate( vector );
	}	

	template<typename T> typename math<T>::vector_type trunc( const T &vector )
	{
		return math<T>::trunc( vector );
	}

	template<typename T> typename math<T>::vector_type round( const T &vector )
	{
		return math<T>::round( vector );
	}

	// xvector-enforced functions (to be sure to use only the approximate SIMD version if requested)

	template<typename T> typename std::enable_if<is_xvector<T>::value,T>::type cos( const T &x )
	{
		math<T>::cos( x );		
	}

	template<typename T> typename std::enable_if<is_xvector<T>::value,T>::type cosh( const T &x )
	{
		math<T>::cosh( x );		
	}

	template<typename T> typename std::enable_if<is_xvector<T>::value,T>::type sin( const T &x )
	{
		math<T>::sin( x );		
	}

	template<typename T> typename std::enable_if<is_xvector<T>::value,T>::type sinh( const T &x )
	{
		math<T>::sinh( x );		
	}

	template<typename T> typename std::enable_if<is_xvector<T>::value,T>::type tan( const T &x )
	{
		math<T>::tan( x );		
	}

	template<typename T> typename std::enable_if<is_xvector<T>::value,T>::type tanh( const T &x )
	{
		math<T>::tanh( x );		
	}


	template<typename T> typename std::enable_if<is_xvector<T>::value>::type sincos( const T &x, T &sin, T &cos )
	{
		math<T>::sincos( x, sin, cos );		
	}			

	///////////////////

	template<typename T1,typename T2> typename math2<T1,T2>::scalar_type dot( const T1 &first, const T2 &second )
	{		
		return math<T1>::dot( first, second );		
	}	

	template<typename T1,typename T2> typename math2<T1,T2>::vector_type cross( const T1 &first, const T2 &second ) 
	{
		return math<T1>::cross( first, second );		
	}

	template<typename T1,typename T2> typename math2<T1,T2>::vector_type reflect( const T1 &i, const T2 &n ) 
	{
		return math<T>::reflect( i, n );		
	}

	template<typename T1,typename T2> typename math2<T1,T2>::vector_type refract( const T1 &i, const T2 &n, const typename math<T1>::scalar_type &r ) 
	{
		return math<T1>::refract( i, n, r );		
	}

	template<typename T> typename math<T>::scalar_type length( const T &vector )
	{	
		return math<T>::length( vector );				
	}

	template<typename T> typename math<T>::scalar_type length_squared( const T &vector )
	{	
		return math<T>::length_squared( vector );				
	}

	template<typename T1,typename T2> typename math2<T1,T2>::scalar_type distance( const T1 &first, const T2 &second ) // need to protect
	{	
		return math<T1>::distance( first, second );				
	}			

	template<typename T> typename math<T>::vector_type normalize( const T &vector )
	{
		return math<T>::normalize( vector );
	}

	template<typename T> typename math<T>::bool_type isinf( const T &vector )
	{
		return math<T>::isinf( vector );
	}

	template<typename T> typename math<T>::bool_type isnan( const T &vector )
	{
		return math<T>::isnan( vector );
	}

	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////

	// Generic traits

	template<typename T> struct is_not_math_vectorized : std::integral_constant<bool, !math<T>::vectorizable && !is_scalar_xvector<T>::value > {};
	template<typename T1,typename T2> struct are_not_math_vectorized : std::integral_constant<bool, is_not_math_vectorized<T1>::value || is_not_math_vectorized<T2>::value> {};

	template<typename T, typename R=T> struct if_not_math_vectorized : std::enable_if<is_not_math_vectorized<T>::value,R> {};
	template<typename T1, typename T2, typename R = typename get_unaligned<T1>::type> struct if_not_math_vectorized2 : std::enable_if< are_not_math_vectorized<T1,T2>::value && is_same_dimensions<T1,T2>::value, R> {};

	template<typename T, typename R=T> struct if_use_generic_nonscalar_math : std::enable_if<is_not_math_vectorized<T>::value && !std::is_scalar<T>::value, R> {};

	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////

	// Generic math (scalar and other unsupported vector types)

	template<typename T1, typename T2> typename if_not_math_vectorized2<T1,T2, typename get_element<T1>::type>::type dot( const T1 &first, const T2 &second )
	{
		get_element<T1>::type dt = 0;

		for( size_t i = 0 ; i < get_dimensions<T1>::value ; ++i )	
			dt += first[i] * second[i];

		return dt;
	}

	template<typename T1, typename T2> typename if_not_math_vectorized2<T1,T2>::type pow( const T1 &first, const T2 &second )
	{
		get_unaligned<T1>::type result;

		for( size_t i = 0 ; i < get_dimensions<T1>::value ; ++i )	
			result[i] = pow( first[i], second[i] );	

		return result;
	}

	template<typename T1, typename T2> typename std::enable_if<is_not_math_vectorized<T1>::value && std::is_scalar<T2>::value, T1>::type pow( const T1 &first, const T2 &second )
	{
		get_unaligned<T1>::type result;

		for( size_t i = 0 ; i < get_dimensions<T1>::value ; ++i )	
			result[i] = pow( first[i], second );

		return result;
	}

	template<typename T> typename if_not_math_vectorized<T,typename get_element<T>::type>::type length_squared( const T &vector )
	{
		typename get_element<T>::type sum(0);

		for( size_t i = 0 ; i < get_dimensions<T>::value ; ++i )	
			sum += vector[i] * vector[i];		

		return sum;
	}

	template<typename T> typename if_not_math_vectorized<T,typename get_element<T>::type>::type length( const T &vector )
	{
		return sqrt( length_squared( vector ) );
	}

	template<typename T> typename if_not_math_vectorized<T,typename get_unaligned<T>::type>::type normalize( const T &vector )
	{
		get_unaligned<T>::type result;		
				
		auto lgth = length( vector );

		if( lgth > 0 )
			lgth = 1 / lgth;

		for( size_t i = 0 ; i < get_dimensions<T>::value ; ++i )
			result[i] = vector[i] * lgth;

		return result;
	}

	template<typename T, typename U> typename if_not_math_vectorized<typename get_unaligned<T>::type>::type lerp( const T &first, const T &second, U s )
	{
		get_unaligned<T>::type result;		
		result = (second - first) * s + first;		
		return result;
	}		

	template<typename T> typename if_use_generic_nonscalar_math<T,typename get_unaligned<T>::type>::type saturate( const T &vector )
	{
		get_unaligned<T>::type result;

		for( size_t i = 0 ; i < get_dimensions<T>::value ; ++i )
			result[i] = saturate( vector[i] );

		return result;
	}

	template<typename T> typename std::enable_if<std::is_scalar<T>::value,T>::type saturate( T value )
	{		
		// cmov expected
		if( value < 0 )
			return 0;

		if( value > 1 )
			return 1;

		return value;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// pure "vector" functions	

	template<typename T, typename R = typename get_unaligned<T>::type> struct if_vector : std::enable_if<is_vector<T>::value,R> {};

	template<typename T1, typename T2, typename R = typename get_unaligned<T1>::type> struct if_vector2 : std::enable_if<is_vector<T1>::value && is_vector<T2>::value && is_same_dimensions<T1,T2>::value, R> {};

	template<typename T, typename F> typename if_vector<T>::type apply( const T &vector, F func )
	{
		get_unaligned<T>::type result;

		for( size_t i = 0 ; i < get_dimensions<T>::value ; ++i )
			result[i] = func( vector[i] );
		
		return result;
	}	

	template<typename T1, typename T2, typename F> typename if_vector2<T1,T2>::type apply( const T1 &first, const T2 &second, F func )
	{
		get_unaligned<T1>::type result;

		for( size_t i = 0 ; i < get_dimensions<T1>::value ; ++i )
			result[i] = func( first[i], second[i] );

		return result;
	}	

	template<typename T> typename if_vector<T>::type cos( const T &vector )
	{		
		return apply( vector, [] ( const get_element<T>::type &value) { return std::cos(value); }  );
	}

	template<typename T> typename if_vector<T>::type cosh( const T &vector )
	{		
		return apply( vector, [] ( const get_element<T>::type &value) { return std::cosh(value); }  );
	}

	template<typename T> typename if_vector<T>::type sin( const T &vector )
	{		
		return apply( vector, [] ( const get_element<T>::type &value) { return std::sin(value); }  );
	}

	template<typename T> typename if_vector<T>::type sinh( const T &vector )
	{		
		return apply( vector, [] ( const get_element<T>::type &value) { return std::sinh(value); }  );
	}

	template<typename T> typename if_vector<T>::type tan( const T &vector )
	{		
		return apply( vector, [] ( const get_element<T>::type &value) { return std::tan(value); }  );
	}

	template<typename T> typename if_vector<T>::type tanh( const T &vector )
	{		
		return apply( vector, [] ( const get_element<T>::type &value) { return std::tanh(value); }  );
	}

	template<typename T> typename if_vector<T>::type atan( const T &vector )
	{		
		return apply( vector, [] ( const get_element<T>::type &value) { return std::atan(value); }  );
	}

	template<typename T1, typename T2> typename if_vector2<T1,T2>::type atan2( const T1 &y, const T2 &x )
	{		
		return apply( y, x, [] ( const get_element<T1>::type &v_y, const get_element<T2>::type &v_x,) { return std::atan2( v_y, v_x ); }  );
	}

	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	// Generic & vectorized functions

	template<typename T> typename get_unaligned<T>::type degrees( const T &value )
	{							
		return value * get_scalar<T>::type(180.0 / boost::math::constants::pi<double>() );		
	}

	template<typename T> typename get_unaligned<T>::type radians( const T &value )
	{			
		return value * get_scalar<T>::type(boost::math::constants::pi<double>() / 180.0);
	}

} // namespace

#endif