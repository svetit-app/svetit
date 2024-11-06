#pragma once

#include <svetit/utils/typename.hpp>

#include <algorithm>
#include <array>

namespace svetit::utils {

namespace {

constexpr auto snakeLength(const char* src, const char* end)
{
	int i = 0;
	for (; src != end; src++, i++) {
		if (*src >= 'A' && *src <= 'Z' && i > 0)
			++i;
		if (*src == ':')
			i = -1;
	}
	return i;
}

} // namespace

template<typename T, unsigned LenPref, unsigned LenSuf>
constexpr auto HandlerName(const char (&prefix)[LenPref], const char (&suffix)[LenSuf], char c) {
	constexpr const auto full = type_name<T>();
	constexpr const auto end = full.data() + full.size();
	constexpr const auto slen = snakeLength(full.data(), end);
	std::array<char, slen + LenPref + LenSuf - 1> result = {};

	int i = LenPref - 1;
	for (const char* src = full.data(); src != end; src++, i++) {
		if (*src >= 'A' && *src <= 'Z') {
			if (static_cast<unsigned>(i) > LenPref - 1 && static_cast<unsigned>(i) < LenPref + slen)
				result[i++] = c;
			if (static_cast<unsigned>(i) < LenPref + slen)
				result[i] = *src + ('a' - 'A');
		}
		else if (*src == ':')
			i = LenPref - 2;
		else if (static_cast<unsigned>(i) < LenPref + slen)
			result[i] = *src;
	}

	std::copy_n(prefix, LenPref - 1, result.begin());
	std::copy_n(suffix, LenSuf - 1, result.begin() + slen + LenPref - 1);
	result[result.size() - 1] = '\0';
	return result;
}

} // namespace svetit::utils
