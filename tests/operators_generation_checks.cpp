#include "hlsl/hlsl.hpp"

using namespace hlsl;

void cast_4( float4 &a, const float4 &b )
{
	a = float4x(b);
}

void cast_4a( float4a &a, const float4a &b )
{
	a = float4x(b);
}

void cast_4a_3( float4a &a, const float4a &b )
{
	a = float3x( b );
}


void cast_4_loop( float4 *a, const float4 *b, size_t n )
{
	while( n-- )
		*a++ = float4x(*b++);
}

void cast_4a_loop( float4a *a, const float4a *b, size_t n )
{
	while( n-- )
		*a++ = float4x(*b++);
}

void cast_4a_loop_iterators( const float4a *first, const float4a *last, float4a *result )
{
	while( first != last )
		*result++ = float4x(*first++);
}

void cast_array( float (&ar)[4], const float4a &b )
{
	ar << float4x(b);
}

void cast_std_array( std::array<float,4> &ar, const float4a &b )
{
	ar << float4x(b);
}

void cast_xmfloat4a( DirectX::XMFLOAT4A &ar, const float4a &b )
{
	ar << float4x(b);
}

void add3a_member( float3a &a, const float3a &b )
{
	a.x+=b.x;
	a.y+=b.y;
	a.z+=b.z;
}

void add3_self_operator( float3 &a, const float3 &b )
{
	a += b;
}

void add3a_self_operator( float3a &a, const float3a &b )
{
	a += b;
}

void add3_operator( float3 &a, const float3 &b )
{
	a = a + b;
}

void add3a_operator( float3a &a, const float3a &b )
{
	a = a + b;
}

void add4_self_operator( float4 &a, const float4 &b )
{
	a += b;
}

void add4_operator( float4 &a, const float4 &b )
{
	a = a + b;
}

void add4a_self_operator( float4a &a, const float4a &b )
{
	a = a + b;
}

void add4a_operator( float4a &a, const float4a &b )
{
	a = a + b;
}

void mul3a_self_operator( float3a &a, const float3a &b )
{
	a *= b;
}

void mul3a_operator( float3a &a, const float3a &b )
{
	a = a * b;
}

// bad unrolling expected
void add3a_foreach( float3a &a, const float3a &b )
{
	auto iter = b.begin();

	for( auto &value : a )
		value += *iter++;
}

// good unrolling expected
void add3a_loop( float3a &a, const float3a &b )
{
	for( size_t i = 0 ; i < a.size() ; ++i )	
		a[i] += b[i];	
}

bool less_member( const float3a &a, const float3a &b )
{
	return a.x < (b.x-FLT_EPSILON) && a.y < (b.y-FLT_EPSILON) && a.z < (b.z-FLT_EPSILON);
}

bool less_operator( const float3a &a, const float3a &b )
{	
	return less( a, b );
}

bool less_NR( const float3a &a, const float3a &b )
{
	bool result = a[0] < b[0];
	for( int i = 1 ; i < 3 ; ++i )
		result &= a[i] < b[i];

	return result;
}

bool less_NR_2( const float3a &a, const float3a &b )
{
	bool result = true;
	for( int i = 0 ; i < 3 ; ++i )
		result &= a[i] < b[i];

	return result;
}


