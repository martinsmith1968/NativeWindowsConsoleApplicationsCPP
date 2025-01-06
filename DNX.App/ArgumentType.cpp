#include "stdafx.h"
#include "ArgumentType.h"

// ReSharper disable CppInconsistentNaming

using namespace DNX::App;

ArgumentTypeTextResolver::ArgumentTypeTextResolver()
{
    SetText(ArgumentType::PARAMETER, "Parameter");
    SetText(ArgumentType::OPTION, "Option");
    SetText(ArgumentType::SWITCH, "+/-");
}
