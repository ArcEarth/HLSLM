#include <type_traits>
#include <utility>

namespace std
{
 template<class...> struct conjunction : std::true_type { };
 template<class B1> struct conjunction<B1> : B1 { };
 template<class B1, class... Bn>
 struct conjunction<B1, Bn...> : std::conditional_t<B1::value != false, conjunction<Bn...>, B1> {};
}

template <size_t... _Args>
struct test
{
 static_assert(std::conjunction<std::bool_constant<(_Args<4)>...>::value, "bad thing!");
};

template <size_t... _Args>
struct test2
{
 static_assert(std::conjunction<std::integral_constant<bool,(_Args<4)>...>::value, "bad thing!");
};

test2<0, 1> b; // OK
test<0, 1> a; // Failed: 'std::integral_constant': template parameter '_Val': '_Val': a variable with non-static storage duration cannot be used as a non-type argument
