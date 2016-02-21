#ifndef HLSL_VECTOR_OPERATORS_HPP
#define HLSL_VECTOR_OPERATORS_HPP

//  Copyright (c) 2012 Patrick Grison
//  Distributed under the MIT Software License


#pragma once

#include "traits.hpp"

namespace hlsl
{
	
	// unvectorized operators (returns unaligned variants: alignment is not controllable for temporary instances, provoking unaligned load/stores )	
	// scalars "operator(float)" will match the generic operators

	template<typename T> T negate( const T &value )
	{
		return -value;
	}

	// negate	
	template<typename T> typename if_is_generic_vector<T>::type operator - ( const T &first )
	{
		return negate(first);
	}

	//
	// operator +
	//
	template<typename T1, typename T2> typename if_are_generic_vectors<T1,T2>::type operator + ( const T1 &first, const T2 &second )
	{
		get_best_unaligned<T1,T2>::type result( first );
		result += second;
		return result;			
	}
	
	// vector+scalar
	template<typename T1, typename T2> typename if_generic_vector_and_scalar<T1,T2>::type operator + ( const T1 &first, const T2 &second )
	{
		get_unaligned<T1>::type result( first );
		result += second;
		return result;
	}

	// scalar+vector
	template<typename T1, typename T2> typename if_generic_vector_and_scalar<T2,T1>::type operator + ( const T1 &first, const T2 &second )
	{		
		return second + first;
	}

	//
	// operator -
	//
	template<typename T1, typename T2> typename if_are_generic_vectors<T1,T2>::type operator - ( const T1 &first, const T2 &second )
	{
		get_best_unaligned<T1,T2>::type result( first );
		result -= second;
		return result;			
	}

	// vector-scalar
	template<typename T1, typename T2> typename if_generic_vector_and_scalar<T1,T2>::type operator - ( const T1 &first, const T2 &second )
	{
		get_unaligned<T1>::type result( first );
		result -= second;
		return result;
	}

	// scalar-vector
	template<typename T1, typename T2> typename if_generic_vector_and_scalar<T2,T1>::type operator - ( const T1 &first, const T2 &second )
	{		
		get_unaligned<T2>::type result( first );
		result -= second;
		return result;
	}

	//
	// operator *
	//
	template<typename T1, typename T2> typename if_are_generic_vectors<T1,T2>::type operator * ( const T1 &first, const T2 &second )
	{
		get_best_unaligned<T1,T2>::type result( first );
		result *= second;
		return result;			
	}

	// vector*scalar
	template<typename T1, typename T2> typename if_generic_vector_and_scalar<T1,T2>::type operator * ( const T1 &first, const T2 &second )
	{
		get_unaligned<T1>::type result( first );
		result *= second;
		return result;
	}

	// scalar*vector
	template<typename T1, typename T2> typename if_generic_vector_and_scalar<T2,T1>::type operator * ( const T1 &first, const T2 &second )
	{		
		return second * first;
	}

	//
	// operator / 
	//
	template<typename T1, typename T2> typename if_are_generic_vectors<T1,T2>::type operator / ( const T1 &first, const T2 &second )
	{
		get_best_unaligned<T1,T2>::type result( first );
		result /= second;
		return result;			
	}

	// vector/scalar
	template<typename T1, typename T2> typename if_generic_vector_and_scalar<T1,T2>::type operator / ( const T1 &first, const T2 &second )
	{
		get_unaligned<T1>::type result( first );
		result /= second;
		return result;
	}

	// scalar/vector
	template<typename T1, typename T2> typename if_generic_vector_and_scalar<T2,T1>::type operator / ( const T1 &first, const T2 &second )
	{		
		get_unaligned<T2>::type result( first );
		result /= second;
		return result;
	}

} // namespace

#endif