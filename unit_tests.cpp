#include "hlsl/hlsl.hpp"

using namespace hlsl;	

template<typename T1, typename T2> void comparators_checks()
{
	assert( T1(0) < T2(1) );
	assert( T1(1) > T2(0) );
	assert( T1(0) == T2(0) );
	assert( T1(0) == T2(worst_limits<T1,T2>::epsilon()) );		
	assert( T1(0) != T2(worst_limits<T1,T2>::epsilon()*2) );		
}

template<typename T1, typename T2> void comparators_symetrical_checks()
{
	comparators_checks<T1,T2>();
	comparators_checks<T2,T1>();
}

template<typename T, typename S> void operators_check()
{			
	T zero(0), one(1), minus_one(-1), two(2);	

	assert( one != zero );
	assert( -one == minus_one );
	assert( -zero == zero );	

	assert( one + zero == one );
	assert( one + one == two );	
	assert( two - one == one );
	
	assert( zero + S(1) == one );
	assert( S(1) + zero == one );		

	assert( one - zero == one );
	assert( one - one == zero );
	assert( zero - one == minus_one );

	assert( zero - S(1) == minus_one );
	assert( S(1) - zero == one );
	assert( S(2) - one == one );
	
	assert( zero * one == zero );
	assert( one * one == one );
	
	assert( one * S(1) == one );
	assert( one * S(0) == zero );	
	assert( S(1) * one == one );
	assert( S(0) * one == zero );	
	assert( S(1) * zero == zero );				

	assert( one / one == one );
	assert( zero / one == zero );
	assert( two / two == one );
	assert( two / one == two );
	assert( two / S(1) == two );
	assert( S(2) / two == one );

	T result;
	result = one + one;
	assert( result == two );
	result = one - one;
	assert( result == zero );
	result = one * one;
	assert( result == one );
	result = one / one;
	assert( result == one );
}

template<typename T> void operators_check()
{
	operators_check<T,get_element<T>::type>();
	operators_check<T,float>();
	operators_check<T,double>();
	operators_check<T,int>();
}


void unit_tests()
{	
	float3a a(1.0f,0.0f,0.0f), b(0.0f,1.0f,0.0f), a2(2,0.0f,0.0);
	float3 ua(a), ub(b);
	float3x xa(a), xb(b);				

	assert( a == ua );
	assert( ua == a );
	
	assert( a == xa );
	assert( xa == a );

	assert( ua == xa );
	assert( xa == ua );
	
	assert( a != b );
	assert( ua != ub );	
	assert( xa != xb );
	
	assert( a != ub );
	assert( ua != b );

	assert( a != xb );
	assert( xa != b );
	
	assert( ua != xb );
	assert( xa != ub );
	
	assert( a == float3(1,0,0) );
	assert( ua == float3(1,0,0) );	
	assert( xa == float3(1,0,0) );	

	assert( a[0] == ua[0] );
	assert( a[0] != b[0] );
	assert( a[0] != a[1] );
	
	comparators_checks<float2, float2> ();
	comparators_checks<float3, float3> ();
	comparators_checks<float4, float4> ();

	comparators_checks<double2, double2> ();
	comparators_checks<double3, double3> ();
	comparators_checks<double4, double4> ();

	comparators_symetrical_checks<float2,double2> ();
	comparators_symetrical_checks<float3,double3> ();
	comparators_symetrical_checks<float4,double4> ();

	operators_check<float2>();
	operators_check<float2a>();
	operators_check<float3>();
	operators_check<float3a>();
	operators_check<float4>();
	operators_check<float4a>();

	operators_check<float2x>();
	operators_check<float3x>();
	operators_check<float4x>();

	operators_check<double2>();
	operators_check<double3>();
	operators_check<double4>();

	auto c(b);
	b = a;
	b = c;

	ua = a;
	b = ub;
	xa = a;
	b = xb;
	xa = ua;
	ub = xb;	
		
	/////////////////////////////////////////////		
	double3 da(a), db(b), da2(a2);

	// lerp

	assert( lerp( -10.0f, 10.0f, 0.0f ) == -10.0f );
	assert( lerp( -10.0f, 10.0f, 0.5f ) == 0.0f );
	assert( lerp( -10.0f, 10.0f, 1.0f ) == 10.0f );		
	assert( lerp( a, b, 0.5f ) == float3x( 0.5f,0.5f,0 ) );
	assert( lerp( a, b, 0 ) == a );
	assert( lerp( a, b, 1.0 ) == b );

	lerp( ua, b, 0.5 );	
	lerp( a, ub, 0.5 );	
	lerp( ua, ub, 0.5 );	
	lerp( xa, xb, 0.5 );	
	lerp( xa, b, 0.5 );	
	lerp( xa, ub, 0.5 );	
	lerp( a, xb, 0.5 );
	lerp( ua, xb, 0.5 );		

	auto d = da + 1.0;

	//assert( lerp( da, db, 0.5 ) == lerp( a, b, 0.5 ) );
	
	// dot		
	assert( dot(a,a) == 1.0f );
	assert( dot(a,b) == 0 );
	assert( dot(a,-a) == -1.0f );
		
	assert( dot( ua, b ) == 0 );
	assert( dot( a, ub ) == 0 );
	assert( dot( ua, ub ) == 0 );

	assert( dot( xa, xb ) == 0 );
	assert( dot( xa, b ) == 0 );
	assert( dot( ua, xb ) == 0 );
	
	// cross
	assert( cross( a, b ) == float3x(0,0,1.0) );
	cross( a, ub );
	cross( ua, b );
	cross( ua, ub );

	cross( xa, xb );
	cross( a, xb );
	cross( xa, b );
	cross( ua, xb );
	cross( xa, ub );

	// normalize
	assert( normalize( a ) == a );	
	assert( normalize( a2 ) == a );
	normalize( ua );
	normalize( xa );
		
	// length
	assert( length( a ) == 1.0f );
	length( ua );
	length( xa );
	
	assert( length( a2 ) == 2.0f );

	// length_squared
	assert( length_squared( a ) == 1.0f );
	length_squared( ub );
	length_squared( xa );

	assert( length_squared( a2 ) == 4.0f );
	
	// generic math	
	
	assert( a * 2.0 == a2 );
	assert( a *-1.0 == -a );	
	assert( (a+b)*2.0 - 2.0*b == 2.0*a );

	assert( normalize( (a+b)*7.67 ) == normalize(a+b) );

	// type load/store
	//	
	assert( normalize( a.c_array() ) == normalize( a ) );
		
	DirectX::XMFLOAT3A xma( a ); // a to xma 
	float AR[3];
	
	AR << normalize( xma ); // load xma		
	xma << float3x(AR);

	//
	float4a f4(0,1,2,0);
	float3a f3( f4.xyz() );
	assert( normalize( float3x( f4 ) ) == normalize( f3 ) );

	float4a r3;
	r3.xyz() = normalize( float3x( f4 ) );
}
