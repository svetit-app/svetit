#pragma once

template <typename>
struct ReturnType;

template <typename R, typename C, typename... Args>
struct ReturnType<R(C::*)(Args...)> { using type = R; };

template <typename T>
using ReturnTypeT = typename ReturnType<T>::type;
