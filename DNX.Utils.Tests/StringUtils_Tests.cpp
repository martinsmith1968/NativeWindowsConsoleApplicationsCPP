#include "pch.h"
#include "../DNX.Utils/StringUtils.h"

using namespace std;
using namespace DNX::Utils;

#define TEST_GROUP StringUtils

TEST(TEST_GROUP, RTrim_single_character_removes_found_target_returns_as_expected) {
    EXPECT_EQ(StringUtils::RTrim("...text...", '.'), "...text");
    EXPECT_EQ(StringUtils::RTrim("...text...", ' '), "...text...");
    EXPECT_EQ(StringUtils::RTrim(". .text. .", '.'), ". .text. ");
    EXPECT_EQ(StringUtils::RTrim(". .text. .", ' '), ". .text. .");
}

TEST(TEST_GROUP, LTrim_single_character_removes_found_target_returns_as_expected) {
    EXPECT_EQ(StringUtils::LTrim("...text...", '.'), "text...");
    EXPECT_EQ(StringUtils::LTrim("...text...", ' '), "...text...");
    EXPECT_EQ(StringUtils::LTrim(". .text. .", '.'), " .text. .");
    EXPECT_EQ(StringUtils::LTrim(". .text. .", ' '), ". .text. .");
}

TEST(TEST_GROUP, Trim_single_character_removes_found_target_returns_as_expected) {
    EXPECT_EQ(StringUtils::Trim("...text...", '.'), "text");
    EXPECT_EQ(StringUtils::Trim("...text...", ' '), "...text...");
    EXPECT_EQ(StringUtils::Trim(". .text. .", '.'), " .text. ");
    EXPECT_EQ(StringUtils::Trim(". .text. .", ' '), ". .text. .");
}

TEST(TEST_GROUP, RTrim_text_removes_found_target_returns_as_expected) {
    EXPECT_EQ(StringUtils::RTrim("...text...", "."), "...text");
    EXPECT_EQ(StringUtils::RTrim("#{text}#", "}#"), "#{text");
    EXPECT_EQ(StringUtils::RTrim("...text...", " "), "...text...");
    EXPECT_EQ(StringUtils::RTrim(". .text. .", ". "), ". .text. .");
    EXPECT_EQ(StringUtils::RTrim(". .text. .", " ."), ". .text.");
}

TEST(TEST_GROUP, After_returns_as_expected) {
    EXPECT_EQ(StringUtils::After("This is some text", "some"), " text");
    EXPECT_EQ(StringUtils::After("This is some text", "bob"), "");
    EXPECT_EQ(StringUtils::After("This is some [[Red]]text[[/Red]]", "[["), "Red]]text[[/Red]]");
    EXPECT_EQ(StringUtils::After("This is some text", " "), "is some text");
    EXPECT_EQ(StringUtils::After("This is some text", ""), "");
    EXPECT_EQ(StringUtils::After("This is some text", ""), "");
    EXPECT_EQ(StringUtils::After("", "o"), "");
}

TEST(TEST_GROUP, Before_returns_as_expected) {
    EXPECT_EQ(StringUtils::Before("This is some text", "some"), "This is ");
    EXPECT_EQ(StringUtils::Before("This is some text", "bob"), "");
    EXPECT_EQ(StringUtils::Before("This is some [[Red]]text[[/Red]]", "[["), "This is some ");
    EXPECT_EQ(StringUtils::Before("This is some text", " "), "This");
    EXPECT_EQ(StringUtils::Before("This is some text", ""), "");
    EXPECT_EQ(StringUtils::Before("This is some text", ""), "");
    EXPECT_EQ(StringUtils::Before("", "o"), "");
}

TEST(TEST_GROUP, LPad_returns_as_expected) {
    EXPECT_EQ(StringUtils::LPad("9", 3, '0'), "009");
    EXPECT_EQ(StringUtils::LPad("9", 3, ' '), "  9");
    EXPECT_EQ(StringUtils::LPad("12345", 3, '0'), "12345");
}

TEST(TEST_GROUP, RPad_returns_as_expected) {
    EXPECT_EQ(StringUtils::RPad("9", 3, '0'), "900");
    EXPECT_EQ(StringUtils::RPad("9", 3, ' '), "9  ");
    EXPECT_EQ(StringUtils::RPad("12345", 3, '0'), "12345");
}

TEST(TEST_GROUP, CountOccurrences_char_returns_as_expected) {
    EXPECT_EQ(StringUtils::CountOccurrences("a1b2a3b4a5b6a7b8", 'a'), 4);
    EXPECT_EQ(StringUtils::CountOccurrences("a1b2a3b4a5b6a7b8", 'b'), 4);
    EXPECT_EQ(StringUtils::CountOccurrences("a1b2a3b4a5b6a7b8", '8'), 1);
    EXPECT_EQ(StringUtils::CountOccurrences("a1b2a3b4a5b6a7b8", ' '), 0);
}

TEST(TEST_GROUP, CountOccurrences_substr_returns_as_expected) {
    EXPECT_EQ(StringUtils::CountOccurrences("aaa:bbb:ccc:ddd", ":"), 3);
    EXPECT_EQ(StringUtils::CountOccurrences("aaa:bbb:ccc:ddd", "aa"), 2);
    EXPECT_EQ(StringUtils::CountOccurrences("aaa:bbb:ccc:ddd", "aaa"), 1);
}

TEST(TEST_GROUP, SplitText_char_returns_as_expected) {
    EXPECT_EQ(StringUtils::SplitText("aaa:bbb:ccc:ddd", ':').size(), 4);
    EXPECT_EQ(StringUtils::SplitText("aaa:bbb:ccc:ddd", ' ').size(), 1);
}

TEST(TEST_GROUP, SplitText_string_returns_as_expected) {
    const auto result1 = StringUtils::SplitText("aaa::bbb::ccc::ddd", "::");
    EXPECT_EQ(result1.size(), 4);

    vector<string> result1_vector(std::begin(result1), std::end(result1));
    EXPECT_EQ("aaa", result1_vector[0]);
    EXPECT_EQ("bbb", result1_vector[1]);
    EXPECT_EQ("ccc", result1_vector[2]);
    EXPECT_EQ("ddd", result1_vector[3]);

    const auto result2 = StringUtils::SplitText("aaa::bbb::ccc::ddd", "  ");
    EXPECT_EQ(result2.size(), 1);
    EXPECT_EQ("aaa::bbb::ccc::ddd", result2.front());

    const auto result3 = StringUtils::SplitText("aaa\nbbb\nccc\nddd", "\r\n");
    EXPECT_EQ(result3.size(), 4);

    vector result3_vector(std::begin(result3), std::end(result3));
    EXPECT_EQ("aaa", result1_vector[0]);
    EXPECT_EQ("bbb", result1_vector[1]);
    EXPECT_EQ("ccc", result1_vector[2]);
    EXPECT_EQ("ddd", result1_vector[3]);
}
