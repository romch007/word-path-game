#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>
#include <utils.hpp>

TEST_CASE("Utils", "[utils]") {
  SECTION("lowercase") {
    std::string test_str = "hEyfoO";
    wordpath::lowercase(test_str);
    REQUIRE( test_str == "heyfoo" );
  }

  SECTION("round_tostr") {
    float f = 33.333333;
    auto result = wordpath::round_to_str(f, 3);
    REQUIRE( result == "33.333" );
  }

  SECTION("is_in") {
    wordpath::word_list words = { "foo", "hey", "abc", "bar" };
    REQUIRE( wordpath::is_in(words, "abc") );
    REQUIRE( !wordpath::is_in(words, "test") );
  }
}

int main(int argc, char* argv[]) {
  return Catch::Session().run(argc, argv);
}
