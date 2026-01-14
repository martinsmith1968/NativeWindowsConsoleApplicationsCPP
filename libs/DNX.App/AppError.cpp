#include "stdafx.h"
#include "AppError.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyModernizePassByValue
// ReSharper disable CppClangTidyMiscUseInternalLinkage

using namespace std;
using namespace DNX::App;

AppError AppError::_empty_error = AppError();

AppError::AppError(): AppError("", 0)
{
}

AppError::AppError(const string& description, const int return_code)
    : _description(description)
    , _return_code(return_code)
{
}

bool AppError::IsEmpty() const
{
    return _return_code == 0 && _description.empty();
}

string AppError::GetDescription() const
{
    return _description;
}

int AppError::GetReturnCode() const
{
    return _return_code;
}

bool AppError::HasDescription() const
{
    return !_description.empty();
}

bool AppError::HasReturnCode() const
{
    return _return_code != 0;
}

AppError& AppError::Empty()
{
    return _empty_error;
}
