#include "stdafx.h"
#include "StringUtils.h"
#include <algorithm>
#include <sstream>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyMiscUseAnonymousNamespace
// ReSharper disable CppClangTidyMiscUseInternalLinkage
// ReSharper disable CppTooWideScopeInitStatement

using namespace std;
using namespace DNX::Utils;

//--------------------------------------------------------------------------
// Class: StringUtils
//--------------------------------------------------------------------------

// From : https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string
wstring StringUtils::ToWideString(const string& str)
{
    wstring wstr(str.length(), 0);
    transform(str.begin(), str.end(), wstr.begin(), [](const char c) {
        return static_cast<wchar_t>(c);
        });

    return wstr;
}

// From : https://stackoverflow.com/questions/4804298/how-to-convert-wstring-into-string
string StringUtils::ToString(const wstring& wstr)
{
    string str(wstr.length(), 0);
    transform(wstr.begin(), wstr.end(), str.begin(), [](const wchar_t c) {
        return static_cast<char>(c);
        });

    return str;
}

string StringUtils::Trim(const string& str, const char removeChar)
{
    return LTrim(RTrim(str, removeChar), removeChar);
}

string StringUtils::LTrim(const string& str, const char removeChar)
{
    auto first = str.find_first_not_of(removeChar);
    if (first == string::npos)
    {
        if (str.find_first_of(removeChar) == string::npos)
            return "";
        else
            first = str.length();
    }

    return str.substr(first, string::npos);
}

string StringUtils::RTrim(const string& str, const char removeChar)
{
    auto last = str.find_last_not_of(removeChar);
    if (last == string::npos)
    {
        if (str.find_last_of(removeChar) == string::npos)
            return str;
        else
            last = -1;
    }

    return str.substr(0, last + 1);
}

string StringUtils::Trim(const string& str, const string& prefix_suffix)
{
    return RTrim(LTrim(str, prefix_suffix), prefix_suffix);
}

string StringUtils::LTrim(const string& str, const string& prefix)
{
    string result = str;
    while (StartsWith(result, prefix))
        result = Right(result, result.length() - prefix.length());

    return result;
}

string StringUtils::RTrim(const string& str, const string& suffix)
{
    string result = str;
    while (EndsWith(result, suffix))
        result = Left(result, result.length() - suffix.length());

    return result;
}

string StringUtils::LPad(const string& str, const size_t min_length, const char prefix)
{
    if (str.size() >= min_length)
        return str;

    return Right(string(min_length, prefix) + str, min_length);
}

string StringUtils::RPad(const string& str, const size_t min_length, const char prefix)
{
    if (str.size() >= min_length)
        return str;

    return Left(str + string(min_length, prefix), min_length);
}

string StringUtils::Before(const string& str, const string& find)
{
    if (str.empty() || find.empty())
        return "";

    const auto pos = str.find(find);
    if (pos == string::npos)
        return "";

    return Left(str, pos);
}

string StringUtils::After(const string& str, const string& find)
{
    if (str.empty() || find.empty())
        return "";

    const auto pos = str.find(find);
    if (pos == string::npos)
        return "";

    return Right(str, str.length() - pos - find.length());

}

string StringUtils::BeforeLast(const string& str, const string& find)
{
    if (str.empty() || find.empty())
        return "";

    const auto pos = str.find_last_of(find);
    if (pos == string::npos)
        return "";

    return Left(str, pos);
}

string StringUtils::AfterLast(const string& str, const string& find)
{
    if (str.empty() || find.empty())
        return "";

    const auto pos = str.find_last_of(find);
    if (pos == string::npos)
        return "";

    return Right(str, str.length() - pos - find.length());

}

string StringUtils::Between(const string& str, const string& first, const string& second)
{
    return BeforeLast(After(str, first), second);
}

string StringUtils::BetweenInner(const string& str, const string& first, const string& second)
{
    return Before(AfterLast(str, first), second);
}

string StringUtils::ReplaceString(string subject, const string& search, const string& replace)
{
    size_t pos = 0;

    while ((pos = subject.find(search, pos)) != string::npos)
    {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }

    return subject;
}

string StringUtils::Repeat(const string& subject, const int count)
{
    // Source : https://stackoverflow.com/questions/166630/how-can-i-repeat-a-string-a-variable-number-of-times-in-c
    ostringstream os;
    for (int i = 0; i < count; i++)
        os << subject;
    return os.str();
}

string StringUtils::ToLower(const string& text)
{
    string copy = text;
    transform(copy.begin(), copy.end(), copy.begin(), tolower);

    return copy;
}

string StringUtils::ToUpper(const string& text)
{
    string copy = text;
    transform(copy.begin(), copy.end(), copy.begin(), toupper);

    return copy;
}

string StringUtils::Left(const string& text, const size_t length)
{
    return text.substr(0, length);
}

string StringUtils::Right(const string& text, const size_t length)
{
    return text.substr(text.length() - length, string::npos);
}

string StringUtils::BoolToString(const bool value, const string& trueValue, const string& falseValue)
{
    if (value)
        return trueValue;
    else
        return falseValue;
}

list<string> StringUtils::SplitText(const string& str, const char splitChar, const char trimChar)
{
    stringstream ss(str);

    list<string> list;

    while (ss.good())
    {
        string piece;
        getline(ss, piece, splitChar);

        if (trimChar != NULL)
        {
            piece = RTrim(piece, trimChar);
        }
        piece = RTrim(piece, splitChar);

        if (!piece.empty())
        {
            list.push_back(piece);
        }
    }

    return list;
}

list<string> StringUtils::SplitText(const string& str, const string& splitText, const string& trimText)
{
    list<string> list;

    string text = str;

    while (!text.empty())
    {
        const auto index = text.find(splitText);
        auto piece = (index == string::npos)
            ? text
            : text.substr(0, index);

        if (!trimText.empty())
        {
            for (const char trimChar : trimText)
            {
                piece = RTrim(piece, trimChar);
            }
        }

        list.emplace_back(piece);

        text = (index == string::npos || index + splitText.length() >= text.length())
            ? ""
            : text.substr(index + splitText.length(), string::npos);
    }

    return list;
}

list<string> StringUtils::SplitTextByAny(const string& str, const string& splitText, const string& trimText, bool ignoreMultipleSplitChars)
{
    list<string> list;

    string text = str;

    while (!text.empty())
    {
        const auto index = text.find_first_of(splitText);
        auto piece = (index == string::npos)
            ? text
            : text.substr(0, index);

        if (!trimText.empty())
        {
            for (const char trimChar : trimText)
            {
                piece = RTrim(piece, trimChar);
            }
        }

        list.emplace_back(piece);

        text = (index == string::npos || index + 1 >= text.length())
            ? ""
            : text.substr(index + 1, string::npos);

        if (ignoreMultipleSplitChars)
        {
            for (const char trimChar : splitText)
            {
                text = LTrim(text, trimChar);
            }
        }
    }

    return list;
}

string StringUtils::JoinText(const list<string>& list, const string& delimiter)
{
    ostringstream ss;

    for (auto iter = list.begin(); iter != list.end(); ++iter)
    {
        if (iter != list.begin())
        {
            ss << delimiter;
        }

        ss << *iter;
    }

    return ss.str();
}

bool StringUtils::Contains(const string& str, const string& search)
{
    if (str.empty() || search.empty())
        return false;

    const auto pos = str.find(search);

    return pos != string::npos;
}

bool StringUtils::ContainsOnly(const string& str, const string& characters)
{
    return ContainsOnly(str, characters.c_str());
}

bool StringUtils::ContainsOnly(const string& str, const char* characters)
{
    if (str.empty() || strlen(characters) == 0)
        return false;

    for (size_t x = 0; x < strlen(characters); ++x)
    {
        auto pos = str.find(characters[x]);
        if (pos == string::npos)
            return false;
    }

    return true;
}

bool StringUtils::ContainsAny(const string& str, const string& characters)
{
    return ContainsAny(str, characters.c_str());
}

bool StringUtils::ContainsAny(const string& str, const char* characters)
{
    if (str.empty() || strlen(characters) == 0)
        return false;

    for (size_t x = 0; x < strlen(characters); ++x)
    {
        if (str.find(characters[x]) == string::npos)
            return true;
    }

    return false;
}

int StringUtils::CountOccurrences(const string& str, const char search)
{
    auto count = 0;

    for (auto i = 0; i < static_cast<int>(str.size()); ++i)
    {
        if (str[i] == search)
            ++count;
    }

    return count;
}

int StringUtils::CountOccurrences(const string& str, const string& search)
{
    auto count = 0;

    for (auto i = 0; i < static_cast<int>(str.size()); ++i)
    {
        const string source = str.substr(i, search.size());

        if (source == search)
            ++count;
    }

    return count;
}

string StringUtils::RemoveAny(const string& str, const string& characters)
{
    if (str.empty() || characters.empty())
        return str;

    ostringstream result;

    for (const auto ch : str)
    {
        if (characters.find(ch) != string::npos)
            continue;

        result << ch;
    }

    return result.str();
}

string StringUtils::RemoveAny(const string& str, const char* characters)
{
    return RemoveAny(str, string(characters));
}

string StringUtils::RemoveAnyExcept(const string& str, const string& characters)
{
    if (str.empty() || characters.empty())
        return str;

    ostringstream result;

    for (const auto ch : str)
    {
        const auto pos = characters.find(ch);
        if (pos == string::npos)
            continue;

        result << ch;
    }

    return result.str();
}

string StringUtils::RemoveAnyExcept(const string& str, const char* characters)
{
    return RemoveAnyExcept(str, string(characters));
}

bool StringUtils::StartsWith(const string& str, const string& prefix)
{
    if (str.length() < prefix.length())
        return false;

    return Left(str, prefix.length()) == prefix;
}

bool StringUtils::EndsWith(const string& str, const string& suffix)
{
    if (str.length() < suffix.length())
        return false;

    return Right(str, suffix.length()) == suffix;
}

bool StringUtils::StartsAndEndsWith(const string& str, const string& prefixAndSuffix)
{
    return StartsWith(str, prefixAndSuffix) && EndsWith(str, prefixAndSuffix);
}

string StringUtils::EnsureStartsWith(const string& str, const string& prefix)
{
    if (prefix.empty() || str.length() < prefix.length())
        return str;

    auto text = StartsWith(str, prefix)
        ? str
        : prefix + str;

    return text;
}

string StringUtils::EnsureEndsWith(const string& str, const string& suffix)
{
    if (suffix.empty() || str.length() < suffix.length())
        return str;

    auto text = EndsWith(str, suffix)
        ? str
        : str + suffix;

    return text;
}

string StringUtils::EnsureStartsAndEndsWith(const string& str, const string& prefixAndSuffix)
{
    return EnsureStartsAndEndsWith(str, prefixAndSuffix, prefixAndSuffix);
}

string StringUtils::EnsureStartsAndEndsWith(const string& str, const string& prefix, const string& suffix)
{
    return EnsureStartsWith(EnsureEndsWith(str, suffix), prefix);
}

string StringUtils::RemoveStartsWith(const string& str, const string& prefix, int count)
{
    if (str.length() < prefix.length())
        return str;

    string trimmedStr = str;
    while (StartsWith(trimmedStr, prefix) && count != 0)
    {
        trimmedStr = trimmedStr == prefix
            ? ""
            : trimmedStr.substr(prefix.length(), string::npos);

        if (count > 0)
            --count;
    }

    return trimmedStr;
}

string StringUtils::RemoveEndsWith(const string& str, const string& suffix, int count)
{
    if (str.length() < suffix.length())
        return str;

    string trimmedStr = str;
    while (EndsWith(trimmedStr, suffix) && count != 0)
    {
        trimmedStr = trimmedStr == suffix
            ? ""
            : trimmedStr.substr(0, trimmedStr.length() - suffix.length());

        if (count > 0)
            --count;
    }

    return trimmedStr;
}

string StringUtils::RemoveStartsAndEndsWith(const string& str, const string& prefixAndSuffix, int count)
{
    return RemoveStartsAndEndsWith(str, prefixAndSuffix, prefixAndSuffix);
}

string StringUtils::RemoveStartsAndEndsWith(const string& str, const string& prefix, const string& suffix, int count)
{
    return RemoveStartsWith(RemoveEndsWith(str, suffix), prefix);
}

list<string> StringUtils::SeparateByLineEndings(const string& text)
{
    const string& possibleLineEndingCharacters = "\n\r";

    auto lines = list<string>();

    string remaining = ReplaceString(text, "\r\n", Left(possibleLineEndingCharacters, 1));

    if (!remaining.empty())
    {
        while (true)
        {
            if (remaining.empty())
            {
                lines.push_back(remaining);
                break;
            }

            auto found = remaining.find_first_of(possibleLineEndingCharacters);
            if (found == string::npos)
            {
                lines.push_back(remaining);
                break;
            }

            lines.push_back(Left(remaining, found));
            remaining = Right(remaining, remaining.length() - found - 1);
        }
    }

    return lines;
}

string StringUtils::NormalizeLineEndings(const string& text, const string& desiredLineEndings)
{
    const auto lines = SeparateByLineEndings(text);

    return JoinText(lines, desiredLineEndings);
}
