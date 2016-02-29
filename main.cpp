//#include "hlsl/hlsl.hpp"

#include "hlslm\hlsl.hpp"
//#include "hlslm\load_store.hpp"

#include <iostream>

#include <type_traits>

using namespace DirectX::hlsl;

//inline xmvector3f XM_CALLCONV wzyx(xmvector4f xmv)
//{
//	xmvector3f result = swizzle<3, 2, 1>(xmv);
//	return result;
//}

struct tag
{};

struct PropertyTest
{
	int m_k;

	const int& get_k() const { return m_k; }
	int& get_k() { return m_k; }

	void put_k(int value) { m_k = value; }
	void put_k(int* value) { m_k = reinterpret_cast<int>(value); }

	__declspec(property(get = get_k, put = put_k)) int k;
	//__declspec(property(get = get_k, put = put_k)) const int& k;
};

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

inline xmvector3f XM_CALLCONV cross(xmvector3f a, xmvector3f b)
{
	return xmvector3f(DirectX::XMVector3Cross(a.v, b.v));
}

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

void OperatorTest()
{
	booo b;
	b.m_k = 313;
	int r = b.k;
	using namespace std;
	cout << "r = b.k = " << r << endl;
	b.k = 317.0f;

	foo<sqr>::excute(3);
	foo<negate>::excute(3);

	xmvector2f v2;
	xmvector3f v3;
	xmvector4f v4;
	xmscalar<float> v0;
	float		scl;

	v2 = v2 + v2;
	v2 = v2 + v0;
	v2 = v2 + v4.xy();
	//v2 = v2 + scl; // error

	v2 = v0 + scl;
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
	v0 += scl;
	//v0 += v2; //error
}

xmvector4f XM_CALLCONV SetX_HL(xmvector4f v, float x)
{
	xmvector2f v2;
	xmscalar<float> vs;

	float f3arry[3];

	xmvector3f v3(f3arry);
	v3 = f3arry;
	DirectX::XMFLOAT3 f3;

	v3.store(f3arry);
	v3.zyx() = f3;

	using traits = memery_vector_traits<DirectX::XMFLOAT3>;
	
	v3 = v3 + f3;

	//v3 = f3 + f3arry;

	//using test_t = decltype(v.xy().yx().xy().yx());

	xmvector3f kf = cross(f3, DirectX::g_XMIdentityR0);

	v2 = v2 + v2;
	v2 = v2.swizzle<0,1>().swizzle<1,0>();
	v2 = v.xy().yx();
	v2 = v2 + vs;
	v2 = v2 + v.xy();
	v.xy() = v2 + v.zw();
	v2.yx() = v2.yx();

	v.xy() = v2.yx() + v.zw();

	auto z = zero();

	v2 = v2 + z;
	v2 = v2 + z;

	//auto veq = v2 == v2;
	//auto vgt = v2 > v2;
	//nor(veq, vgt);

	xmvector4f result;

	//cacate_t vp;
	xmscalar<float> xv(x);
	result.swizzle<3,2,1,0>() = v.swizzle<1,0>().swizzle<0, 0, 1, 1>();
	result.yz() = v.xy();
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

	ret0.store(f4);

	std::cout << f4.x << ',' << f4.y << ',' << f4.z << ',' << f4.w << std::endl;

	ret1 = SetX_XM(xmv, 5.0f);

	DirectX::XMStoreFloat4A(&f3, ret1.v);

	std::cout << f3.x << ',' << f3.y << ',' << f3.z << ',' << f3.w << std::endl;
	//xmvector3f result = wzyx(xmv);

	PropertyTest p;
	auto& s = p.k;

	const auto& cp = p;

	auto& cs = cp.k;
	//xmvector2f sar = swizzle_assign(make_swizzler<2, 1>(result), make_swizzler<2, 1>(xmv));

	//xmvector4i xmvi(1);
	//result = set_z(result, 5.0f);

	//DirectX::XMFLOAT4A f4;
	//DirectX::XMStoreFloat4A(&f4, result.v);

	OperatorTest();

	char ch;
	std::cin >> ch;

	//unit_tests();	

	//// breakpoint here to follow the "How it Works" Codeplex documentation
	//float3a n,v,l;
	//blinn_phong( n,v,l );
}


