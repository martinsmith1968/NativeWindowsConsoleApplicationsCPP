#pragma once
#include <string>

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyModernizePassByValue

using namespace std;

namespace DNX::App
{
    //--------------------------------------------------------------------------
    // Class: AppError
    //--------------------------------------------------------------------------
    class AppError
    {
        static AppError _empty_error;

        string _description;
        int _return_code;

    public:
        AppError();
        explicit AppError(const string& description, const int return_code = 0);

        [[nodiscard]] bool IsEmpty() const;

        [[nodiscard]] string GetDescription() const;
        [[nodiscard]] int GetReturnCode() const;

        [[nodiscard]] bool HasDescription() const;
        [[nodiscard]] bool HasReturnCode() const;

        static AppError& Empty();
    };
}
