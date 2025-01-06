#pragma once

#include <map>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppUseAssociativeContains

using namespace std;

namespace DNX::Utils
{
    //--------------------------------------------------------------------------
    // Class: MapUtils
    //--------------------------------------------------------------------------
    class MapUtils
    {
    public:
        template<class T, class V>
        static bool Exists(const map<T, V>& map, const T key)
        {
            return map.find(key) != map.end();
        }

        //template<class T, class V>
        //static list<T> GetKeys(const map<T, V>& map)
        //{
        //    list<T> items;
        //    for (auto kvp : map)
        //        items.emplace_back(kvp.first);
        //    return items;
        //}
        //
        //template<class T, class V>
        //static list<V> GetValues(const map<T, V>& map)
        //{
        //    list<V> items;
        //    for (auto kvp : map)
        //        items.emplace_back(kvp.second);
        //    return items;
        //}
    };
}
