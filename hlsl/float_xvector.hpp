#ifndef HLSL_FLOAT_XVECTOR_HPP
#define HLSL_FLOAT_XVECTOR_HPP

//  Copyright (c) 2012 Patrick Grison
//  Distributed under the MIT Software License


#pragma once

#include "load_store.hpp"

namespace hlsl
{	
	template<size_t N> struct float_xvector
	{
		xmvector vector;

		operator xmvector () const
		{
			return vector;
		}				

		operator xmvector & ()
		{
			return vector;
		}

		// constructors
			
		float_xvector() {}

		float_xvector( xmvector v ) : vector( v ) {}			

		template<typename U> float_xvector( const U &other, typename std::enable_if<is_xvector_compatible<U,float_xvector>::value>::type *p=nullptr )
		{
			vector = load( other );
		}	

		float_xvector( float v )
		{
			vector = DirectX::XMVectorReplicate( v );
		}

		float_xvector( float x, float y, float z = 0.0f, float w = 0.0f )
		{
			vector = DirectX::XMVectorSet( x, y, z, w );
		}
				
		// assignment		

		float_xvector &operator = ( xmvector v )
		{
			vector = v;
			return *this;
		}				
		
		float_xvector &operator = ( float v )
		{
			vector = DirectX::XMVectorReplicate( v );
			return *this;
		}						

		void zero()
		{
			vector = DirectX::XMVectorZero();
		}

		// components

		float x() const
		{
			return DirectX::XMVectorGetX( vector );
		}

		float y() const
		{
			return DirectX::XMVectorGetY( vector );
		}

		float z() const
		{
			return DirectX::XMVectorGetZ( vector );
		}

		float w() const
		{
			return DirectX::XMVectorGetW( vector );
		}

		// operators
		
		float_xvector operator - () const
		{
			return DirectX::XMVectorNegate( vector );
		}

		float_xvector operator + ( const float_xvector &other ) const
		{
			return DirectX::XMVectorAdd( vector, other );
		}
		
		float_xvector operator + ( float value ) const
		{
			return DirectX::XMVectorAdd( vector, DirectX::XMVectorReplicate(value) );
		}		

		float_xvector &operator += ( const float_xvector &other ) 
		{
			vector = DirectX::XMVectorAdd( vector, other );
			return *this;
		}

		float_xvector &operator += ( float value )
		{
			vector = DirectX::XMVectorAdd( vector, DirectX::XMVectorReplicate(value) );
			return *this;
		}	

		float_xvector operator - ( const float_xvector &other ) const
		{
			return DirectX::XMVectorSubtract( vector, other );
		}	

		float_xvector operator - ( float value ) const
		{
			return DirectX::XMVectorSubtract( vector, DirectX::XMVectorReplicate(value) );
		}		

		float_xvector operator -= ( const float_xvector &other ) 
		{
			vector = DirectX::XMVectorSubtract( vector, other );
			return *this;
		}

		float_xvector &operator -= ( float value )
		{
			vector = DirectX::XMVectorSubtract( vector, DirectX::XMVectorReplicate(value) );
			return *this;
		}	

		float_xvector operator * ( const float_xvector &other ) const
		{
			return DirectX::XMVectorMultiply( vector, other );
		}

		float_xvector operator * ( float value ) const
		{
			return DirectX::XMVectorScale( vector, value );
		}	

		float_xvector &operator *= ( const float_xvector &other )
		{
			vector = DirectX::XMVectorMultiply( vector, other );
			return *this;
		}

		float_xvector &operator *= ( float value )
		{
			vector = DirectX::XMVectorScale( vector, value );
			return *this;
		}		

		float_xvector operator / ( const float_xvector &other ) const
		{
			return DirectX::XMVectorDivide( vector, other );
		}

		float_xvector operator / ( float value ) const
		{
			return DirectX::XMVectorDivide( vector, DirectX::XMVectorReplicate(value) );
		}

		float_xvector &operator /= ( const float_xvector &other )
		{
			vector = DirectX::XMVectorDivide( vector, other );
			return *this;
		}

		float_xvector &operator /= ( float value )
		{
			vector = DirectX::XMVectorDivide( vector, DirectX::XMVectorReplicate(value) );
			return *this;
		}	

		bool operator == ( const float_xvector &other ) const
		{
			return math<float_xvector>::near_equal( vector, other, DirectX::g_XMEpsilon );
		}

		bool operator != ( const float_xvector &other ) const
		{
			return !( *this == other );
		}
		
		bool operator < ( const float_xvector &other ) const
		{
			return math<float_xvector>::less( vector, other - float_xvector(DirectX::g_XMEpsilon) );
		}	

		bool operator <= ( const float_xvector &other ) const
		{
			return math<float_xvector>::less( vector, other + float_xvector(DirectX::g_XMEpsilon) );
		}	

		bool operator > ( const float_xvector &other ) const
		{
			return math<float_xvector>::greater( vector, other + float_xvector(DirectX::g_XMEpsilon) );
		}	

		bool operator >= ( const float_xvector &other ) const
		{
			return math<float_xvector>::greater( vector, other - float_xvector(DirectX::g_XMEpsilon) );
		}	
	};

	template<size_t N> struct get_element<typename float_xvector<N>> { typedef float type; };
	
	template<size_t N> struct get_dimensions<typename float_xvector<N>> { enum { value=N }; };	
	
	template<size_t N> struct get_xvector<typename float_xvector<N>> { 
		enum { value=true }; 
		typedef float_xvector<N> type; 
	};	
	
	template<size_t N> struct is_xvector<typename float_xvector<N>> : std::true_type {};

	template<size_t NA, size_t NB> struct is_xvector_compatible_impl<typename float_xvector<NA>, typename float_xvector<NB>> : std::true_type {};

	// float xvectors

	typedef float_xvector<2> float2x;
	typedef float_xvector<3> float3x;
	typedef float_xvector<4> float4x;		


	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////

	// Structure to encapsulate a scalar float xmvector (all elements/channels are equal)
	// Used by functions that should return a float, but permits to maintain the xmvector as long as possible

	struct float_scalar_xvector 
	{
		typedef float scalar_type;

		xmvector vector;

		operator xmvector () const
		{
			return vector;
		}		

		operator float() const
		{
			return DirectX::XMVectorGetX( vector );
		}

		float_scalar_xvector() {}

		float_scalar_xvector( float v ) : vector( DirectX::XMVectorReplicate(v) ) {}

		float_scalar_xvector( xmvector v ) : vector( v ) {}		

		template<size_t N> float_scalar_xvector( const float_xvector<N> &v ) : vector( v.vector ) {}

		float_scalar_xvector &operator = ( xmvector v )
		{
			vector = v;
			return *this;
		}		

		template<size_t N> float_scalar_xvector &operator = ( const float_xvector<N> &v )
		{
			vector = v.vector;
			return *this;
		}		
	};

	template<> struct is_scalar_xvector_impl<float_scalar_xvector> : std::true_type {};	
	template<> struct get_scalar_impl<float_scalar_xvector> { enum { value=true }; typedef float type; };

} // namespace

#endif