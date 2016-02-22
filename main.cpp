//#include "hlsl/hlsl.hpp"
#include "hlslm\xmvector.hpp"
#include <iostream>

using namespace DirectX::hlsl;


inline xmvector3f XM_CALLCONV wzyx(xmvector4f xmv)
{
	xmvector3f result = make_swizzler<1, 1, 1>(make_swizzler<3, 2, 1, 0>(xmv));
	return result;
}

int main( int argc, char *argv[] )
{
	xmvector4f xmv;
	xmv.v = DirectX::XMVectorSet(1, 2, 3, 4);

	xmvector3f result = wzyx(xmv);

	DirectX::XMFLOAT4A f4;
	DirectX::XMStoreFloat4A(&f4, result.v);

	std::cout << f4.x << ',' << f4.y << ',' << f4.z << ',' << f4.w << std::endl;
	char ch;
	std::cin >> ch;

	//unit_tests();	

	//// breakpoint here to follow the "How it Works" Codeplex documentation
	//float3a n,v,l;
	//blinn_phong( n,v,l );
}


