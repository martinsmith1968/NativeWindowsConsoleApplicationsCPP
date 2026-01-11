#include "pch.h"

#include "TestHelper.h"
#include <iostream>

using namespace std;

// ReSharper disable CppInconsistentNaming

void TestHelper::ShowErrors(const Arguments& arguments)
{
    cout << "Error Count: " << arguments.GetErrors().size() << endl;
    for (const auto& error : arguments.GetErrors())
    {
        cout << "Error: " << error << endl;
    }
}

void TestHelper::ShowErrors(const Commands& commands)
{
    cout << "Error Count: " << commands.GetErrors().size() << endl;
    for (const auto& error : commands.GetErrors())
    {
        cout << "Error: " << error << endl;
    }
}

void TestHelper::ShowErrors(const Command& command)
{
    ShowErrors(command.GetArguments());
}
