#pragma once

#include <svetit/paging.hpp>

#include <userver/formats/json/value.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/utest/using_namespace_userver.hpp>

namespace svetit {

template<typename T>
formats::json::Value Serialize(
	const PagingResult<T>& item,
	formats::serialize::To<formats::json::Value>)
{
	formats::json::ValueBuilder builder{};
	builder["list"] = item.items;
	builder["total"] = item.total;

	return builder.ExtractValue();
}

} // namespace svetit
