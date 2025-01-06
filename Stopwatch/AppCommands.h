#pragma once
#include "stdafx.h"
#include "../DNX.App/Commands.h"
#include "Commands/CancelCommand.h"
#include "Commands/ElapsedCommand.h"
#include "Commands/ListCommand.h"
#include "Commands/PauseCommand.h"
#include "Commands/ResumeCommand.h"
#include "Commands/StartCommand.h"
#include "Commands/StopCommand.h"

// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyModernizeUseEqualsDefault
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene
// ReSharper disable CppTooWideScopeInitStatement
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers

using namespace std;
using namespace DNX::Utils;

namespace Stopwatch
{
    //------------------------------------------------------------------------------
    // Commands
    class AppCommands final : public Commands
    {
        ListArguments  _list_arguments;

        ListCommand m_list_command;
        StartCommand m_start_command;
        StopCommand m_stop_command;
        ElapsedCommand m_elapsed_command;
        PauseCommand m_pause_command;
        ResumeCommand m_resume_command;
        CancelCommand m_cancel_command;

    public:
        AppCommands()
        {
            AddCommand(m_list_command);
            AddCommand(m_start_command);
            AddCommand(m_stop_command);
            AddCommand(m_elapsed_command);
            AddCommand(m_pause_command);
            AddCommand(m_resume_command);
            AddCommand(m_cancel_command);
        }

        void Execute(const Command& command)
        {
            // NOTE: Necessary until rework for Parse to return a pointer rather than a reference
            //       Then we *should* be able to call command->Execute() from main and let the vtable execute the correct instance method
            //       (Currently doesn't work with references, but at least it's all encapsulated here)
            if (command.GetName() == m_list_command.GetName())
            {
                m_list_command.Execute();
            }
            else if (command.GetName() == m_start_command.GetName())
            {
                m_start_command.Execute();
            }
            else if (command.GetName() == m_stop_command.GetName())
            {
                m_stop_command.Execute();
            }
            else if (command.GetName() == m_elapsed_command.GetName())
            {
                m_elapsed_command.Execute();
            }
            else if (command.GetName() == m_pause_command.GetName())
            {
                m_pause_command.Execute();
            }
            else if (command.GetName() == m_resume_command.GetName())
            {
                m_resume_command.Execute();
            }
            else if (command.GetName() == m_cancel_command.GetName())
            {
                m_cancel_command.Execute();
            }
            else
            {
                const string message = "Invalid or Unsupported Command Target: " + command.GetName();
                throw exception(message.c_str());
            }
        }
    };
}
