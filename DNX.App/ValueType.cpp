#include "stdafx.h"
#include "ValueType.h"

using namespace DNX::App;

ValueTypeTextResolver::ValueTypeTextResolver()
{
    SetText(ValueType::CHAR, "Char");
    SetText(ValueType::STRING, "Text");
    SetText(ValueType::BOOL, "+/-");
    SetText(ValueType::INT, "Integer");
    SetText(ValueType::DOUBLE, "Number");
    SetText(ValueType::ENUM, "Value");
    SetText(ValueType::DATE, "Date");
    SetText(ValueType::DATETIME, "DateTime");
};
