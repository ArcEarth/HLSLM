#ifndef HLSL_XVECTOR_OPERATORS_HPP
#define HLSL_XVECTOR_OPERATORS_HPP

//  Copyright (c) 2012 Patrick Grison
//  Distributed under the MIT Software License

#pragma once

#include "traits.hpp"

namespace hlsl
{		
	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	// Vectorized operators	

	// applies on both "memory" vectors that specialises a get_xvector<>
	// and on "register" vectors (float_xvector<>) for scalar,vector operators	

	// negate operator
	template<typename T> typename get_xvector<T>::type operator - ( const T &first )
	{
		typedef get_xvector<T>::type X;
		return -X(first);
	}
	
	//
	// operator +
	//
	template<typename T1, typename T2> typename get_common_xvector<T1,T2>::type operator + ( const T1 &first, const T2 &second )
	{
		typedef get_common_xvector<T1,T2>::type X;
		return X(first) + X(second);
	}

	// vector + scalar
	template<typename T1, typename T2> typename if_vectorizable_vector_and_scalar<T1,T2>::type operator + ( const T1 &first,  const T2 &second )
	{
		typedef get_xvector<T1>::type X;
		return X(first) + second;
	}

	// scalar + vector
	template<typename T1, typename T2> typename if_vectorizable_and_scalar<T2,T1>::type operator + ( const T1 &first,  const T2 &second )
	{
		return second + first;
	}

	//
	// operator -
	//
	template<typename T1, typename T2> typename get_common_xvector<T1,T2>::type operator - ( const T1 &first, const T2 &second )
	{
		typedef get_common_xvector<T1,T2>::type X;
		return X(first) - X(second);
	}

	// vector - scalar
	template<typename T1, typename T2> typename if_vectorizable_vector_and_scalar<T1,T2>::type operator - ( const T1 &first,  const T2 &second )
	{
		typedef get_xvector<T1>::type X;
		return X(first) - second;
	}

	// scalar - vector
	template<typename T1, typename T2> typename if_vectorizable_and_scalar<T2,T1>::type operator - ( const T1 &first,  const T2 &second )
	{
		typedef get_xvector<T2>::type X;
		return X(first) - X(second);
	}

	//
	// operator *
	//
	template<typename T1, typename T2> typename get_common_xvector<T1,T2>::type operator * ( const T1 &first, const T2 &second )
	{
		typedef get_common_xvector<T1,T2>::type X;
		return X(first) * X(second);
	}

	// vector*scalar
	template<typename T1, typename T2> typename if_vectorizable_vector_and_scalar<T1,T2>::type operator * ( const T1 &first,  const T2 &second )
	{		
		typedef get_xvector<T1>::type X;
		return X(first) * second;
	}

	// scalar*vector
	template<typename T1, typename T2> typename if_vectorizable_and_scalar<T2,T1>::type operator * ( const T1 &first,  const T2 &second )
	{
		return second * first;
	}

	//
	// operator /
	//
	template<typename T1, typename T2> typename get_common_xvector<T1,T2>::type operator / ( const T1 &first, const T2 &second )
	{
		typedef get_common_xvector<T1,T2>::type X;
		return X(first) / X(second);
	}

	// vector/scalar
	template<typename T1, typename T2> typename if_vectorizable_vector_and_scalar<T1,T2>::type operator / ( const T1 &first,  const T2 &second )
	{
		typedef get_xvector<T1>::type X;
		return X(first) / second;
	}	

	// scalar/vector
	template<typename T1, typename T2> typename if_vectorizable_and_scalar<T2,T1>::type operator / ( const T1 &first,  const T2 &second )
	{
		typedef get_xvector<T2>::type X;
		return X(first) / X(second);
	}
	
}

#endif