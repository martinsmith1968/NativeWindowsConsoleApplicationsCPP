#pragma once

#include "gtest/gtest.h"
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <list>
#include <regex>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppClangTidyCppcoreguidelinesSpecialMemberFunctions

using namespace std;

#define TEST_STDOUT(target) TEST_STDOUT(target, , )

#define TEST_STDOUT(target, before_act, after_act) \
    using namespace DNX::GoogleTest::Utils; \
    /* Arrange */\
    string test_class_name           = ::testing::UnitTest::GetInstance()->current_test_info()->test_case_name(); \
    string test_name                 = ::testing::UnitTest::GetInstance()->current_test_info()->name(); \
    string directory_name            = "expected\\" + test_class_name + "\\"; \
    string test_output_file_name     = directory_name + test_name + ".testout"; \
    string expected_output_file_name = directory_name + test_name + ".expected"; \
    \
    /* Act */\
    before_act; \
    { \
        auto redirect                = StdOutRedirect(test_output_file_name); \
        target; \
    } \
    after_act; \
    \
    /* Assert */\
    CompareFileContents(test_output_file_name, expected_output_file_name);

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

            m_output = std::ofstream(m_file_name);
            m_old_output = std::cout.rdbuf();
            std::cout.rdbuf(m_output.rdbuf()); //redirect std::cout to out.txt!
        }

        ~StdOutRedirect()
        {
            std::cout.rdbuf(m_old_output); //reset to standard output again
        }
    };

    static list<string> GetFileLines(const string& file_name)
    {
        list<string> lines;

        ifstream file(file_name);
        string str;

        while (std::getline(file, str))
        {
            lines.emplace_back(str);
        }

        file.close();

        return lines;
    }

    static void CompareFileContents(const string& test_output_file_name, const string& expected_output_file_name)
    {
        EXPECT_TRUE(std::filesystem::exists(test_output_file_name)) << test_output_file_name + " not exist";
        EXPECT_TRUE(std::filesystem::exists(expected_output_file_name)) << expected_output_file_name + " not exist";

        const auto test_output_lines     = GetFileLines(test_output_file_name);
        const auto expected_output_lines = GetFileLines(expected_output_file_name);

        EXPECT_EQ(test_output_lines.size(), expected_output_lines.size()) << " files should have the same number of lines";
        EXPECT_EQ(test_output_lines, expected_output_lines) << " file contents should be comparable";
    }
}
