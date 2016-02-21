#ifndef HLSL_VECTOR_HPP
#define HLSL_VECTOR_HPP

//  Copyright (c) 2012 Patrick Grison
//  Distributed under the MIT Software License


#pragma once

#include <boost/operators.hpp>
#include "traits.hpp"
#include "float_xvector.hpp"

namespace hlsl
{	
	template<typename T, size_t N, bool A> struct vector {};	

	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////

	// unaligned floatX

	typedef vector<float,2,false> float2;
	typedef vector<float,3,false> float3;
	typedef vector<float,4,false> float4;

	// aligned floatX

	typedef __declspec( align(16) ) vector<float,2,true> float2a;
	typedef __declspec( align(16) ) vector<float,3,true> float3a;
	typedef __declspec( align(16) ) vector<float,4,true> float4a;	

	// doubleX

	typedef vector<double,2,false> double2;
	typedef vector<double,3,false> double3;
	typedef vector<double,4,false> double4;	
	
	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////

	// vector<> traits

	template<typename T, size_t N, bool A> struct is_vector_impl<typename vector<T,N,A>> : std::true_type {};		

	template<typename T, size_t N, bool A> struct is_floating_point_impl<typename vector<T,N,A>> : std::is_floating_point<T> {};		

	template<typename T, size_t N, bool A> struct get_dimensions_impl<typename vector<T,N,A>> { enum { value=N }; };

	template<typename T, size_t N, bool A> struct get_element_impl<typename vector<T,N,A>> { enum { value=true }; typedef T type; };

	template<typename T, size_t N, bool A> struct get_unaligned_impl<typename vector<T,N,A>>
	{
		enum { value = true };
		typedef vector<T,N,false> type;
	};

	template<typename T, size_t N, bool A> auto negate( const vector<T,N,A> &vector ) -> decltype( vector.negate() )
	{
		return vector.negate();
	}

	template<typename T, typename U, size_t N, bool A, bool UA> struct worst_limits<vector<T,N,A>, vector<U,N,UA>> : worst_limits<T,U> {};	

	///////////////////////////////////////////////////////////////////////////////////////	
	///////////////////////////////////////////////////////////////////////////////////////
	// Comparison templates: using && at member level will generate short jumps, & will generate set/and (will depends on context, stays on &&)


	template<typename T, typename U,bool A, bool UA, typename F> bool test_and( const vector<T,2,A> &first, const vector<U,2,UA> &second, F func )
	{
		return func(first.x, second.x) && func(first.y, second.y);
	}

	template<typename T, typename U,bool A, bool UA, typename F> bool test_and( const vector<T,3,A> &first, const vector<U,3,UA> &second, F func )
	{
		return func(first.x, second.x) && func(first.y, second.y) && func(first.z, second.z);
	}

	template<typename T, typename U,bool A, bool UA, typename F> bool test_and( const vector<T,4,A> &first, const vector<U,4,UA> &second, F func )
	{
		return func(first.x, second.x) && func(first.y, second.y) && func(first.z, second.z) && func(first.w, second.w);
	}


	///////////////////////////////////////////////////////////////////////////////////////	
	///////////////////////////////////////////////////////////////////////////////////////


	// integer (no epsilon) comparison
	template<typename T, typename U> typename if_are_generic_non_float_vectors<T,U>::type equal( const T &first, const U &second )
	{		
		return test_and( first, second, [] (get_element<T>::type a, get_element<U>::type b) { return a==b; } );
	};	

	template<typename T, typename U> typename if_are_generic_non_float_vectors<T,U>::type less( const T &first, const U &second )
	{		
		return test_and( first, second, [] (get_element<T>::type a, get_element<U>::type b) { return a<b; } );		
	};

	template<typename T, typename U> typename if_are_generic_non_float_vectors<T,U>::type greater( const T &first, const U &second )
	{				
		return test_and( first, second, [] (get_element<T>::type a, get_element<U>::type b) { return a > b; } );		
	};

	// floating point (epsilon) comparison
	template<typename T, typename U> typename if_are_generic_float_vectors<T,U>::type equal( const T &first, const U &second )
	{				
		return test_and( first, second, [] (get_element<T>::type a, get_element<U>::type b) { return std::abs(a-b) <= worst_limits<T,U>::epsilon(); } );
	};

	template<typename T, typename U> typename if_are_generic_float_vectors<T,U>::type less( const T &first, const U &second )
	{				
		return test_and( first, second, [] (get_element<T>::type a, get_element<U>::type b) { return a <= ( b - worst_limits<T,U>::epsilon() ); } );		
	};		

	template<typename T, typename U> typename  if_are_generic_float_vectors<T,U>::type greater( const T &first, const U &second )
	{				
		return test_and( first, second, [] (get_element<T>::type a, get_element<U>::type b) { return a >= (b + worst_limits<T,U>::epsilon() ); } );
	};
		
	template<typename T, typename U> bool equal( const T &first, const U &second, typename get_common_xvector<T,U>::type *p = nullptr )
	{
		typedef get_common_xvector<T,U>::type X;				
		return X(first) == X(second);
	}

	template<typename T, typename U> bool less( const T &first, const U &second, typename get_common_xvector<T,U>::type *p = nullptr )
	{
		typedef get_common_xvector<T,U>::type X;
		return X(first) < X(second);
	}

	template<typename T, typename U> bool greater( const T &first, const U &second, typename get_common_xvector<T,U>::type *p = nullptr )
	{
		typedef get_common_xvector<T,U>::type X;
		return X(first) > X(second);
	}

	///////////////////////////////////////////////////////////////////////////////////////	
	///////////////////////////////////////////////////////////////////////////////////////


	template<typename T, size_t N, bool A> struct vector_operators
	{
		typedef vector<T,N,A>		type;
		typedef T					value_type;
		typedef T					array_type[N];		
		typedef vector<T,N,false>	unaligned_vector;		
	
		array_type &c_array() 
		{
			return reinterpret_cast<array_type &>(*(type*)this);
		}
		
		const array_type &c_array() const
		{
			return reinterpret_cast<const array_type &>(*(const type*)this);
		}

		// Accessors

		T &operator[](int e)
		{
			return c_array()[e];
		}		

		const T &operator[](int e) const
		{
			return c_array()[e];
		}	

		operator array_type &()
		{
			return c_array();
		}

		operator const array_type &() const
		{
			return c_array();
		}		

		size_t size() const
		{
			return N;
		}

		// Iterators

		T *begin() const
		{
			return const_cast<T*>( c_array() );
		}

		T *end() const
		{
			return const_cast<T*>( c_array() + N );
		}			
		
		type &operator = ( T value )
		{
			for( size_t i = 0 ; i < N ; ++i )
				(*this)[i] = value;

			return (type&) *this;
		}

		type &operator = ( const array_type &other )
		{
			for( size_t i = 0 ; i < N ; ++i )
				(*this)[i] = other[i];

			return (type&) *this;
		}

		template<typename U, bool UA> type &operator = ( const vector<U,N,UA> &other ) 
		{
			for( size_t i = 0 ; i < N ; ++i )
				(*this)[i] = other[i];

			return (type&) *this;
		}
		
		// Aligned / Unaligned assignment of same type		
		template<bool UA> type &operator = ( const vector<T,N,UA> &other ) 
		{
			*this = reinterpret_cast<const type &> (other);
			return (type&) *this;
		}				

		// Operators
		template<typename U, bool UA> type &operator += ( const vector<U,N,UA> &other )
		{
			for( size_t i = 0 ; i < N ; ++i )
				(*this)[i] += other[i];
	
			return (type&) *this;
		}

		template<typename U> type &operator += ( U value )
		{
			for( size_t i = 0 ; i < N ; ++i )
				(*this)[i] += value;

			return (type&) *this;
		}

		template<typename U, bool UA> type &operator -= ( const vector<U,N,UA> &other )
		{
			for( size_t i = 0 ; i < N ; ++i )
				(*this)[i] -= other[i];

			return (type&) *this;
		}

		template<typename U> type &operator -= ( U value )
		{
			for( size_t i = 0 ; i < N ; ++i )
				(*this)[i] -= value;

			return (type&) *this;
		}

		template<typename U, bool UA> type &operator *= (const vector<U,N,UA> &other)
		{
			for( size_t i = 0 ; i < N ; ++i )
				(*this)[i] *= other[i];

			return (type&) *this;
		}

		template<typename U> type &operator *= ( U value )
		{
			for( size_t i = 0 ; i < N ; ++i )
				(*this)[i] *= value;

			return (type&) *this;
		}

		template<typename U, bool UA> type &operator /= (const vector<U,N,UA> &other)
		{
			for( size_t i = 0 ; i < N ; ++i )
				(*this)[i] /= other[i];

			return (type&) *this;
		}

		template<typename U> type &operator /= ( U value )
		{
			for( size_t i = 0 ; i < N ; ++i )
				(*this)[i] /= value;

			return (type&) *this;
		}

		unaligned_vector negate () const
		{
			unaligned_vector neg;

			for( size_t i = 0 ; i < N ; ++i )
				neg[i] = - (*this)[i];

			return neg;
		}

		// Comparators
		template<typename U> bool operator == ( const U &other ) const
		{
			return equal( (const type &)*this, other );
		}

		template<typename U> bool operator != ( const U &other ) const
		{
			return !equal( (const type &)*this, other );
		}

		template<typename U> bool operator < ( const U &other ) const
		{
			return less( (const type &)*this, other );
		}

		template<typename U> bool operator <= ( const U &other ) const
		{
			return !greater( (const type &)*this, other );
		}

		template<typename U> bool operator > ( const U &other ) const
		{
			return greater( (const type &)*this, other );
		}

		template<typename U> bool operator >= ( const U &other ) const
		{
			return !less( (const type &)*this, other );
		}
	};
	
	///////////////////////////////////////////////////////////////////////////////////////	
	///////////////////////////////////////////////////////////////////////////////////////

	template<typename T, size_t N, bool A, typename B> struct vector_casts : B {};

	// 3D vector : 2D casts
	template<typename T, bool A, typename B> struct vector_casts<T,3,A,B> : B
	{
		typedef vector<T,2,A> xy_type;

		xy_type &xy()
		{
			return reinterpret_cast<xy_type&>( (type&)*this );
		}

		const xy_type &xy() const
		{
			return reinterpret_cast<const xy_type&>( (const type&)*this );
		}

		/*
		xy_type &yz()
		{
			return &reinterpret_cast<xy_type*>( &y );
		}

		const xy_type &yz() const
		{
			return &reinterpret_cast<const xy_type*>( &y );
		}
		*/
	};
	
	// 4D vector : 3D,2D casts
	template<typename T, bool A, typename B> struct vector_casts<T,4,A,B> : vector_casts<T,3,A,B>
	{		
		typedef vector<T,3,A> xyz_type;

		xyz_type &xyz()
		{
			return reinterpret_cast<xyz_type&>( (type&)*this);
		}

		const xyz_type &xyz() const
		{
			return reinterpret_cast<const xyz_type&>( (type&)*this);
		}

		/*
		xy_type &zw()
		{
			return *reinterpret_cast<xy_type*>( &z );
		}

		const xy_type &zw() const
		{
			return *reinterpret_cast<const xy_type*>( &z );
		}
		*/
	};
	
	///////////////////////////////////////////////////////////////////////////////////////	
	///////////////////////////////////////////////////////////////////////////////////////

	template<typename T, size_t N, bool A> struct vector_base : vector_casts<T,N,A,vector_operators<T,N,A>> {};

	///////////////////////////////////////////////////////////////////////////////////////	
	///////////////////////////////////////////////////////////////////////////////////////
	
	template<typename T, bool A> struct vector<T,2,A> : vector_base<T,2,A>
	{														
		T x, y;							
								
		// Constructors

		vector() {}				

		explicit vector( T value ) : x(value), y(value) {}		
		
		vector( T _x, T _y ) : x(_x), y(_y) {}

		template<typename U, bool UA> vector( const vector<U,2,UA> &other ) : x(other.x), y(other.y) {}

		template<typename U> vector( const U (&array)[2] ) : x(array[0]), y(array[1]) {}

		template<typename U> explicit vector( const U *array ) : x(array[0]), y(array[1]) {}	

		// xvector constructor and assignment
				
		template<typename U> vector( const U &xvector, typename std::enable_if<is_xvector_compatible<vector,U>::value>::type *p=nullptr )
		{
			store( *this, xvector );
		}

		template<typename U> typename std::enable_if<is_xvector_compatible<vector,U>::value, vector>::type &operator = ( const U &xvector )
		{
			store( *this, xvector );
			return *this;
		}		
	};	

	template<typename T, bool A> struct vector<T,3,A> : vector_base<T,3,A>
	{							
		T x,y,z;									

		// Constructors

		vector() {}		
		
		explicit vector( T value ) : x(value), y(value), z(value) {}			
				
		vector( T _x, T _y, T _z ) : x(_x), y(_y), z(_z) {}

		template<typename U, bool UA> vector( const vector<U,3,UA> &other ) : x(other.x), y(other.y), z(other.z) {}

		template<typename U> vector( const U (&array)[3] ) : x(array[0]), y(array[1]), z(array[2]) {}

		template<typename U> explicit vector( const U *array ) : x(array[0]), y(array[1]), z(array[2]) {}
		
		// xvector constructor and assignment

		template<typename U> vector( const U &xvector, typename std::enable_if<is_xvector_compatible<vector,U>::value>::type *p=nullptr )
		{
			store( *this, xvector );			
		}

		template<typename U> typename std::enable_if<is_xvector_compatible<vector,U>::value, vector>::type &operator = ( const U &xvector )
		{
			store( *this, xvector );
			return *this;
		}
		
	};			
	
	template<typename T, bool A> struct vector<T,4,A> : vector_base<T,4,A>	
	{						
		T x,y,z,w;		

		// Constructors

		vector() {}		

		explicit vector( T value ) : x(value), y(value), z(value), w(value) {}			

		vector( T _x, T _y, T _z, T _w ) : x(_x), y(_y), z(_z), w(_w) {}

		template<typename U, bool UA> vector( const vector<U,4,UA> &other ) : x(other.x), y(other.y), z(other.z), w(other.w) {}

		template<typename U> vector( const U (&array)[4] ) : x(array[0]), y(array[1]), z(array[2]), w(array[3]) {}

		template<typename U> explicit vector( const U *array ) : x(array[0]), y(array[1]), z(array[2]), w(array[3]) {}

		// xvector constructor and assignment
		
		template<typename U> vector( const U &xvector, typename std::enable_if<is_xvector_compatible<vector,U>::value>::type *p=nullptr )
		{
			store( *this, xvector );		
		}
		
		template<typename U> typename std::enable_if<is_xvector_compatible<vector,U>::value, vector>::type &operator = ( const U &xvector )
		{
			store( *this, xvector );
			return *this;
		}		
	};		

	// get_xvector specializations ( use __DISABLE_GET_XVECTOR__ to force fallback on non-vectorized operators ) 

#ifndef __DISABLE_GET_XVECTOR__

	template<size_t N, bool A> struct get_xvector_impl<typename vector<float,N,A>, typename std::enable_if<N>=2 && N<=4>::type > {
		enum { value = true };
		typedef float_xvector<N> type;
	};

#endif

} // namespace

#endif