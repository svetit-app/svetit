#pragma once

// #include <map>
#include <string>
#include "../repo/repository.hpp"
// #include <string_view>

namespace svetit::auth {

class Session final {
public:
	explicit Session(Repository& rep);

	void Save(const std::string& userId);

private:
	Repository& _repo;
};

} // namespace svetit::auth

