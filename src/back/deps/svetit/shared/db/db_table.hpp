#pragma once

#include "db_base.hpp"

#include <boost/pfr/core_name.hpp>

namespace svetit::db {

template<typename T>
struct Table {
	static std::string FieldsString();

protected:
	std::shared_ptr<Base> _db;
};

template<typename T>
inline std::string Table<T>::FieldsString()
{
	auto names = boost::pfr::names_as_array<T>();
	std::size_t size = 0;
	for (auto&& it: names)
		size += it.size() + 1;
	if (size == 0)
		return std::string{};

	std::string res;
	res.reserve(size * 1.2f);
	res.resize(size);

	std::size_t i = 0;
	for (auto&& it: names)
	{
		for (auto&& c : it)
		{
			if (c >= 'A' && c <= 'Z')
			{
				res.resize(res.size() + 1);
				res[i++] = '_';
				res[i++] = std::tolower(c);
			}
			else
				res[i++] = c;
		}

		res[i++] = ',';
	}

	res.resize(res.size() - 1);
	return res;
}

} // namespace svetit::db
