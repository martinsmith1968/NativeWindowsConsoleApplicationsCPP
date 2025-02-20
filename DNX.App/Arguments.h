#pragma once
#include "stdafx.h"
#include "../DNX.Utils/FileUtils.h"
#include "ArgumentType.h"
#include "Argument.h"
#include "ParserContext.h"
#include "ValueConverter.h"
#include <list>
#include <map>
#include <string>


// ReSharper disable CppInconsistentNaming
// ReSharper disable CppClangTidyCppcoreguidelinesAvoidConstOrRefDataMembers
// ReSharper disable CppClangTidyCppcoreguidelinesSpecialMemberFunctions
// ReSharper disable CppClangTidyClangDiagnosticHeaderHygiene

using namespace std;
using namespace DNX::Utils;

namespace DNX::App
{
    //--------------------------------------------------------------------------
    // Class: Arguments
    //--------------------------------------------------------------------------
    class Arguments
    {
        const string HelpShortName                    = "?";
        const string UseDefaultArgumentsFileShortName = "@";
        const string UseLocalArgumentsFileShortName   = "$";

        const string HelpLongName                    = "help";
        const string UseDefaultArgumentsFileLongName = "use-default-arguments-file";
        const string UseLocalArgumentsFileLongName   = "use-local-arguments-file";

        const string HelpDescription             = "Show Help screen";
        string GetUseDefaultArgumentsFileDesc()
        {
            return "Use Default Arguments File (" + FileUtils::GetFileNameAndExtension(AppDetails::GetDefaultArgumentsFileName()) + ")";
        }
        string GetUseLocalArgumentsFileDesc()
        {
            return "Use Local Arguments File (" + FileUtils::GetFileNameAndExtension(AppDetails::GetDefaultArgumentsFileName()) + ")";
        }

        int _last_position = 0;
        map<string, Argument> _arguments{};
        map<string, string> _values{};
        list<string> _notes{};
        list<string> _errors{};

        void AddArgumentComplete(
            ArgumentType argumentType,
            ValueType valueType,
            const string& shortName,
            const string& longName,
            const string& defaultValue,
            const string& description,
            bool required,
            int position,
            const list<string>& valueList
        );

    protected:
        ArgumentTypeTextResolver ArgumentTypeText;

        void AddStandardArguments();
        void AddFileOverrideArguments();
        void virtual PostParseValidate();

        void AddNote(const string& note);

        void AddArgument(
            ArgumentType argumentType,
            ValueType valueType,
            const string& shortName,
            const string& longName = "",
            const string& defaultValue = "",
            const string& description = "",
            bool required = false,
            int position = 0,
            const list<string>& valueList = list<string>()
        );

        void AddParameter(
            const ValueType valueType,
            int position,
            const string& longName = "",
            const string& defaultValue = "",
            const string& description = "",
            bool required = true,
            const list<string>& valueList = list<string>()
        );
        void AddOption(
            const ValueType valueType,
            const string& shortName,
            const string& longName = "",
            const string& defaultValue = "",
            const string& description = "",
            bool required = false,
            int position = 0,
            const list<string>& valueList = list<string>()
        );
        void AddSwitch(
            const string& shortName,
            const string& longName = "",
            const string& defaultValue = "",
            const string& description = "",
            bool required = false,
            int position = 0
        );

        void AddError(const string& text);

        Argument& GetArgumentByLongName(const string& longName);
        Argument& GetArgumentByShortName(const string& shortName);
        Argument& GetArgumentByName(const string& name);
        [[nodiscard]] Argument& GetParameterAtPosition(const int position);

        [[nodiscard]] list<Argument> GetRequiredArguments() const;

        string GetArgumentValue(const string& name);
        bool GetSwitchValue(const string& name);
        void SetArgumentValue(const string& name, const string& value);
        bool HasArgumentValue(const string& name);

        [[nodiscard]] int GetNextPosition() const;
        void AdvancePosition();

        friend class ArgumentsParser;
        friend class Commands;
        friend class CommandsParser;

    public:
        Arguments();
        virtual ~Arguments() = default;

        [[nodiscard]] bool IsEmpty() const;
        void Reset();

        static Arguments _empty_arguments;
        static Arguments& Empty() { return _empty_arguments; }

        [[nodiscard]] list<Argument> GetArguments() const;
        [[nodiscard]] list<Argument> GetArgumentsByType(ArgumentType ArgumentType) const;
        [[nodiscard]] list<Argument> GetArgumentsByTypes(const list<ArgumentType>& ArgumentTypes) const;

        [[nodiscard]] list<string> GetErrors() const;
        [[nodiscard]] list<string> GetNotes() const;
        [[nodiscard]] bool IsValid() const;
        bool IsHelp();
        bool IsUsingDefaultArgumentsFile();

    };
}
