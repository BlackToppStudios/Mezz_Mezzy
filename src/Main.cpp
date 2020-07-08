// © Copyright 2010 - 2019 BlackTopp Studios Inc.
/* This file is part of The UTF8 Conversion Library.

    The UTF8 Conversion Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The UTF8 Conversion Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The UTF8 Conversion Library.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'doc' folder. See 'gpl.txt'
*/
/* We welcome the use of the UTF8 Conversion Library to anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/

/// @file
/// @brief The main entry point and dispatch table for the mezzy tool.

#include "CommandLineCallingTable.h"

#include "CommandLine.h"
#include "RepoLookup.h"

#include <iostream>

namespace  {
    Mezzanine::Mezzy::CommandLineCallingTable CallingTable;
}

void AddCallingTableEntries();

Mezzanine::ExitCode main(int ArgCount, char** Arguments)
{
    // Parse command line arguments
    Mezzanine::CommandLineArguments ParsedArgs(ArgCount, Arguments);

    // Add each Argument
    AddCallingTableEntries();

    // The Help visitor scans the existing arguments to generate help
    Mezzanine::Mezzy::CommandLineCallingTableVisitorHelpMessager
            Helper(ParsedArgs.ExecutableCommand, CallingTable);

    // Bail if no args
    if(0 == ParsedArgs.Arguments.size())
    {
        std::cerr << "No arguments: " << std::endl
                  << Helper.GetDefaultHelpMessage() << std::endl;
        return EXIT_FAILURE;
    }

    // Bail on bad entries.
    for(const auto& Arg : ParsedArgs.Arguments)
    {
        if(!CallingTable.EntryExists(Arg.first))
        {
            std::cerr << "Unknown argument: " << Arg.first << std::endl
                      << Helper.GetDefaultHelpMessage() << std::endl;
            return EXIT_FAILURE;
        }
    }

    // If all entries exist invoke them.
    for(const auto& Arg : ParsedArgs.Arguments)
        { CallingTable.ExecuteEntry(Arg.first, Arg.second); }


    return EXIT_SUCCESS;
    // if they don't


    ////////////////////////////////////
    //TODO

    // Tool to get keys from CMakeList.txt

    // Way to create Fresh CMakelists.txt0

    // Add empty Test/Header/Source triplet
    //      modify CMakeLists*

    // Add empty Main
    //      and add to CMakeLists.txt

    // Way to create one whole package

    // run command in each repo
    //      run updates in each repo.
    //      do git status in each repo.

    // run command in each build dirf

    // Query for the most recent version of the Jagati
    //      get hash
    //      get version
    //      have a way to put both in file



    return EXIT_SUCCESS;
}

void AddCallingTableEntries()
{
    CallingTable.AddTableEntry(
        "-p",  "--package-dir",
        "Print The Mezzanine Package Directory",
        "This prints the location in the enviornment variable '" +
            Mezzanine::Mezzy::GetMezzaninePathVar() + "' ",
        Mezzanine::Mezzy::GetMezzaninePathHandler);

    CallingTable.AddTableEntry(
        "-j",  "--jagati-dir",
        "Print The directory of the Jagati metapackage package",
        "Based on the Mezzanine Package Directory environment variable this prints location of the Jagati directory. "
            "This won't exist on every system, only people developing the Jagati need it.",
        Mezzanine::Mezzy::GetMezzaninePathHandler);



}
