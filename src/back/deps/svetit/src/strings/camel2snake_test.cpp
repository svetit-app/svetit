#include <svetit/strings/camel2snake.hpp>

#include <userver/utest/utest.hpp>
#include <userver/utest/using_namespace_userver.hpp>

using namespace svetit;

UTEST(Camel2Snake, Basic) {
	EXPECT_EQ("some_camel_string", Camel2Snake("SomeCamelString"));
	EXPECT_EQ("some_camel_string", Camel2Snake("Some_Camel_String"));
	EXPECT_EQ("some_camel_string", Camel2Snake("someCamelString"));
}
