#pragma once

#include "stdafx.h"
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// ReSharper disable CppClangTidyPerformanceAvoidEndl
// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClassCanBeFinal

using namespace std;

// Source : https://stackoverflow.com/questions/53164946/c-approach-to-handling-a-collection-of-multiple-exceptions
class AggregateException : public exception
{
    vector<exception> _exceptions;
    string _whatMessage;

public:
    explicit AggregateException(vector<exception> exceptions)
    {
        _exceptions = std::move(exceptions);

        stringstream what_string;
        what_string << "AggregateException: (" << _exceptions.size() << ")" << endl;
        for (auto const& ex : _exceptions) {
            what_string << " " << ex.what() << endl;
        }
        _whatMessage = what_string.str();
    }

    [[nodiscard]] const char* what() const noexcept override {
        return _whatMessage.c_str();
    }

    [[nodiscard]] vector<exception> GetExceptions() const
    {
        return _exceptions;
    }
};
