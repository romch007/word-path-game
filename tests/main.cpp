#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>
#include <utils.hpp>

TEST_CASE("Utils", "[utils]") {
  SECTION("lowercase") {
    std::string test_str = "hEyfoO";
    lowercase(test_str);
    REQUIRE( test_str == "heyfoo" );
  }
}

int main(int argc, char* argv[]) {
  return Catch::Session().run(argc, argv);
}
