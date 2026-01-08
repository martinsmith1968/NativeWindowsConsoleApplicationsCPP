#include "pch.h"
#include "../DNX.Utils/ListUtils.h"
#include "../DNX.Utils/StringUtils.h"

using namespace std;
using namespace DNX::Utils;

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticPadded

#define TEST_GROUP StringUtils

TEST(TEST_GROUP, IsNullOrEmpty_returns_as_expected)
{
    const string text1;
    EXPECT_TRUE(StringUtils::IsNullOrEmpty(&text1));

    const string* text2 = nullptr;
    EXPECT_TRUE(StringUtils::IsNullOrEmpty(text2));

    const string text3 = "a";
    EXPECT_FALSE(StringUtils::IsNullOrEmpty(&text3));
}

TEST(TEST_GROUP, IsNullOrWhiteSpace_returns_as_expected)
{
    const string text1;
    EXPECT_TRUE(StringUtils::IsNullOrWhiteSpace(&text1));

    const string* text2 = nullptr;
    EXPECT_TRUE(StringUtils::IsNullOrWhiteSpace(text2));

    const string text3 = " ";
    EXPECT_TRUE(StringUtils::IsNullOrWhiteSpace(&text3));

    const string text4 = "a";
    EXPECT_FALSE(StringUtils::IsNullOrWhiteSpace(&text4));
}

TEST(TEST_GROUP, IsEmpty_returns_as_expected)
{
    const string text1;
    EXPECT_TRUE(StringUtils::IsEmpty(text1));

    const string text2 = "  ";
    EXPECT_FALSE(StringUtils::IsEmpty(text2));

    const string text3 = "a";
    EXPECT_FALSE(StringUtils::IsEmpty(text3));
}

TEST(TEST_GROUP, IsEmptyrWhiteSpace_returns_as_expected)
{
    const string text1;
    EXPECT_TRUE(StringUtils::IsEmptyOrWhiteSpace(text1));

    const string text2 = "  ";
    EXPECT_TRUE(StringUtils::IsEmptyOrWhiteSpace(text2));

    const string text3 = "a";
    EXPECT_FALSE(StringUtils::IsEmptyOrWhiteSpace(text3));
}

TEST(TEST_GROUP, RTrim_single_character_removes_found_target_returns_as_expected) {
    EXPECT_EQ(StringUtils::RTrim("...text...", '.'), "...text");
    EXPECT_EQ(StringUtils::RTrim("...text...", ' '), "...text...");
    EXPECT_EQ(StringUtils::RTrim(". .text. .", '.'), ". .text. ");
    EXPECT_EQ(StringUtils::RTrim(". .text. .", ' '), ". .text. .");
    EXPECT_EQ(StringUtils::RTrim("..........", '.'), "");
    EXPECT_EQ(StringUtils::RTrim("          ", ' '), "");
}

TEST(TEST_GROUP, LTrim_single_character_removes_found_target_returns_as_expected) {
    EXPECT_EQ(StringUtils::LTrim("...text...", '.'), "text...");
    EXPECT_EQ(StringUtils::LTrim("...text...", ' '), "...text...");
    EXPECT_EQ(StringUtils::LTrim(". .text. .", '.'), " .text. .");
    EXPECT_EQ(StringUtils::LTrim(". .text. .", ' '), ". .text. .");
    EXPECT_EQ(StringUtils::LTrim("..........", '.'), "");
}

TEST(TEST_GROUP, Trim_single_character_removes_found_target_returns_as_expected) {
    EXPECT_EQ(StringUtils::Trim("...text...", '.'), "text");
    EXPECT_EQ(StringUtils::Trim("...text...", ' '), "...text...");
    EXPECT_EQ(StringUtils::Trim(". .text. .", '.'), " .text. ");
    EXPECT_EQ(StringUtils::Trim(". .text. .", ' '), ". .text. .");
    EXPECT_EQ(StringUtils::Trim("..........", '.'), "");
}

TEST(TEST_GROUP, RTrim_text_removes_found_target_returns_as_expected) {
    EXPECT_EQ(StringUtils::RTrim("...text...", "."), "...text");
    EXPECT_EQ(StringUtils::RTrim("#{text}#", "}#"), "#{text");
    EXPECT_EQ(StringUtils::RTrim("...text...", " "), "...text...");
    EXPECT_EQ(StringUtils::RTrim(". .text. .", ". "), ". .text. .");
    EXPECT_EQ(StringUtils::RTrim(". .text. .", " ."), ". .text.");
    EXPECT_EQ(StringUtils::RTrim("..........", "."), "");
    EXPECT_EQ(StringUtils::RTrim("#.#.#text#.#.#", ".#"), "#.#.#text#");
    EXPECT_EQ(StringUtils::RTrim("          ",  " "), "");
}

TEST(TEST_GROUP, LTrim_text_removes_found_target_returns_as_expected) {
    EXPECT_EQ(StringUtils::LTrim("...text...", "."), "text...");
    EXPECT_EQ(StringUtils::LTrim("#{text}#", "#{"), "text}#");
    EXPECT_EQ(StringUtils::LTrim("...text...", " "), "...text...");
    EXPECT_EQ(StringUtils::LTrim(". .text. .", " ."), ". .text. .");
    EXPECT_EQ(StringUtils::LTrim(". .text. .", ". "), ".text. .");
    EXPECT_EQ(StringUtils::Trim("#.#.#text#.#.#", "#."), "#text#.#.#");
    EXPECT_EQ(StringUtils::LTrim("..........", "."), "");
}

TEST(TEST_GROUP, Trim_text_removes_found_target_returns_as_expected) {
    EXPECT_EQ(StringUtils::Trim("...text...", "."), "text");
    EXPECT_EQ(StringUtils::Trim("...text...", " "), "...text...");
    EXPECT_EQ(StringUtils::Trim(". .text. .", "."), " .text. ");
    EXPECT_EQ(StringUtils::Trim(". .text. .", " "), ". .text. .");
    EXPECT_EQ(StringUtils::Trim("#.#.#text#.#.#", "#."), "#text#.#.#");
    EXPECT_EQ(StringUtils::Trim("..........","."), "");
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

    vector<string> result1_vector(begin(result1), end(result1));
    EXPECT_EQ("aaa", result1_vector[0]);
    EXPECT_EQ("bbb", result1_vector[1]);
    EXPECT_EQ("ccc", result1_vector[2]);
    EXPECT_EQ("ddd", result1_vector[3]);

    const auto result2 = StringUtils::SplitText("aaa::bbb::ccc::ddd", "  ");
    EXPECT_EQ(result2.size(), 1);
    EXPECT_EQ("aaa::bbb::ccc::ddd", result2.front());

    auto result3 = StringUtils::SplitText("aaa\nbbb\nccc\nddd", "\n");
    EXPECT_EQ(result3.size(), 4);
    EXPECT_EQ("aaa", ListUtils::GetAt(result3, 0));
    EXPECT_EQ("bbb", ListUtils::GetAt(result3, 1));
    EXPECT_EQ("ccc", ListUtils::GetAt(result3, 2));
    EXPECT_EQ("ddd", ListUtils::GetAt(result3, 3));

    auto result4 = StringUtils::SplitText("aaa:bbb::ccc:::ddd", ":");
    EXPECT_EQ(result4.size(), 7);
    EXPECT_EQ("aaa", ListUtils::GetAt(result4, 0));
    EXPECT_EQ("bbb", ListUtils::GetAt(result4, 1));
    EXPECT_EQ("", ListUtils::GetAt(result4, 2));
    EXPECT_EQ("ccc", ListUtils::GetAt(result4, 3));
    EXPECT_EQ("", ListUtils::GetAt(result4, 4));
    EXPECT_EQ("", ListUtils::GetAt(result4, 5));
    EXPECT_EQ("ddd", ListUtils::GetAt(result4, 6));
}

TEST(TEST_GROUP, SplitTextByAny_string_returns_as_expected) {
    auto result1 = StringUtils::SplitTextByAny("aaa:bbb::ccc:::ddd", ":");
    EXPECT_EQ(result1.size(), 7);
    EXPECT_EQ("aaa", ListUtils::GetAt(result1, 0));
    EXPECT_EQ("bbb", ListUtils::GetAt(result1, 1));
    EXPECT_EQ("", ListUtils::GetAt(result1, 2));
    EXPECT_EQ("ccc", ListUtils::GetAt(result1, 3));
    EXPECT_EQ("", ListUtils::GetAt(result1, 4));
    EXPECT_EQ("", ListUtils::GetAt(result1, 5));
    EXPECT_EQ("ddd", ListUtils::GetAt(result1, 6));

    auto result2 = StringUtils::SplitTextByAny("aaa :bbb ::ccc :::ddd ", ":");
    EXPECT_EQ(result2.size(), 7);
    EXPECT_EQ("aaa ", ListUtils::GetAt(result2, 0));
    EXPECT_EQ("bbb ", ListUtils::GetAt(result2, 1));
    EXPECT_EQ("", ListUtils::GetAt(result2, 2));
    EXPECT_EQ("ccc ", ListUtils::GetAt(result2, 3));
    EXPECT_EQ("", ListUtils::GetAt(result2, 4));
    EXPECT_EQ("", ListUtils::GetAt(result2, 5));
    EXPECT_EQ("ddd ", ListUtils::GetAt(result2, 6));

    auto result3 = StringUtils::SplitTextByAny("aaa :bbb ::ccc :::ddd ", ":", " ");
    EXPECT_EQ(result3.size(), 7);
    EXPECT_EQ("aaa", ListUtils::GetAt(result3, 0));
    EXPECT_EQ("bbb", ListUtils::GetAt(result3, 1));
    EXPECT_EQ("", ListUtils::GetAt(result3, 2));
    EXPECT_EQ("ccc", ListUtils::GetAt(result3, 3));
    EXPECT_EQ("", ListUtils::GetAt(result3, 4));
    EXPECT_EQ("", ListUtils::GetAt(result3, 5));
    EXPECT_EQ("ddd", ListUtils::GetAt(result3, 6));

    auto result4 = StringUtils::SplitTextByAny("aaa:bbb::ccc-ddd:::eee", ":-");
    EXPECT_EQ(result4.size(), 8);
    EXPECT_EQ("aaa", ListUtils::GetAt(result4, 0));
    EXPECT_EQ("bbb", ListUtils::GetAt(result4, 1));
    EXPECT_EQ("", ListUtils::GetAt(result4, 2));
    EXPECT_EQ("ccc", ListUtils::GetAt(result4, 3));
    EXPECT_EQ("ddd", ListUtils::GetAt(result4, 4));
    EXPECT_EQ("", ListUtils::GetAt(result4, 5));
    EXPECT_EQ("", ListUtils::GetAt(result4, 6));
    EXPECT_EQ("eee", ListUtils::GetAt(result4, 7));

    auto result5 = StringUtils::SplitTextByAny("aaa:bbb::ccc:::ddd", ":", "", true);
    EXPECT_EQ(result5.size(), 4);
    EXPECT_EQ("aaa", ListUtils::GetAt(result5, 0));
    EXPECT_EQ("bbb", ListUtils::GetAt(result5, 1));
    EXPECT_EQ("ccc", ListUtils::GetAt(result5, 2));
    EXPECT_EQ("ddd", ListUtils::GetAt(result5, 3));
}

//----------------------------------------------------------------------------------------------------
class StringUtils_SeparateByLineEndingsFixture
    : public ::testing::TestWithParam<tuple<string, int>>
{
protected:
    void SetUp() override
    {
        m_input_text          = get<0>(GetParam());
        m_expected_line_count = get<1>(GetParam());
    }
    string m_input_text;
    int m_expected_line_count = 0;
};

TEST_P(StringUtils_SeparateByLineEndingsFixture, SeparateByLineEndings_can_parse_text_correctly)
{
    // Act
    const auto lines = StringUtils::SeparateByLineEndings(m_input_text);

    // Assert
    EXPECT_EQ(m_expected_line_count, lines.size());
}

INSTANTIATE_TEST_CASE_P(
    SeparateByLineEndingsTests,
    StringUtils_SeparateByLineEndingsFixture,
    ::testing::Values(
        make_tuple("", 0)
        , make_tuple(" ", 1)
        , make_tuple("\r", 2)
        , make_tuple("\n", 2)
        , make_tuple("\r\n", 2)
        , make_tuple("\n\r", 3)
        , make_tuple("Line1\r\nLine2\r\nLine3", 3)
        , make_tuple("Line1\rLine2\rLine3", 3)
        , make_tuple("Line1\nLine2\nLine3", 3)
        , make_tuple("Line1\nLine2\rLine3", 3)
        , make_tuple("\nLine1\nLine2\rLine3\n", 5)
        , make_tuple("\n\rLine1\nLine2\rLine3\n\r", 7)
    )
);

//----------------------------------------------------------------------------------------------------
class StringUtils_NormalizeLineEndingsDefaultLineEndingFixture
    : public ::testing::TestWithParam<tuple<string, string>>
{
protected:
    void SetUp() override
    {
        m_input_text = get<0>(GetParam());
        m_expected_text = get<1>(GetParam());
    }
    string m_input_text;
    string m_expected_text;
};

TEST_P(StringUtils_NormalizeLineEndingsDefaultLineEndingFixture, NormalizeLineEndings_can_process_text_correctly)
{
    // Act
    const auto result = StringUtils::NormalizeLineEndings(m_input_text);

    // Assert
    EXPECT_EQ(m_expected_text, result);
}

INSTANTIATE_TEST_CASE_P(
    NormalizeLineEndingsDefaultLineEndingTests,
    StringUtils_NormalizeLineEndingsDefaultLineEndingFixture,
    ::testing::Values(
        make_tuple("", "")
        , make_tuple(" ", " ")
        , make_tuple("\r", "\r\n")
        , make_tuple("\n", "\r\n")
        , make_tuple("\r\n", "\r\n")
        , make_tuple("\n\r", "\r\n\r\n")
        , make_tuple("Line1\r\nLine2\r\nLine3", "Line1\r\nLine2\r\nLine3")
        , make_tuple("Line1\rLine2\rLine3", "Line1\r\nLine2\r\nLine3")
        , make_tuple("Line1\nLine2\nLine3", "Line1\r\nLine2\r\nLine3")
        , make_tuple("Line1\nLine2\rLine3", "Line1\r\nLine2\r\nLine3")
        , make_tuple("\nLine1\nLine2\rLine3\n", "\r\nLine1\r\nLine2\r\nLine3\r\n")
        , make_tuple("\n\rLine1\nLine2\rLine3\n\r", "\r\n\r\nLine1\r\nLine2\r\nLine3\r\n\r\n")
    )
);
