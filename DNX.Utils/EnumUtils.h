#pragma once

#include "stdafx.h"
#include <list>
#include <map>
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;

namespace DNX::Utils::EnumUtils
{
    // TODO: Introduce text categories by value of T
    template <class T>
    class EnumTextResolver
    {
        map<T, string> _map;

    protected:
        void SetText(const T value, const string& text)
        {
            _map[value] = text;
        }

    public:
        string GetText(const T value) const
        {
            static string empty_result;

            for (auto iter = _map.begin(); iter != _map.end(); ++iter)
            {
                if (iter->first == value)
                {
                    return iter->second;
                }
            }
            return empty_result;
        }

        T GetValue(const string& text) const
        {
            for (auto iter = _map.begin(); iter != _map.end(); ++iter)
            {
                if (iter->second == text)
                {
                    return iter->first;
                }
            }
            return T();
        }

        [[nodiscard]] list<string> GetAllText() const
        {
            list<string> list;

            for (auto iter = _map.begin(); iter != _map.end(); ++iter)
            {
                list.push_back(iter->second);
            }

            return list;
        }

        list<T> GetAllValues() const
        {
            list<T> list;

            for (auto iter = _map.begin(); iter != _map.end(); ++iter)
            {
                list.push_back(iter->first);
            }

            return list;
        }
    };
}
