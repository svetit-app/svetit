#pragma once

#include <svetit/utils/typename.hpp>

#include <array>

namespace svetit::db::utils {

namespace {

constexpr auto snakeLength(const char* src, const char* end, char stop)
{
	std::size_t res = 0;
	for (; src != end; src++) {
		if (*src != stop) {
			if (*src >= 'A' && *src <= 'Z' && res > 0)
				++res;
			++res;
		} else if (res > 0)
			break;
	}
	return res;
}

} // namespace

template<typename T>
constexpr auto TableName() {
	constexpr const auto full = type_name<T>();
	constexpr const auto s = full.data();
	constexpr const auto end = full.data() + full.size();
	constexpr const auto vendorLength = snakeLength(s, end, ':');
	constexpr const auto appLength = snakeLength(s + vendorLength + 2, end, ':');
	constexpr const auto modelLength = snakeLength(s + vendorLength + 2 + appLength + 2, end, ':');
	constexpr const auto tableLength = snakeLength(s + vendorLength + 2 + appLength + 2 + modelLength, end, ':');

	std::array<char, appLength + 1 + tableLength + 1> result = {};
	result[appLength + 1 + tableLength] = '\0';

	std::size_t i = 0, x = 0;
	for (const char* src = s; src != end; src++, i++) {
		if (i < vendorLength + 2)
			continue;
		if (i > vendorLength + 2 + appLength
		 && i < vendorLength + 2 + appLength + 2 + modelLength + 2)
			continue;
		if (*src == ':') {
			result[x++] = '.';
			continue;
		}

		if (*src >= 'A' && *src <= 'Z') {
			if (x > 0 && x != appLength + 1)
				result[x++] = '_';
			result[x] = *src + ('a' - 'A');
		} else {
			result[x] = *src;
		}
		++x;
	}
	result[x] = '\0';
	return result;
}

} // namespace svetit::db::utils
