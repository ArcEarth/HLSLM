#ifndef HLSL_FLOAT_MATH_HPP
#define HLSL_FLOAT_MATH_HPP

//  Copyright (c) 2012 Patrick Grison
//  Distributed under the MIT Software License

#pragma once

#include "float_xvector.hpp"

namespace hlsl
{
	// all dimensions (always 4 components)
	template<typename T> struct all_float_dimensions_math
	{
		static T abs( const T &vector )
		{
			return DirectX::XMVectorAbs( vector );
		}

		static T floor( const T &vector )
		{
			return DirectX::XMVectorFloor( vector );
		}

		static T ceil( const T &vector )
		{
			return DirectX::XMVectorCeiling( vector );
		}

		static T min( const T &first, const T &second )
		{
			return DirectX::XMVectorMin( first, second );
		}

		static T max( const T &first, const T &second )
		{
			return DirectX::XMVectorMax( first, second );
		}

		static T dst( const T &first, const T &second )
		{
			return DirectX::XMVectorSubtract( first, second );
		}

		static T rcp( const T &vector )
		{
			return DirectX::XMVectorReciprocal( vector );
		}

		static T rsqrt( const T &vector )
		{
			return DirectX::XMVectorReciprocalSqrt( vector );
		}

		static T lerp( const T &first, const T &second, float s )
		{			
			return DirectX::XMVectorLerp( first, second, s );
		}	

		static T fmod( const T &first, const T &second )
		{
			return DirectX::XMVectorMod( first, second );
		}

		static T pow( const T &first, const T &second )
		{
			return DirectX::XMVectorPow( first, second );
		}

		static T exp( const T &vector )
		{
			return DirectX::XMVectorExp( vector );			
		}

		static T exp2( const T &vector )
		{
			return DirectX::XMVectorPow( DirectX::g_XMTwo, vector );			
		}

		static T log( const T &vector )
		{
			return DirectX::XMVectorLog( vector );
		}		

		static T clamp( const T &vector, const T &Min, const T &Max )
		{
			return DirectX::XMVectorClamp( vector, Min, Max );
		}

		static T saturate( const T &vector )
		{
			return DirectX::XMVectorSaturate( vector );
		}

		static T trunc( const T &vector )
		{
			return DirectX::XMVectorTruncate( vector );
		}

		static T round( const T &vector )
		{
			return DirectX::XMVectorRound( vector );
		}

		static T cos( const T &x )
		{
			return DirectX::XMVectorCos( x );
		}

		static T cosh( const T &x )
		{
			return DirectX::XMVectorCosH( x );
		}

		static T sin( const T &x )
		{
			return DirectX::XMVectorSin( x );
		}

		static T sinh( const T &x )
		{
			return DirectX::XMVectorSinH( x );
		}

		static T tan( const T &x )
		{
			return DirectX::XMVectorTan( x );
		}

		static T tanh( const T &x )
		{
			return DirectX::XMVectorTanH( x );
		}

		static T atan( const T &x )
		{
			return DirectX::XMVectorATan( x );
		}

		static T atan2( const T &y, const T &x )
		{
			return DirectX::XMVectorATan2( x );
		}

		static void sincos( const T &x, T &s, T &c )
		{
			DirectX::XMVectorSinCos( &((xmvector&)s), &((xmvector&)c), x );
		}					
	};

	// float2
	struct float2x_math : all_float_dimensions_math<float2x>
	{
		typedef float2x vector_type;
		typedef float_scalar_xvector scalar_type;
		typedef bool bool_type;

		static scalar_type dot( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector2Dot( first, second );		
		}

		static vector_type cross( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector2Cross( first, second );			
		}		

		static vector_type reflect( const vector_type &i, const vector_type &n )
		{
			return DirectX::XMVector2Reflect( i, n );
		}

		static vector_type refract( const vector_type &i, const vector_type &n, const scalar_type &r )
		{
			return DirectX::XMVector2RefractV( i, n, r );
		}

		static scalar_type length( const vector_type &vector )
		{				
			return DirectX::XMVector2Length( vector );			
		}

		static scalar_type length_squared( const vector_type &vector )
		{				
			return DirectX::XMVector2LengthSq( vector );
		}

		static scalar_type distance( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector2Length( DirectX::XMVectorSubtract(first, second) );
		}

		static const vector_type normalize( const vector_type &vector )
		{
			return DirectX::XMVector2Normalize( vector );
		}

		static bool equal( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector2Equal( first, second );
		}

		static bool not_equal( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector2NotEqual( first, second );
		}

		static bool near_equal( const vector_type &first, const vector_type &second, xmvector epsilon )
		{
			return DirectX::XMVector2NearEqual( first, second, epsilon );
		}		

		static bool less( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector2Less( first, second );			
		}		

		static bool less_or_equal( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector2LessOrEqual( first, second );			
		}		

		static bool greater( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector2Greater( first, second );			
		}		

		static bool greater_or_equal( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector2GreaterOrEqual( first, second );			
		}		

		static bool in_bounds( const vector_type &vector, const vector_type &bounds )
		{
			return DirectX::XMVector2InBounds( vector, bounds );
		}

		static bool isinf( const vector_type &vector )
		{
			return DirectX::XMVector2IsInfinite( vector );
		}

		static bool isnan( const vector_type &vector )
		{
			return DirectX::XMVector2IsNaN( vector );
		}
	};

	struct float3x_math : all_float_dimensions_math<float3x>
	{
		typedef float3x	vector_type;
		typedef float_scalar_xvector scalar_type;
		typedef bool bool_type;

		static scalar_type dot( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector3Dot( first, second );					
		}

		static vector_type cross( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector3Cross( first, second );
		}

		static vector_type reflect( const vector_type &i, const vector_type &n )
		{
			return DirectX::XMVector3Reflect( i, n );
		}

		static vector_type refract( const vector_type &i, const vector_type &n, const scalar_type &r )
		{
			return DirectX::XMVector3RefractV( i, n, r );
		}

		static scalar_type length( const vector_type &vector )
		{				
			return DirectX::XMVector3Length( vector );			
		}

		static scalar_type length_squared( const vector_type &vector )
		{				
			return DirectX::XMVector3LengthSq( vector );			
		}

		static scalar_type distance( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector3Length( DirectX::XMVectorSubtract(first, second) );
		}

		static const vector_type normalize( const vector_type &vector )
		{
			return DirectX::XMVector3Normalize( vector );
		}

		static bool equal( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector3Equal( first, second );
		}

		static bool not_equal( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector3NotEqual( first, second );
		}

		static bool near_equal( const vector_type &first, const vector_type &second, xmvector epsilon )
		{
			return DirectX::XMVector3NearEqual( first, second, epsilon );
		}		

		static bool less( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector3Less( first, second );			
		}		

		static bool less_or_equal( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector3LessOrEqual( first, second );			
		}		

		static bool greater( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector3Greater( first, second );			
		}		

		static bool greater_or_equal( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector3GreaterOrEqual( first, second );			
		}		

		static bool in_bounds( const vector_type &vector, const vector_type &bounds )
		{
			return DirectX::XMVector3InBounds( vector, bounds );
		}		

		static bool isinf( const vector_type &vector )
		{
			return DirectX::XMVector3IsInfinite( vector );
		}

		static bool isnan( const vector_type &vector )
		{
			return DirectX::XMVector3IsNaN( vector );
		}
	};

	struct float4x_math : all_float_dimensions_math<float4x>
	{
		typedef float4x vector_type;
		typedef float_scalar_xvector scalar_type;
		typedef bool bool_type;

		static scalar_type dot( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector4Dot( first, second );		
		}

		static vector_type cross( const vector_type &first, const vector_type &second, const vector_type &third )
		{
			return DirectX::XMVector4Cross( first, second, third );
		}

		static vector_type reflect( const vector_type &i, const vector_type &n )
		{
			return DirectX::XMVector4Reflect( i, n );
		}

		static vector_type refract( const vector_type &i, const vector_type &n, const scalar_type &r )
		{
			return DirectX::XMVector4RefractV( i, n, r );
		}

		static scalar_type length( const vector_type &vector )
		{				
			return DirectX::XMVector4Length( vector );			
		}

		static scalar_type length_squared( const vector_type &vector )
		{				
			return DirectX::XMVector4LengthSq( vector );			
		}

		static scalar_type distance( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector4Length( DirectX::XMVectorSubtract(first, second) );
		}

		static const vector_type normalize( const vector_type &vector )
		{
			return DirectX::XMVector4Normalize( vector );
		}

		static bool equal( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector4Equal( first, second );
		}

		static bool not_equal( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector4NotEqual( first, second );
		}

		static bool near_equal( const vector_type &first, const vector_type &second, xmvector epsilon )
		{
			return DirectX::XMVector4NearEqual( first, second, epsilon );
		}		

		static bool less( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector4Less( first, second );			
		}		

		static bool less_or_equal( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector4LessOrEqual( first, second );			
		}		

		static bool greater( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector4Greater( first, second );			
		}		

		static bool greater_or_equal( const vector_type &first, const vector_type &second )
		{
			return DirectX::XMVector4GreaterOrEqual( first, second );			
		}		

		static bool in_bounds( const vector_type &vector, const vector_type &bounds )
		{
			return DirectX::XMVector4InBounds( vector, bounds );
		}		

		static bool isinf( const vector_type &vector )
		{
			return DirectX::XMVector4IsInfinite( vector );
		}

		static bool isnan( const vector_type &vector )
		{
			return DirectX::XMVector4IsNaN( vector );
		}
	};
} // namespace 

#endif