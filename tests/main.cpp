//#include "hlsl/hlsl.hpp"

#include <hlslm\hlsl.hpp>
//#include "hlslm\load_store.hpp"

#include <iostream>

#include <type_traits>

using namespace hlsl;

//inline xmvector3f XM_CALLCONV wzyx(xmvector4f xmv)
//{
//	xmvector3f result = swizzle<3, 2, 1>(xmv);
//	return result;
//}

static constexpr size_t szVector4 = sizeof(xmvector4f);
static constexpr size_t szVector3 = sizeof(xmvector3f);
static constexpr size_t szVector2 = sizeof(xmvector2f);


std::ostream& XM_CALLCONV operator << (std::ostream& os, DirectX::FXMVECTOR v)
{
	os << '(' << (float)v.m128_f32[0] << ')';
	return os;
}

std::ostream& XM_CALLCONV operator << (std::ostream& os, const xmvector2f v)
{
	os << '(' << (float)v.x() << ',' << (float)v.y() << ')';
	return os;
}

std::ostream& XM_CALLCONV operator << (std::ostream& os, const xmvector3f v)
{
	os << '(' << (float)v.x() << ',' << (float)v.y() << ',' << (float)v.z() << ')';
	return os;
}

std::ostream& XM_CALLCONV operator << (std::ostream& os, const xmvector4f v)
{
	os << '(' << (float)v.x() << ',' << (float)v.y() << ',' << (float)v.z() << ',' << (float)v.w() << ')';
	return os;
}


//struct tag
//{};
//
//struct PropertyTest
//{
//	int m_k;
//
//	const int& get_k() const { return m_k; }
//	int& get_k() { return m_k; }
//
//	void put_k(int value) { m_k = value; }
//	void put_k(int* value) { m_k = reinterpret_cast<intptr_t>(value); }
//
//	__declspec(property(get = get_k, put = put_k)) int k;
//	//__declspec(property(get = get_k, put = put_k)) const int& k;
//};
//

//#include <xmmintrin.h>
//struct empty_struct
//{
//};
//
//struct vector_struct
//{
//	__m128 mts0;
//};
//
//struct hva2 : public empty_struct
//{
//	__m128 array[2];
//};    // 2 element HVA type on __m128, inherit an empty struct
//
//struct hva3 : public vector_struct
//{
//	__m128 array[2];
//};    // 3 element HVA type on __m128
//
//
//__m128 __vectorcall test2(const hva2 v) // error C2719: 'v': formal parameter with requested alignment of 16 won't be aligned
//{
//	return v.array[0];
//}
//
//__m128 __vectorcall test3(const hva3 v) // ok
//{
//	return v.array[0];
//}


inline xmvector3f XM_CALLCONV cross(xmvector3f a, xmvector3f b)
{
	return xmvector3f(DirectX::XMVector3Cross(a.v, b.v));
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

inline int negate(int x)
{
	return -x;
}

int sqr(int x)
{
	return x*x;
}

template <typename T, typename Class, T(Class::*getter)(void) const, void(Class::*setter)(T) = nullptr>
struct Property
{
	inline operator T(void) const
	{
		return (reinterpret_cast<const Class*>(this)->*getter)();
	}

	Property& operator= (const Property&) = delete;
	Property& operator= (Property&&) = delete;

	void operator= (T value)
	{
		static_assert(setter != nullptr, "This property is read only");
		(reinterpret_cast<Class*>(this)->*setter)(value);
	}
};

struct empty_tag
{

};

struct booo
{
	int get_k() const;
	void set_k(int val);

	int m_k;
	Property<int,booo, &booo::get_k, &booo::set_k> k;

	static constexpr size_t sizek = sizeof(decltype(k));
	//static constexpr size_t sizetag = sizeof(decltype(tag));
	static constexpr size_t sizemk = sizeof(decltype(m_k));
};

static constexpr size_t offk = offsetof(booo, k);
//static constexpr size_t offtag = offsetof(booo, tag);
static constexpr size_t offmk = offsetof(booo, m_k);

inline int booo::get_k() const { return m_k; }

void booo::set_k(int val)
{
	m_k = val;
}

template <int(*pf)(int)>
struct foo
{
	static void excute(int a)
	{
		pf(a);
	}
};

static constexpr size_t get_dimension(size_t a, size_t b)
{
	return
		a == b ? a :
		a == 1 ? b :
		b == 1 ? a :
		0;
};

static constexpr size_t sss = get_dimension(1, 5);

void OperatorTest()
{
	booo b;
	b.m_k = 313;
	int r = b.k;
	using namespace std;
	cout << "r = b.k = " << r << endl;

	foo<sqr>::excute(3);
	foo<negate>::excute(3);

	xmvector2f v2 = {.0f, 1.0f};
	xmvector3f v3 = {-1.0f, -1.0f, -1.0f};
	xmvector4f v4;
	xmscalar<float> v0;
	float		scl = 2.0f;
	xmvector4i vi4(1);
	xmfloat		xscl = 1.0f;
	DirectX::XMFLOAT3 f3;
	v3 = v3 * f3;


	static constexpr bool is_over = traits::binary_operator_traits<xmvector2f, float>::overload;
	v3.xy() = min(v2,1.0f);
	//xmfloat rv3 = rcp(1.0f); // error
	xmfloat rv3 = rcp(xmfloat(1.0f));
	rv3 += rv3;

	cout << v3;

	uint iarray[4] = {1,2,3,4};
	vi4 /= vi4;
	vi4 = xor(vi4, vi4.wzyx());

	vi4 = vi4 ^ iarray;
	vi4 = vi4 & vi4.yxwz();
	vi4 ^= xmuint(5);
	vi4.yxz() ^= v4.cast<uint>().zyx();

	v2 = v2 + v2;
	v2 = v2 + v0;
	v2 = v2 + v4.xy();
	v2 = v2 * 0.5f;
	//v2 = v2 + scl; // error

	v2 = v0 + load(scl);
	v2 = v0 + v2;
	v2 += v2;
	v2 += v4.xy();
	v2 += v0;
	//v2 += v3;// error

	//v2 = scl + v2; // error

	v2 = v4.xy() + v2;
	//v2 = v4.xy() + v3;// error
	v2 = v4.xy() + v0;
	//v2 = v4.xy() + scl;// error
	v2 = v4.xy() + v3.yz();
	v2 = (v4.xyz() + v3).yz();

	//v4.xy() += v3; // error
	v4.xy() += v3.xy();
	v4.xy() += v3.yz();
	v4.xy() += v2;
	v4.xy() += v0;
	//v4.xy() += scl; // error

	v0 += v0;
	v0 += load(scl);
	//v0 += v2; //error
}

xmvector4f XM_CALLCONV SetX_HL(xmvector4f v, float x)
{
	//using namespace std;
	//cout << "v = " << v << endl;
	v.yx() += v.zw();
	//cout << "v.xy() += v.zw(); v ==" << v << endl;
	v.yz() = v.yz() + v.yx();
	//cout << "v.xy() = v.wz() + v.zw(); v ==" << v << endl;
	return v;
	
	//using rstSwz = typename indirect_assign<index_sequence<0, 1, 2, 3>, index_sequence<0, 1>, index_sequence<0, 0>>::type;
	//using sorted = typename sort_sequence<index_sequence<3*4+1, 1*4+2, 2*4+0, 0*4+3>>::type;
}

int __cdecl main( int argc, char *argv[] )
{
	DirectX::XMFLOAT4A f4;
	DirectX::XMFLOAT3A f3;
	xmvector4f xmv;
	xmvector4f ret0, ret1;

	xmvector3f v3;
	v3 = { 1.0f, 2.0f, 3.0f };
	v3 = { 1.0f, 2.0f, 3.0f, 0.0f };
	xmv = { 100.f,200.f,300.f,400.f };

	float il3[] = { 1.0f, 2.0f, 3.0f };
	xmv.xyz() = v3 + il3;
	ret0 = SetX_HL(xmv, 5.0f);
	OperatorTest();

	system("PAUSE");

	ret0 *= 2.0f;
	f4 << ret0;
	ret0 = f4;
	// f3 << ret0; error no overload
	f3 << ret0.xyz();
	
	//store(f4, ret0);

	ret0.store(f4);

	//std::cout << f4.x << ',' << f4.y << ',' << f4.z << ',' << f4.w << std::endl;

	//ret1 = SetX_XM(xmv, 5.0f);

	//DirectX::XMStoreFloat4A(&f3, ret1.v);

	//std::cout << f3.x << ',' << f3.y << ',' << f3.z << ',' << f3.w << std::endl;
	//xmvector3f result = wzyx(xmv);

	//PropertyTest p;
	//auto& s = p.k;

	//const auto& cp = p;

	//auto& cs = cp.k;
	//xmvector2f sar = swizzle_assign(make_swizzler<2, 1>(result), make_swizzler<2, 1>(xmv));

	//xmvector4i xmvi(1);
	//result = set_z(result, 5.0f);

	//DirectX::XMFLOAT4A f4;
	//DirectX::XMStoreFloat4A(&f4, result.v);

	//char ch;
	//std::cin >> ch;

	//unit_tests();	

	//// breakpoint here to follow the "How it Works" Codeplex documentation
	//float3a n,v,l;
	//blinn_phong( n,v,l );
}


