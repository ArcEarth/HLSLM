#include "hlslm/hlsl.hpp"

using namespace hlsl;

float blinn_phong_xm( const float3a &normal, const float3a &view, const float3a &light )
{	
	using namespace DirectX;
	return XMVectorGetX( XMVector3Dot( XMVector3Normalize( XMVectorAdd( XMLoadFloat3A( (const XMFLOAT3A*)&view ), XMLoadFloat3A( (const XMFLOAT3A*)&light ) ) ), XMLoadFloat3A( (const XMFLOAT3A*)&normal ) ) );	
}

float blinn_phong( const float3a &normal, const float3a &view, const float3a &light )
{
	auto h = normalize( view + light);
	return dot( h, normal );
}

void load_add_store_xm( float3a &s, const float3a &l )
{
	using namespace DirectX;
	XMStoreFloat3A( (XMFLOAT3A*)&s, XMVectorAdd( XMLoadFloat3A( (const XMFLOAT3A*)&l ), XMVectorReplicate(0.5f) ) );
}

void load_add_store( float3a &s, const float3a &l )
{
	s = l + 0.5f;
}

