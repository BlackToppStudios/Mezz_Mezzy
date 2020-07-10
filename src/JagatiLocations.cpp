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
/// @brief All of the rules and policy for interacting with env vars.

// Other Mezzanine Headers
#include "PathUtilities.h"

// Mezzy Headers
#include "DataTypes.h"
#include "EnvironmentVars.h"
#include "JagatiLocations.h"


namespace Mezzanine {
namespace Mezzy {

using Mezzanine::Mezzy::GetMezzaninePath;
using Mezzanine::Filesystem::IsDirectorySeparator;
using Mezzanine::Filesystem::GetDirectorySeparator_Host;

namespace {
    SAVE_WARNING_STATE
    SUPPRESS_CLANG_WARNING("-Wexit-time-destructors")
    SUPPRESS_CLANG_WARNING("-Wglobal-constructors")

    const String JagatiDirSuffix = "Jagati";
    const String JagatiCMakeSuffix = JagatiDirSuffix + GetDirectorySeparator_Host() + "Jagati.cmake";
    const String JagatiPackageIndexSuffix = JagatiDirSuffix + GetDirectorySeparator_Host() + "JagatiIndex.cmake";

    RESTORE_WARNING_STATE

    // Does the same basic attaching of suffixes to the Mezzanine path.
    String GetMezzaninePathWithSuffix(String Suffix)
    {
        String results{GetMezzaninePath()};

        if(results.empty())
            { return ""; }

        const String::value_type LastChar{ results[results.size()-1] };
        if(IsDirectorySeparator(LastChar))
            { return GetMezzaninePath() + Suffix; }
        else
            { return GetMezzaninePath() + GetDirectorySeparator_Host() + Suffix; }
    }
}

String GetJagatiDir()
    { return GetMezzaninePathWithSuffix(JagatiDirSuffix); }

String GetJagatiCMakeFile()
    { return GetMezzaninePathWithSuffix(JagatiCMakeSuffix); }

String GetJagatiPackageListFile()
    { return GetMezzaninePathWithSuffix(JagatiPackageIndexSuffix); }



} // Namepspaces Mezzy/Mezzanine
}

