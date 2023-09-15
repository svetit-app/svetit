#include "hello.hpp"

#include <userver/utest/utest.hpp>

UTEST(SayHelloTo, Basic) {
  EXPECT_EQ(svetit::space::SayHelloTo("Developer"), "Hello, Developer!\n");
  EXPECT_EQ(svetit::space::SayHelloTo({}), "Hello, unknown user!\n");
}
