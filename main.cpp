//#include "hlsl/hlsl.hpp"
#include "hlslm\xmvector.hpp"
#include "hlslm\xmvector_operators.hpp"

#include <iostream>

#include <type_traits>

using namespace DirectX::hlsl;

//inline xmvector3f XM_CALLCONV wzyx(xmvector4f xmv)
//{
//	xmvector3f result = swizzle<3, 2, 1>(xmv);
//	return result;
//}

xmvector4f XM_CALLCONV SetX_XM(xmvector4f v, float x)
{
	//float vx = swizzle<1>(v);
	xmvector4f result;
	result.v = DirectX::XMVectorSetX(v.v, x);
	return result;
}

using std::integer_sequence;

template <typename IS1, typename IS2>
struct test;

template <int... a, int... b>
struct test<integer_sequence<int,a...>, integer_sequence<int, b...>>
{
	using type = integer_sequence<int,(a+b)...>;
};

using abtype = typename test<integer_sequence<int, 1, 2>, integer_sequence<int, 2, 3>>::type;

xmvector4f XM_CALLCONV SetX_HL(xmvector4f v, float x)
{
	xmvector2f v2;
	xmscalar<float> vs;

	float kf = v2.x();

	v2 = v2.swizzle<0,1>().swizzle<1,0>();
	v2 = v.xy().yx();
	v2 = v2 + vs;
	v2 = v2 + v.xy();
	v.xy() = v2 - v.yw();

	xmvector4f result;
	using cacate_t = decltype(result.xywz().wz());

	cacate_t vp;
	xmscalar<float> xv(x);
	result.swizzle<3,2,1,0>() = v.swizzle<1,0>().swizzle<0, 0, 1, 1>();
	result.xz() = v.xy();
	//using mask_seq = typename detail::sequence_to_mask<index_sequence<0,1>>::type;
	//result.v = detail::swizzle_assign(v.swizzle<0>(), xv.swizzle<0>());
	return result;

	//using rstSwz = typename indirect_assign<index_sequence<0, 1, 2, 3>, index_sequence<0, 1>, index_sequence<0, 0>>::type;
	//using sorted = typename sort_sequence<index_sequence<3*4+1, 1*4+2, 2*4+0, 0*4+3>>::type;
}

int __cdecl main( int argc, char *argv[] )
{
	DirectX::XMFLOAT4A f4, f3;
	xmvector4f xmv;
	xmvector4f ret0,ret1;
	xmv.v = DirectX::XMVectorSet(1, 2, 3, 4);

	ret0 = SetX_HL(xmv, 5.0f);

	DirectX::XMStoreFloat4A(&f4, ret0.v);

	std::cout << f4.x << ',' << f4.y << ',' << f4.z << ',' << f4.w << std::endl;

	ret1 = SetX_XM(xmv, 5.0f);

	DirectX::XMStoreFloat4A(&f3, ret1.v);

	std::cout << f3.x << ',' << f3.y << ',' << f3.z << ',' << f3.w << std::endl;
	//xmvector3f result = wzyx(xmv);

	//xmvector2f sar = swizzle_assign(make_swizzler<2, 1>(result), make_swizzler<2, 1>(xmv));

	//xmvector4i xmvi(1);
	//result = set_z(result, 5.0f);

	//DirectX::XMFLOAT4A f4;
	//DirectX::XMStoreFloat4A(&f4, result.v);

	char ch;
	std::cin >> ch;

	//unit_tests();	

	//// breakpoint here to follow the "How it Works" Codeplex documentation
	//float3a n,v,l;
	//blinn_phong( n,v,l );
}


