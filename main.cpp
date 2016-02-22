//#include "hlsl/hlsl.hpp"
#include "hlslm\xmvector.hpp"
#include <iostream>

#include <type_traits>

using namespace DirectX::hlsl;

using swizzler = xmvector_swizzler<float, 1, 2>;

swizzler sv;

inline xmvector3f XM_CALLCONV wzyx(xmvector4f xmv)
{
	xmvector3f result = swizzle<3, 2, 1>(xmv);
	return result;
}

xmvector4f XM_CALLCONV SetX_XM(xmvector4f v, float x)
{
	xmvector4f result;
	result.v = DirectX::XMVectorSetX(v.v, x);
	return result;
}

xmvector4f XM_CALLCONV SetX_HL(xmvector4f v, float x)
{
	//xmvector2f result;
	//xmscalar<float> xv(x);

	//using mask_seq = typename detail::sequence_to_mask<index_sequence<0,1>>::type;
	//result = swizzle_assign(swizzle<0,1>(result), swizzle<0,2>(xv));
	//return result.as<4>();
}

int __cdecl main( int argc, char *argv[] )
{
	xmvector4f xmv;
	xmv.v = DirectX::XMVectorSet(1, 2, 3, 4);

	xmvector4f ret0,ret1;

	ret0 = SetX_HL(xmv, 5.0f);

	ret1 = SetX_XM(xmv, 5.0f);

	DirectX::XMFLOAT4A f4,f3;
	DirectX::XMStoreFloat4A(&f4, ret0.v);
	DirectX::XMStoreFloat4A(&f3, ret1.v);

	//xmvector3f result = wzyx(xmv);

	//xmvector2f sar = swizzle_assign(make_swizzler<2, 1>(result), make_swizzler<2, 1>(xmv));

	//xmvector4i xmvi(1);
	//result = set_z(result, 5.0f);

	//DirectX::XMFLOAT4A f4;
	//DirectX::XMStoreFloat4A(&f4, result.v);

	std::cout << f4.x << ',' << f4.y << ',' << f4.z << ',' << f4.w << std::endl;
	std::cout << f3.x << ',' << f3.y << ',' << f3.z << ',' << f3.w << std::endl;
	//char ch;
	//std::cin >> ch;

	//unit_tests();	

	//// breakpoint here to follow the "How it Works" Codeplex documentation
	//float3a n,v,l;
	//blinn_phong( n,v,l );
}


