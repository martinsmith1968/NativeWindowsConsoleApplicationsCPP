#pragma once

#include "stdafx.h"
#include <list>
#include <string>
#include "MathUtils.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace DNX::Utils
{
    //--------------------------------------------------------------------------
    // Class: ListUtils
    //--------------------------------------------------------------------------
    class ListUtils
    {
    public:
        static list<string> ToList(int argc, char* argv[], const int startAt = 0);

        template<class T>
        static T& GetAt(list<T>& list, const int index)
        {
            static T default_return_value;

            if (index < 0 || index >= list.size())
                return default_return_value;

            auto iter = list.begin();

            std::advance(iter, index);

            return *iter;
        }

        template<class T>
        static bool Exists(const list<T>& list, const T value)
        {
            for (auto iter = list.begin(); iter != list.end(); ++iter)
            {
                if (*iter == value)
                {
                    return true;
                }
            }

            return false;
        }

        static string Find(const list<string>& list, const string& value, bool case_sensitive = true);

        template<class T>
        static T& GetRandom(list<T>& list)
        {
            const auto index = MathUtils::GetRandomNumber(0, static_cast<int>(list.size()), false);

            T& result = GetAt(list, index);

            return result;
        }
    };
}
