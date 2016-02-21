#include "hlsl/hlsl.hpp"

using namespace hlsl;

void unit_tests();

float blinn_phong( const float3a &normal, const float3a &view, const float3a &light );

int main( int argc, char *argv[] )
{
	unit_tests();	

	// breakpoint here to follow the "How it Works" Codeplex documentation
	float3a n,v,l;
	blinn_phong( n,v,l );
}


