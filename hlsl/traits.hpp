#ifndef HLSL_SIMD_TRAITS_HPP
#define HLSL_SIMD_TRAITS_HPP

//  Copyright (c) 2012 Patrick Grison
//  Distributed under the MIT Software License


#pragma once

#include <type_traits>
#include <boost/type_traits/promote.hpp> 

namespace hlsl
{
	/////////////////////////////////////////////////////
	// vectors traits	

	// is_vector : check if the type is a "memory" vector that belongs to this library or its extensions	
	template<typename T> struct is_vector_impl : std::false_type {};	
	template<typename T> struct is_vector : is_vector_impl<typename std::remove_cv<T>::type> {};

	// is_floating_point : check is vector has floating-point elements	
	template<typename T> struct is_floating_point_impl : std::false_type {};
	template<typename T> struct is_floating_point : is_floating_point_impl<typename std::remove_cv<T>::type> {};

	// is_xvector : check if the type is an SIMD "intrinsic register"
	template<typename T> struct is_xvector_impl : std::false_type {};
	template<typename T> struct is_xvector : is_xvector_impl<typename std::remove_cv<T>::type> {};

	// is_scalar_xvector : check if the type is an xvector that encapsulate a scalar result (all channels to the same value)
	template<typename T> struct is_scalar_xvector_impl : std::false_type {};
	template<typename T> struct is_scalar_xvector : is_scalar_xvector_impl<typename std::remove_cv<T>::type> {};

	// is_aligned : check if the type is to be considered as 16-bytes aligned for load/stores
	template<typename T> struct is_aligned_impl : std::integral_constant<bool, __alignof(T)%16==0 > {};	
	template<> struct is_aligned_impl<void> : std::false_type {};
	template<typename T> struct is_aligned : is_aligned_impl<typename std::remove_cv<T>::type> {};	
	
	// get_unaligned : get the unaligned variant of a type, for temporary/return objects whose alignment is not controllable
	template<typename T> struct get_unaligned_impl { 
		enum { value = false };
		typedef T type; 
	};
	template<typename T> struct get_unaligned : get_unaligned_impl<typename std::remove_cv<T>::type> {};


	// get_dimensions : vector dimensions
	template<typename T> struct get_dimensions_impl { enum { value=0 }; };
	template<typename T> struct get_dimensions : get_dimensions_impl<typename std::remove_cv<T>::type> {};

	// get_element : vector element/channel type (int, float, double...)
	template<typename T> struct get_element_impl { enum { value=false }; };
	template<typename T> struct get_element : get_element_impl<typename std::remove_cv<T>::type> {};


	// get_xvector : xvector type (encapsulated intrinsic SIMD register) available for the type
	template<typename T, typename Enable = void> struct get_xvector_impl { enum { value = false }; };	
	template<typename T> struct get_xvector : get_xvector_impl<typename std::remove_cv<T>::type> {};	
	
	// get_scalar : return scalar type from scalar_xvector<>
	template<typename T, typename Enable = void> struct get_scalar_impl { enum { value = false }; typedef T type; };
	template<typename T> struct get_scalar : get_scalar_impl<typename std::remove_cv<T>::type> {};	

	template<typename T> typename get_scalar<T>::type to_scalar( const T &value )
	{
		return get_scalar<T>::type( value );
	}

	////////////////////////////////////////////////////
	// tests		

	// is_same_dimensions
	template<typename T1, typename T2> struct is_same_dimensions : std::integral_constant<bool,get_dimensions<T1>::value == get_dimensions<T2>::value && get_dimensions<T1>::value != 0> {};
	
	// is_xvector_of
	template<typename X, typename T, typename Enable = void> struct is_xvector_of : std::false_type {};
	template<typename X, typename T> struct is_xvector_of<X,T,typename std::enable_if<get_xvector<T>::value>::type> : std::is_same<X, typename get_xvector<T>::type> {};	

	// is_xvector_compatible
	template<typename T, typename X> struct is_xvector_compatible_impl : std::false_type {};
	template<typename T, typename X, typename Enable = void> struct is_xvector_compatible : std::false_type {};
	template<typename T, typename X> struct is_xvector_compatible<T,X,typename std::enable_if<get_xvector<T>::value>::type> : is_xvector_compatible_impl<typename get_xvector<T>::type,typename std::remove_cv<X>::type> {};

	// is_xvector_convertible : check if the type is the same xvector, or can be casted to	
	template<typename T, typename X> struct is_xvector_convertible : std::integral_constant<bool,std::is_same<T,X>::value || is_xvector_of<X,T>::value> {};
		
	// is_element_of
	template<typename T, typename V, typename Enable = void> struct is_element_of : std::false_type {};
	template<typename T, typename V> struct is_element_of<T,V, typename std::enable_if<get_element<T>::value>::type> : std::is_same<T,typename get_element<T>::type> {};
	


	/////////////////////////////////////////////////////
	// xvector operators

	// get_common_xvector : common xvector type usable for a pair a types
	template<typename T1, typename T2, typename Enable = void> struct get_common_xvector {	enum { value = false }; };
	template<typename T1, typename T2> struct get_common_xvector<T1,T2, typename std::enable_if<std::is_same<typename get_xvector<T1>::type, typename get_xvector<T2>::type>::value>::type>
	{
		enum { value = true };
		typedef typename get_xvector<T1>::type type;
	};	
		
	// is_vectorizable_and_scalar : T1 is get_xvector<> capable (vector and float_xvector) and T2 is scalar
	template<typename T1, typename T2> struct is_vectorizable_and_scalar : std::integral_constant<bool,get_xvector<T1>::value && std::is_scalar<T2>::value> {};	

	template<typename T1, typename T2, typename Enable = void> struct if_vectorizable_and_scalar {};
	template<typename T1, typename T2> struct if_vectorizable_and_scalar<T1,T2, typename std::enable_if<is_vectorizable_and_scalar<T1,T2>::value>::type>
	{
		typedef typename get_xvector<T1>::type type;
	};

	// is_vectorizable_vector_and_scalar : T1 is get_xvector<> capable (ONLY vector) and T2 is scalar
	template<typename T1, typename T2> struct is_vectorizable_vector_and_scalar : std::integral_constant<bool,!is_xvector<T1>::value && is_vectorizable_and_scalar<T1,T2>::value> {};
	template<typename T1, typename T2, typename Enable = void> struct if_vectorizable_vector_and_scalar {};
	template<typename T1, typename T2> struct if_vectorizable_vector_and_scalar<T1,T2, typename std::enable_if<is_vectorizable_vector_and_scalar<T1,T2>::value>::type>
	{
		typedef typename get_xvector<T1>::type type;
	};

	/////////////////////////////////////////////////////
	// generic vector operators

	// is_generic_vector : check if is a vector and has not an SIMD xvector usable type
	template<typename T> struct is_generic_vector : std::integral_constant<bool,is_vector<T>::value && !get_xvector<T>::value> {};
	// if_is_generic_vector
	template<typename T, typename R = typename get_unaligned<T>::type> struct if_is_generic_vector : std::enable_if<is_generic_vector<T>::value,R> {};
	// if_generic_vector_and_scalar
	template<typename T1, typename T2, typename R = typename get_unaligned<T1>::type> struct if_generic_vector_and_scalar : std::enable_if<is_generic_vector<T1>::value && std::is_scalar<T2>::value,R> {};
	
	// is_promotable<T1,T2> : checks if T1 can lead to T2 by recursive boost::promotion (float=>double)
	template<typename T1, typename T2, typename T3> struct is_promotable_recursive : is_promotable_recursive<T2,typename boost::promote<T2>::type,T3> {};
	template<typename T1, typename T2> struct is_promotable_recursive<T1,T1,T2> : std::false_type {};
	template<typename T1, typename T2> struct is_promotable_recursive<T1,T2,T2> : std::true_type {};
	template<typename T> struct is_promotable_recursive<T,T,T> : std::false_type {};
	template<typename T1, typename T2> struct is_promotable : is_promotable_recursive<T1,typename boost::promote<T1>::type,T2> {};
	
	// get_best_vector : select T1 or T2 based on element precision (floatX vs doubleX selects doubleX) (floatX vs integerX selects floatX)
	template<typename T1, typename T2, typename Enable = void> struct get_best_vector {	enum { value = false }; };
	template<typename T1, typename T2> struct get_best_vector<T1, T2, typename std::enable_if<is_same_dimensions<T1,T2>::value && get_element<T1>::value && get_element<T2>::value>::type> : std::conditional< is_promotable<typename get_element<T1>::type, typename get_element<T2>::type>::value || (std::is_floating_point<T2>::value && !std::is_floating_point<T1>::value), T2, T1> { enum { value = true }; };

	// get_best_unaligned : select most precise T1/T2 and give its unaligned variant
	template<typename T1, typename T2, typename Enable = void> struct get_best_unaligned {	enum { value = false }; };
	template<typename T1, typename T2> struct get_best_unaligned<T1, T2, typename std::enable_if<get_best_vector<T1,T2>::value>::type > {
		enum { value = true };
		typedef typename get_unaligned<typename get_best_vector<T1,T2>::type>::type type;
	};

	// are_generic_vectors : checks if T1/T2 are vectors, of same dimensions, and that can't be both casted to a common SIMD xvector
	template<typename T1, typename T2> struct are_generic_vectors : std::integral_constant<bool,is_same_dimensions<T1,T2>::value && (is_vector<T1>::value && is_vector<T2>::value) && !get_common_xvector<T1,T2>::value> {};
	// if_are_generic_vectors
	template<typename T1, typename T2, typename R = typename get_best_unaligned<T1,T2>::type> struct if_are_generic_vectors : std::enable_if<are_generic_vectors<T1,T2>::value,R> {};
		
	
	// are_generic_float_vectors : checks as are_generic_vectors but if T1 and T2 are floating point
	template<typename T1, typename T2> struct are_generic_float_vectors : std::integral_constant<bool,are_generic_vectors<T1,T2>::value && is_floating_point<T1>::value && is_floating_point<T2>::value > {};
	template<typename T1, typename T2, typename R = bool > struct if_are_generic_float_vectors : std::enable_if< are_generic_float_vectors<T1,T2>::value, R> {};
	
	template<typename T1, typename T2> struct are_generic_non_float_vectors : std::integral_constant<bool,are_generic_vectors<T1,T2>::value && !is_floating_point<T1>::value && !is_floating_point<T2>::value > {};
	template<typename T1, typename T2, typename R = bool > struct if_are_generic_non_float_vectors : std::enable_if< are_generic_non_float_vectors<T1,T2>::value, R> {};

	
	/////////////////////////////////////////////////////
	// Generic promotion

	template<typename T, typename Enable = void> struct get_float_promoted_element { enum { value = false }; };
	template<typename T> struct get_float_promoted_element<T,typename std::enable_if<get_element<T>::value>::type>
	{
		enum { value = true }; 
		typedef typename boost::floating_point_promotion<typename get_element<T>::type> type;
	};

	/////////////////////////////////////////////////////
	// limits evaluation
	template<typename T, typename U> struct worst_limits : std::numeric_limits< typename std::conditional< is_promotable<T,U>::value,T,U>::type > {};	

} // namespace

#endif