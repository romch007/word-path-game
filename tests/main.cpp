#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>
#include <utils.hpp>

TEST_CASE("Utils", "[utils]") {
  SECTION("lowercase") {
    std::string test_str = "hEyfoO";
    lowercase(test_str);
    REQUIRE( test_str == "heyfoo" );
  }

  SECTION("round_tostr") {
    float f = 33.333333;
    auto result = round_to_str(f, 3);
    REQUIRE( result == "33.333" );
  }

  SECTION("is_in") {
    word_list words = { "foo", "hey", "abc", "bar", "test" };
    REQUIRE( is_in(words, "abc") );
    REQUIRE( !is_in(words, "test") );
  }
}

int main(int argc, char* argv[]) {
  return Catch::Session().run(argc, argv);
}
