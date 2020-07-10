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
/// @brief This is the very simple innards of the Calling table a simple IOC for commaand line args.

// Mezzy Headers
#include "CommandLineCallingTable.h"

#include <iostream>
#include <exception>

namespace Mezzanine {
namespace Mezzy {

void CommandLineCallingTable::ValidateShortKeyForAddition(const CommandLineCallingTable::KeyType& ShortKey)
{
    if(!IsShortKey(ShortKey))
        { throw std::runtime_error("Invalid short key '" + ShortKey + "' cannot be added."); }
    if(ShortEntryExists(ShortKey))
        { throw std::runtime_error("Short argument handler '" + ShortKey + "' already exists."); }
}

void CommandLineCallingTable::ValidateLongKeyForAddition(const CommandLineCallingTable::KeyType& LongKey)
{
    if(!IsLongKey(LongKey))
        { throw std::runtime_error("Invalid long key '" + LongKey + "' cannot be added."); }
    if(LongEntryExists(LongKey))
        { throw std::runtime_error("Long argument handler '" + LongKey + "' already exists."); }
}

void CommandLineCallingTable::ValidateKeysForAddition(const CommandLineCallingTable::KeyType& ShortKey,
                                                      const CommandLineCallingTable::KeyType& LongKey)
{
    ValidateShortKeyForAddition(ShortKey);
    ValidateLongKeyForAddition(LongKey);
}

Mezzanine::Boole CommandLineCallingTable::IsShortKey(const CommandLineCallingTable::KeyType& Key)
{
    if(Key.size() != 2)
        { return false; }
    return (ArgToken == Key[0]) && (ArgToken != Key[1]);
}

Mezzanine::Boole CommandLineCallingTable::IsLongKey(const CommandLineCallingTable::KeyType& Key)
{
    if(Key.size() < 3)
        { return false; }
    return (ArgToken == Key[0]) && (ArgToken == Key[1]) && (ArgToken != Key[2]);
}

Mezzanine::Boole CommandLineCallingTable::isValidKey(const CommandLineCallingTable::KeyType& Key)
    { return IsLongKey(Key) || IsShortKey(Key); }

Boole CommandLineCallingTable::ShortEntryExists(const CommandLineCallingTable::KeyType& Key) const
    { return ShortCallingTable.end() != ShortCallingTable.find(Key); }

Boole CommandLineCallingTable::LongEntryExists(const CommandLineCallingTable::KeyType& Key) const
    { return LongCallingTable.end() != LongCallingTable.find(Key); }

Boole CommandLineCallingTable::EntryExists(const CommandLineCallingTable::KeyType& Key) const
{
    if(IsShortKey(Key))
        { return ShortEntryExists(Key); }

    if(IsLongKey(Key))
        { return LongEntryExists(Key); }

    return false;
}

void CommandLineCallingTable::AddTableEntry(const KeyType& ShortKey,
                                            const KeyType& LongKey,
                                            const HelpMessageType& BriefMessage,
                                            const HelpMessageType& HelpMessage,
                                            ArgumentHandlerType ArgumentHandler)
{
    ValidateKeysForAddition(ShortKey, LongKey);

    ShortCallingTable[ShortKey] = LongKey;
    ShortKeyReverseLookupTable[LongKey] = ShortKey;

    AddTableEntry(LongKey, BriefMessage, HelpMessage, ArgumentHandler);
}

void CommandLineCallingTable::AddTableEntry(const KeyType& LongKey,
                                            const HelpMessageType& BriefMessage,
                                            const HelpMessageType& HelpMessage,
                                            ArgumentHandlerType ArgumentHandler)
{
    ValidateLongKeyForAddition(LongKey);

    LongCallingTable[LongKey] = ArgumentHandler;
    LongArgToBriefTable[LongKey] = BriefMessage;
    LongArgToHelpMessageTable[LongKey] = HelpMessage;
}


void CommandLineCallingTable::ExecuteEntry(const CommandLineCallingTable::KeyType& KeyToRun,
                                           const CommandLineCallingTable::ArgVectorType& Args)
{
    KeyType CleanedKey = KeyToRun;
    if(IsShortKey(KeyToRun))
    {
        if(ShortEntryExists(KeyToRun))
            { CleanedKey = ShortCallingTable[KeyToRun]; }
        else
            { throw std::runtime_error("Short argument handler '" + KeyToRun + "' does not exist."); }
    }
    else if(IsLongKey(CleanedKey))
    {
        if(!LongEntryExists(CleanedKey))
            { throw std::runtime_error("Long argument handler '" + CleanedKey + "' does not exist."); }

    }
    else
        { throw std::runtime_error("Invalid argument handler '" + CleanedKey + "'."); }

    LongCallingTable[CleanedKey](Args);
}


CommandLineCallingTableVisitorHelpMessager::CommandLineCallingTableVisitorHelpMessager(
    String ExecutableName,
    CommandLineCallingTable& TargetCallingTable
    ) : Name(CleanExecutableName(ExecutableName)), CallingTable(TargetCallingTable)
{}

String CommandLineCallingTableVisitorHelpMessager::CleanExecutableName(const String& RawName) const
{
    String::size_type LastSlash = RawName.find_last_of('\\');

    if(String::npos == LastSlash)
        { LastSlash = RawName.find_last_of('/'); }

    if(String::npos == LastSlash)
        { return RawName; }
    else
    {
        if(RawName.size() > LastSlash+1)
            { return RawName.substr(LastSlash+1);  }
        else
            { return RawName; }
    }
}

String CommandLineCallingTableVisitorHelpMessager::GetName() const
    { return Name; }

String CommandLineCallingTableVisitorHelpMessager::GetHeading()
{
    if(Heading.empty())
        { CallingTable.ForEachEntry(*this); }
    return Heading;
}

String CommandLineCallingTableVisitorHelpMessager::GetBriefTable()
{
    if(BriefTable.empty())
        { CallingTable.ForEachEntry(*this); }
    return BriefTable;
}

String CommandLineCallingTableVisitorHelpMessager::GetHelpTable()
{
    if(HelpTable.empty())
        { CallingTable.ForEachEntry(*this); }
    return HelpTable;
}

String CommandLineCallingTableVisitorHelpMessager::GetDefaultHelpMessage()
{
    return "\nUsage:\n  " + GetName()  + GetHeading()
            + "\n\nArgument Summary:" + GetBriefTable()
            + "\n\nArgument Details:" + GetHelpTable()
            + "\n";
}


void CommandLineCallingTableVisitorHelpMessager::operator()(
    const CommandLineCallingTable::CommandLineCallingTableRecord& Handler)
{
    const String Keys( (Handler.ShortKey.size()) ? (Handler.ShortKey+" | "+Handler.LongKey) : (Handler.LongKey) );
    const Int32 LeftMargin{40};

    Heading += " [ " + Keys + " ]";

    const String Prefix = "\n  " + Keys + String(LeftMargin - 3 - Keys.size(), ' ') + " ";

    BriefTable += Prefix + Handler.BriefHelp;

    HelpTable += Prefix + Handler.LongHelp +"\n";
}





} // Namepspaces Mezzy/Mezzanine
}


