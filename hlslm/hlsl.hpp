#pragma once
#include "vector_traits.hpp"
#pragma warning(push)
#pragma warning(disable:4996)
#include "xmvector.hpp"
#include "xmswizzler.hpp"
#include "xmoperators.hpp"
#include "xmconstants.hpp"
#include "load.hpp"

// explicit instantiation
namespace DirectX
{
	namespace hlsl
	{
		template struct xmscalar<float>;
		template struct xmvector<float, 2>;
		template struct xmvector<float, 3>;
		template struct xmvector<float, 4>;

		template struct xmscalar<uint>;
		template struct xmvector<uint, 2>;
		template struct xmvector<uint, 3>;
		template struct xmvector<uint, 4>;
	}
}

// namespace for hlsl-math libary
namespace hlsl = DirectX::hlsl;

#pragma warning(pop)