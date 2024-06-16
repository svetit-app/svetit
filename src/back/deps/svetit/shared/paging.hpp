#pragma once

#include <shared/move_tuple.hpp>

#include <userver/formats/json/value.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include <boost/pfr/core.hpp>
#include <boost/pfr/traits.hpp>

namespace svetit {

struct Paging {
	int start, limit;
};

template<typename T>
class PagingResult {
	constexpr static auto makeTupleOfModelAndTotal() {
		return std::tuple_cat(boost::pfr::structure_to_tuple(T{}), std::tuple<int64_t>{});
	}

public:
	using RawContainer = std::vector<decltype(makeTupleOfModelAndTotal())>;

	void operator=(RawContainer&& data)
	{
		items.reserve(data.size());
		for (auto&& it : data) {
			T item;
			auto itemRef = boost::pfr::structure_tie(item);
			moveTuple(std::move(it), itemRef);
			items.push_back(std::move(item));
		}

		if (!data.empty())
			total = std::get<std::tuple_size_v<typename RawContainer::value_type> - 1>(data.front());
	}

	std::vector<T> items;
	int64_t total = 0;
};

Paging parsePaging(const server::http::HttpRequest& req);
Paging parsePaging(const formats::json::Value& params);

} // namespace svetit
