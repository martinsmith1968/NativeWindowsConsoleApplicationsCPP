#pragma once

#include "gtest/gtest.h"
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <regex>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppClangTidyCppcoreguidelinesSpecialMemberFunctions

using namespace std;

namespace DNX::GoogleTest::Utils
{
    // Source : https://stackoverflow.com/questions/10150468/how-to-redirect-cin-and-cout-to-files
    class StdOutRedirect
    {
        string m_file_name;
        ofstream m_output;
        streambuf* m_old_output;

    public:
        explicit StdOutRedirect(const string& file_name)
        {
            m_file_name = file_name;

            m_output = ofstream(m_file_name);
            m_old_output = cout.rdbuf();
            cout.rdbuf(m_output.rdbuf()); //redirect cout to out.txt!
        }

        ~StdOutRedirect()
        {
            cout.rdbuf(m_old_output); //reset to standard output again
        }
    };

    static string GetFilePath(const string& file_name)
    {
        string file_path;
        const size_t last_slash_idx = file_name.rfind('\\');
        if (string::npos != last_slash_idx)
        {
            file_path = file_name.substr(0, last_slash_idx);
        }
        return file_path;
    }

    static list<string> GetFileLines(const string& file_name)
    {
        list<string> lines;

        ifstream file(file_name);
        string str;

        while (getline(file, str))
        {
            lines.emplace_back(str);
        }

        file.close();

        return lines;
    }

    static void CompareFileContents(const string& test_output_file_name, const string& expected_output_file_name)
    {
        EXPECT_TRUE(filesystem::exists(test_output_file_name)) << test_output_file_name + " not exist";
        EXPECT_TRUE(filesystem::exists(expected_output_file_name)) << expected_output_file_name + " not exist";

        const auto test_output_lines     = GetFileLines(test_output_file_name);
        const auto expected_output_lines = GetFileLines(expected_output_file_name);

        EXPECT_EQ(test_output_lines.size(), expected_output_lines.size()) << " files should have the same number of lines";
        EXPECT_EQ(test_output_lines, expected_output_lines) << " file contents should be comparable";
    }
}

#define TEST_STDOUT(target) TEST_STDOUT_SETUP_TEARDOWN(target, , )

#define TEST_STDOUT_SETUP_TEARDOWN(target, before_act, after_act) \
    using namespace DNX::GoogleTest::Utils; \
    /* Arrange */\
    string test_file_name            = ::testing::UnitTest::GetInstance()->current_test_info()->file(); \
    string test_class_name           = ::testing::UnitTest::GetInstance()->current_test_info()->test_case_name(); \
    string test_name                 = ::testing::UnitTest::GetInstance()->current_test_info()->name(); \
    string test_file_root_path       = GetFilePath(test_file_name); \
    string base_path_name            = test_file_root_path + "\\expected\\" + test_class_name + "\\"; \
    string test_output_file_name     = base_path_name + test_name + ".testout"; \
    string expected_output_file_name = base_path_name + test_name + ".expected"; \
    \
    /* Act */\
    before_act; \
    { \
        auto stdout_redirect = StdOutRedirect(test_output_file_name); \
        target; \
    } \
    after_act; \
    \
    /* Assert */\
    CompareFileContents(test_output_file_name, expected_output_file_name);
