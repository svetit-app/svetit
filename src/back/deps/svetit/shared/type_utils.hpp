#pragma once

#include <boost/type_traits/function_traits.hpp>
#include <tuple>
#include <type_traits>

template <typename T>
struct FunctionTraits
	: public boost::function_traits<decltype(&T::operator())>
{};

template <typename C, typename R, typename... Args>
struct FunctionTraits<R(C::*)(Args...)>
{
	typedef R result_type;

	template <size_t i>
	struct arg
	{
		typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
	};
};

template <typename T>
using ReturnTypeT = typename FunctionTraits<T>::result_type;

template <typename T, size_t i = 0>
using FuncArgT = std::remove_cvref_t<typename FunctionTraits<T>::template arg<i>::type>;

template <typename T>
class HasGetByKey
{
	template <typename C> static char test( decltype(&C::GetByKey) ) ;
	template <typename C> static int test(...);
public:
	enum { value = sizeof(test<T>(0)) == sizeof(char) };
};
