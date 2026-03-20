#include "ast0.h"

#include <catch2/catch_test_macros.hpp>
#include <limits>
#include <sstream>

void TestAst(const std::string &code, const std::string &expected)
{
    std::stringstream captured_cout;
    auto *original_cout_buf = std::cout.rdbuf();
    std::cout.rdbuf(captured_cout.rdbuf());

    clang::tooling::runToolOnCode(std::make_unique<ImplicitCastAction>(), code, "FakeFileChangeIfYouWantToSuffer.cpp");

    std::cout.rdbuf(original_cout_buf);

    std::string captured_output = captured_cout.str();

    REQUIRE(captured_output == expected);
}

TEST_CASE("Test mul")
{

    std::string Code = R"(
        double sum(int a, float b) {
	        return a + b;
        }

        int mul(float a, float b) {
	        return a + sum(a, b);
        }
    )";

    TestAst(Code, "cast between floats (different size)\nintegral -> floating\nfloating -> integral\ncast between floats (different size)\nfloating -> integral\n5\n");
}