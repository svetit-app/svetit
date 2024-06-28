#pragma once

#include "gettypeindexbyidindex.hpp"

#include <tuple>
#include <userver/formats/json/value.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include <boost/pfr/core.hpp>

namespace svetit::db::utils {

template <typename T, typename IdsT, typename NamesT, std::size_t i = 0, typename... Ts>
auto GetTupleOfIds(const NamesT& names, const formats::json::Value& params, const std::tuple<Ts...>& t) {
	if constexpr(i == sizeof...(Ts))
		return std::tuple<>{};
	else {
		using FieldsT = decltype(boost::pfr::structure_to_tuple(T{}));
		constexpr std::size_t idTypeIndex = GetTypeIndexByIdIndex<i>(IdsT{});
		using IdT = std::tuple_element_t<idTypeIndex, FieldsT>;

		const auto name = names.at(idTypeIndex);
		auto value = std::make_tuple(params[name].template As<IdT>());

		auto second = GetTupleOfIds<T, IdsT, NamesT, i + 1>(names, params, t);
		return std::tuple_cat(value, second);
	}
}

} // namespace svetit::db::utils
