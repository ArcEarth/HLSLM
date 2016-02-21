#include "hlsl/hlsl.hpp"

using namespace hlsl;
using namespace std;

template<typename T, size_t N> struct offsets_static_asserts {};

template<typename T> struct offsets_static_asserts<T,2>
{
	offsets_static_asserts()
	{		
		static_assert( offsetof(T,x) == 0, "x offset mismatch" );
		static_assert( offsetof(T,y) == sizeof(get_element<T>::type), "y offset mismatch" );
	}
};

template<typename T> struct offsets_static_asserts<T,3> : offsets_static_asserts<T,2>
{
	offsets_static_asserts()
	{			
		static_assert( offsetof(T,z) == 2*sizeof(get_element<T>::type), "z offset mismatch" );
	}
};

template<typename T> struct offsets_static_asserts<T,4> : offsets_static_asserts<T,3>
{
	offsets_static_asserts()
	{			
		static_assert( offsetof(T,w) == 3*sizeof(get_element<T>::type), "w offset mismatch" );
	}
};

template<typename T, typename E, size_t N, bool A> void type_static_asserts()
{	
	static_assert( is_vector<T>::value, "is_vector<>" );
	static_assert( is_vector<const T>::value, "is_vector<const>" );			

	static_assert( get_dimensions<T>::value == N, "get_dimensions<>" );
	static_assert( get_dimensions<const T>::value == N, "get_dimensions<const>" );	

	static_assert( is_same<get_element<T>::type,E>::value, "get_element<>" );
	static_assert( is_same<get_element<const T>::type,E>::value, "get_element<const>" );

	static_assert( is_aligned<T>::value == A, "is_aligned<>" );
	static_assert( is_aligned<const T>::value == A, "is_aligned<const>" );

	static_assert( sizeof(get_element<T>::type) * get_dimensions<T>::value == sizeof(T), "size mismatch" );	
}

template<typename T, typename E, size_t N, bool A> void type_and_offsets_static_asserts()
{
	type_static_asserts<T,E,N,A>();
	offsets_static_asserts<T,N>();
}

template<typename T> struct two_elements_struct
{
	T fist, second;	
};

template<typename T, typename U> void get_unaligned_static_asserts()
{
	static_assert( is_same<get_unaligned<T>::type,U>::value, "get_unaligned<>" );
	static_assert( is_same<get_unaligned<const T>::type,U>::value, "get_unaligned<const>" );

	static_assert( is_same<get_element<T>::type, get_element<U>::type>::value, "get_element T vs U " );
	static_assert( get_dimensions<T>::value == get_dimensions<U>::value, "get_dimensions T vs U" );
	static_assert( is_aligned<T>::value && !is_aligned<U>::value, "is_aligned T vs U" );	
	static_assert( !is_aligned<get_unaligned<T>::type>::value, "is_aligned get_unaligned" );

	static_assert( offsetof( two_elements_struct<T>, second )%16 == 0, "alignment" );
	static_assert( __alignof(T) == 16, "alignment" );
}


template<typename T, typename X> void get_xvector_static_asserts()
{	
	static_assert( std::is_same<get_xvector<T>::type,X>::value, "get_xvector" );
	static_assert( std::is_same<get_xvector<const T>::type,X>::value, "get_xvector" );
	static_assert( is_xvector_of<X,T>::value, "is_xvector_of" );
	static_assert( is_xvector_of<X,const T>::value, "is_xvector_of" );
	static_assert( !is_xvector_of<T,X>::value, "is_xvector_of" );
	static_assert( !is_xvector_of<void,T>::value, "is_xvector_of" );
}

void traits_static_asserts()
{
	static_assert( get_dimensions<void>::value == 0, "get_dimensions" );
	static_assert( !get_element<void>::value, "get_element" );
	static_assert( !is_vector<void>::value, "is_vector" );
	static_assert( !is_aligned<void>::value, "is_aligned" );
	static_assert( !get_unaligned<void>::value, "get_unaligned" );
	static_assert( std::is_same<get_unaligned<void>::type,void>::value, "get_unaligned" );
	
	/// floatX
	type_and_offsets_static_asserts<float2,float,2,false>();
	type_and_offsets_static_asserts<float2a,float,2,true>();
	type_and_offsets_static_asserts<float3,float,3,false>();
	type_and_offsets_static_asserts<float3a,float,3,true>();
	type_and_offsets_static_asserts<float4,float,4,false>();
	type_and_offsets_static_asserts<float4a,float,4,true>();
		
	get_unaligned_static_asserts<float2a,float2>();
	get_unaligned_static_asserts<float3a,float3>();
	get_unaligned_static_asserts<float4a,float4>();

	get_xvector_static_asserts<float2,float2x>();
	get_xvector_static_asserts<float2a,float2x>();
	
	get_xvector_static_asserts<float3,float3x>();
	get_xvector_static_asserts<float3a,float3x>();

	get_xvector_static_asserts<float4,float4x>();
	get_xvector_static_asserts<float4a,float4x>();	

	/// doubleX
	type_and_offsets_static_asserts<double2,double,2,false>();
	type_and_offsets_static_asserts<double3,double,3,false>();
	type_and_offsets_static_asserts<double4,double,4,false>();

	/// arrays
	type_static_asserts<float[2],float,2,false>();
	type_static_asserts<float[3],float,3,false>();
	type_static_asserts<float[4],float,4,false>();
	type_static_asserts<float[8],float,8,false>();

	get_xvector_static_asserts<float[2],float2x>();
	get_xvector_static_asserts<float[3],float3x>();
	get_xvector_static_asserts<float[4],float4x>();

	/// std::array
	type_static_asserts<std::array<float,2>,float,2,false>();
	type_static_asserts<std::array<float,3>,float,3,false>();
	type_static_asserts<std::array<float,4>,float,4,false>();
	type_static_asserts<std::array<float,8>,float,8,false>();

	get_xvector_static_asserts<std::array<float,2>,float2x>();
	get_xvector_static_asserts<std::array<float,3>,float3x>();
	get_xvector_static_asserts<std::array<float,4>,float4x>();
}
