#pragma once

#include "stdafx.h"
#include <list>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace DNX::Utils
{
    //--------------------------------------------------------------------------
    // Class: StringUtils
    //--------------------------------------------------------------------------
    class StringUtils
    {
    public:
        static wstring ToWideString(string& str);
        static string ToString(wstring& wstr);

        static string Trim(const string& str, const char removeChar = ' ');
        static string LTrim(const string& str, const char removeChar = ' ');
        static string RTrim(const string& str, const char removeChar = ' ');

        static string Trim(const string& str, const string& prefix_suffix);
        static string LTrim(const string& str, const string& prefix);
        static string RTrim(const string& str, const string& suffix);

        static string LPad(const string& str, size_t min_length, const char prefix = ' ');
        static string RPad(const string& str, size_t min_length, const char prefix = ' ');

        static string Before(const string& str, const string& find);
        static string After(const string& str, const string& find);
        static string BeforeLast(const string& str, const string& find);
        static string AfterLast(const string& str, const string& find);

        static string Between(const string& str, const string& first, const string& second);
        static string BetweenInner(const string& str, const string& first, const string& second);

        static string ReplaceString(string subject, const string& search, const string& replace);
        static string Repeat(const string& subject, const int count);

        static string ToLower(const string& text);
        static string ToUpper(const string& text);

        static string Left(const string& text, const size_t length);
        static string Right(const string& text, const size_t length);

        static string BoolToString(const bool value, const string& trueValue = "true", const string& falseValue = "false");

        static list<string> SplitText(const string& str, const char splitChar = ',', const char trimChar = NULL);
        static string JoinText(const list<string>& list, const string& delimiter);

        static bool Contains(const string& str, const string& search);

        static bool ContainsOnly(const string& str, const string& characters);
        static bool ContainsOnly(const string& str, const char* characters);

        static bool ContainsAny(const string& str, const string& characters);
        static bool ContainsAny(const string& str, const char* characters);

        static int CountOccurrences(const string& str, const char search);
        static int CountOccurrences(const string& str, const string& search);

        static string RemoveAny(const string& str, const string& characters);
        static string RemoveAny(const string& str, const char* characters);

        static string RemoveAnyExcept(const string& str, const string& characters);
        static string RemoveAnyExcept(const string& str, const char* characters);

        static bool StartsWith(const string& str, const string& prefix);
        static bool EndsWith(const string& str, const string& suffix);
        static bool StartsAndEndsWith(const string& str, const string& prefixAndSuffix);

        static string EnsureStartsWith(const string& str, const string& prefix);
        static string EnsureEndsWith(const string& str, const string& suffix);
        static string EnsureStartsAndEndsWith(const string& str, const string& prefixAndSuffix);
        static string EnsureStartsAndEndsWith(const string& str, const string& prefix, const string& suffix);

        static string RemoveStartsWith(const string& str, const string& prefix, int count = -1);
        static string RemoveEndsWith(const string& str, const string& suffix, int count = -1);
        static string RemoveStartsAndEndsWith(const string& str, const string& prefixAndSuffix, int count = -1);
        static string RemoveStartsAndEndsWith(const string& str, const string& prefix, const string& suffix, int count = -1);
    };
}
