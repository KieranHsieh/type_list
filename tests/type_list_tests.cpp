/*
MIT License

Copyright (c) 2023 Kieran Hsieh

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <kt/type_list.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("type_list reports correct size", "[access]") {
    REQUIRE(kt::type_list<>.size() == 0);    // NOLINT
    REQUIRE(kt::type_list<int>.size() == 1);
    REQUIRE(kt::type_list<int, int>.size() == 2);
}

TEST_CASE("type_list can check for emptiness", "[access]") {
    REQUIRE(kt::type_list<>.empty());
    REQUIRE(!kt::type_list<int>.empty());
}

TEST_CASE("type_list can access first element", "[access]") {
    REQUIRE(std::is_same_v<kt::from_tag_t<decltype(kt::type_list<int>.front())>, int>);
    REQUIRE(std::is_same_v<kt::from_tag_t<decltype(kt::type_list<float, int>.front())>, float>);
}

TEST_CASE("type_list can access last element", "[access]") {
    REQUIRE(std::is_same_v<kt::from_tag_t<decltype(kt::type_list<int>.back())>, int>);
    REQUIRE(std::is_same_v<kt::from_tag_t<decltype(kt::type_list<int, float>.back())>, float>);
}

TEST_CASE("type_list can check for element existence", "[access]") {
    REQUIRE(kt::type_list<int, double>.contains(kt::tag<int>));
    REQUIRE(kt::type_list<int, double>.contains(kt::tag<double>));
    REQUIRE(!kt::type_list<int, double>.contains(kt::tag<float>));
    REQUIRE(!kt::type_list<>.contains(kt::tag<int>));
}

TEST_CASE("type_list can get the index of an element", "[access]") {
    REQUIRE(kt::type_list<int, double>.index_of(kt::tag<int>) == 0);
    REQUIRE(kt::type_list<int, double>.index_of(kt::tag<int>) == 0);
    REQUIRE(kt::type_list<int, double>.index_of(kt::tag<double>) == 1);
    REQUIRE(kt::type_list<int, double>.index_of(kt::tag<short>) == kt::npos);
}

TEST_CASE("type_list can count the occurences of an element", "[access]") {
    REQUIRE(kt::type_list<>.count_of(kt::tag<int>) == 0);
    REQUIRE(kt::type_list<double>.count_of(kt::tag<int>) == 0);
    REQUIRE(kt::type_list<int, double>.count_of(kt::tag<double>) == 1);
    REQUIRE(kt::type_list<int, double, double>.count_of(kt::tag<double>) == 2);
}

TEST_CASE("type_list can do conjunction and disjunction operations", "[access]") {
    REQUIRE(kt::type_list<>.all_of(kt::func<std::is_const>));
    REQUIRE(kt::type_list<const int>.all_of(kt::func<std::is_const>));
    REQUIRE(!kt::type_list<int>.all_of(kt::func<std::is_const>));
    REQUIRE(!kt::type_list<const int, int>.all_of(kt::func<std::is_const>));

    REQUIRE(!kt::type_list<>.one_of(kt::func<std::is_const>));
    REQUIRE(kt::type_list<const int, int>.one_of(kt::func<std::is_const>));
}

TEST_CASE("type_list can access a given index", "[access]") {
    REQUIRE(std::is_same_v<kt::from_tag_t<decltype(kt::type_list<int, double, short>.at(kt::index<0>))>, int>);
    REQUIRE(std::is_same_v<kt::from_tag_t<decltype(kt::type_list<int, double, short>.at(kt::index<1>))>, double>);
    REQUIRE(std::is_same_v<kt::from_tag_t<decltype(kt::type_list<int, double, short>.at(kt::index<2>))>, short>);
}

TEST_CASE("type_list can be comapred with == and !=", "[equality]") {
    REQUIRE(kt::type_list<int, double, float> == kt::type_list<int, double, float>);
    REQUIRE(kt::type_list<int> != kt::type_list<float>);
    REQUIRE(kt::type_list<int> != kt::type_list<int, float>);
    REQUIRE(kt::type_list<int> != kt::type_list<int, int>);
}

TEST_CASE("type_list can append elements", "[modifiers]") {
    REQUIRE(kt::type_list<int>.append(kt::tag<double>) == kt::type_list<int, double>);
    REQUIRE(kt::type_list<>.append(kt::tag<double>) == kt::type_list<double>);
    REQUIRE(kt::type_list<int, double>.append(kt::tag<double>) == kt::type_list<int, double, double>);

    REQUIRE(kt::type_list<>.append(kt::type_list<int>) == kt::type_list<int>);
    REQUIRE(kt::type_list<>.append(kt::type_list<int, double>) == kt::type_list<int, double>);
    REQUIRE(kt::type_list<short>.append(kt::type_list<int>) == kt::type_list<short, int>);
    REQUIRE(kt::type_list<short>.append(kt::type_list<int, double>) == kt::type_list<short, int, double>);

    REQUIRE(kt::type_list<> + kt::tag<int> == kt::type_list<int>);
    REQUIRE(kt::type_list<int> + kt::tag<int> == kt::type_list<int, int>);
    REQUIRE(kt::type_list<int> + kt::tag<double> == kt::type_list<int, double>);
    REQUIRE(kt::type_list<int> + kt::type_list<int, double> == kt::type_list<int, int, double>);
}

TEST_CASE("type_list can prepend elements", "[modifiers]") {
    REQUIRE(kt::type_list<int>.prepend(kt::tag<double>) == kt::type_list<double, int>);
    REQUIRE(kt::type_list<>.prepend(kt::tag<double>) == kt::type_list<double>);
    REQUIRE(kt::type_list<int, double>.prepend(kt::tag<double>) == kt::type_list<double, int, double>);

    REQUIRE(kt::type_list<>.prepend(kt::type_list<int>) == kt::type_list<int>);
    REQUIRE(kt::type_list<>.prepend(kt::type_list<int, double>) == kt::type_list<int, double>);
    REQUIRE(kt::type_list<short>.prepend(kt::type_list<int>) == kt::type_list<int, short>);
    REQUIRE(kt::type_list<short>.prepend(kt::type_list<int, double>) == kt::type_list<int, double, short>);
}

TEST_CASE("type_list can apply metafunctions", "[modifiers]") {
    REQUIRE(kt::type_list<>.apply(kt::func<std::add_const>) == kt::type_list<>);
    REQUIRE(kt::type_list<int, double>.apply(kt::func<std::add_const>) == kt::type_list<const int, const double>);
    REQUIRE(kt::type_list<int, double>.apply(kt::index<0>, kt::func<std::add_const>) == kt::type_list<const int, double>);
    REQUIRE(kt::type_list<int, double, float, short, long>.apply_range(kt::index<1>, kt::index<3>, kt::func<std::add_const>) ==
            kt::type_list<int, const double, const float, short, long>);
}

TEST_CASE("type_list can filter using metafunctions", "[modifiers]") {
    REQUIRE(kt::type_list<>.filter(kt::func<std::is_const>) == kt::type_list<>);
    REQUIRE(kt::type_list<int, const double, float>.filter(kt::func<std::is_const>) == kt::type_list<const double>);
    REQUIRE(kt::type_list<int, const double, float>.filter_range(kt::index<1>, kt::index<3>, kt::func<std::is_const>) ==
            kt::type_list<int, const double>);
}

TEST_CASE("type_list can remove elements", "[modifiers]") {
    REQUIRE(kt::type_list<>.remove(kt::tag<int>) == kt::type_list<>);
    REQUIRE(kt::type_list<int, int>.remove(kt::tag<int>) == kt::type_list<>);
    REQUIRE(kt::type_list<int, int, double>.remove(kt::tag<int>) == kt::type_list<double>);

    REQUIRE(kt::type_list<int, double>.remove(kt::index<0>) == kt::type_list<double>);
    REQUIRE(kt::type_list<int, double, short>.remove(kt::index<1>) == kt::type_list<int, short>);
    REQUIRE(kt::type_list<int, double, short>.remove(kt::index<2>) == kt::type_list<int, double>);

    REQUIRE(kt::type_list<int, double, short, float>.remove(kt::index<1>, kt::index<3>) == kt::type_list<int, float>);
}

TEST_CASE("type_list can pop front and back elements", "[modifiers]") {
    REQUIRE(kt::type_list<>.pop_back() == kt::type_list<>);
    REQUIRE(kt::type_list<int>.pop_back() == kt::type_list<>);
    REQUIRE(kt::type_list<int, float>.pop_back() == kt::type_list<int>);

    REQUIRE(kt::type_list<>.pop_front() == kt::type_list<>);
    REQUIRE(kt::type_list<int>.pop_front() == kt::type_list<>);
    REQUIRE(kt::type_list<int, float>.pop_front() == kt::type_list<float>);
}