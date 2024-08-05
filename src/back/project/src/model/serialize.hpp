#pragma once

#include <shared/serialize.hpp>
#include <shared/parse/uuid.hpp>
#include <type_traits>

namespace svetit::project::model {

template<typename T>
std::enable_if_t<IsParsable<T>::value, formats::json::Value> Serialize(
	const T& item,
	formats::serialize::To<formats::json::Value> to)
{
	return svetit::Serialize(item, to);
}

template<typename T>
std::enable_if_t<IsParsable<T>::value, T> Parse(
	const formats::json::Value& json,
	formats::parse::To<T> to)
{
	return svetit::Parse(json, to);
}

} // namespace svetit::project::model
