#include "role.hpp"

namespace svetit::space {

/*static*/ std::string Role::ToString(Role::Type role)
{
	switch (role) {
	case Admin:
		return "admin";
	case User:
		return "user";
	case Guest:
		return "guest";
	default:
		break;
	}

	return "";
}

/*static*/ Role::Type Role::FromString(const std::string& role)
{
	if (role == "admin")
		return Admin;
	if (role == "user")
		return User;
	if (role == "guest")
		return Guest;

	return Unknown;
}

} // namespace svetit::space