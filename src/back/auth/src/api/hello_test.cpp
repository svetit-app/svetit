#include "hello.hpp"

#include <userver/utest/utest.hpp>

UTEST(SayHelloTo, Basic) {
  EXPECT_EQ(svetit::auth::handlers::SayHelloTo("Developer"), "Hello, Developer!\n");
  EXPECT_EQ(svetit::auth::handlers::SayHelloTo({}), "Hello, unknown user!\n");
}
