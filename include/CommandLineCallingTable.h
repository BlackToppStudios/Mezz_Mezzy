// © Copyright 2010 - 2019 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
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
#ifndef Mezz_Mezzy_CommandLineParse_h
#define Mezz_Mezzy_CommandLineParse_h


/// @file
/// @brief The interface for parsing command line arguments.

#ifndef SWIG
    // Mezzanine Headers
    #include "DataTypes.h"
    #include "CommandLine.h"

#endif

/// @namespace Mezzy
/// @brief Components that are part of Mezzy's behavior
/// @details The Mezzy Command Line tools are a grab bag of useful-ness around the Mezzanine engine. This are tools for
/// updating packages, bootstrapping new packages and issuing git commands to multiple packages.

namespace Mezzanine {
namespace Mezzy {




/// @brief This is a minimal IOC (Inversion of Control) contrainer for command line arguments.
/// @details This is designed to be a simple way to call functions from string names. Each function can accept a vector
/// of strings as an argument. This closely corresponds to the conventions around passing options on the command line
/// and is intended to be used to ease passing control to command line
/// @n @n
/// Eventuall this container will support long and short forms of parameters, for now entries into this table should
/// simply be added twice.
/// @TODO consider creating - CommandLineCallingTableIterator
class MEZZ_LIB CommandLineCallingTable
{
public:
    /// @brief This uses strings as keys, but that may change as this contains changes.
    using KeyType = Mezzanine::String;

    /// @brief Each Argument handler might have things passed to it, this is the type for that vector of strings.
    using ArgVectorType = Mezzanine::ArgVector;

    /// @brief A function that is mapped onto a string, it should return nothing and accept a vector of strings
    using ArgumentHandlerType = std::function<void(const ArgVectorType&)>;

    /// @brief The type for storing help messages.
    using HelpMessageType = Mezzanine::String;

    /// @brief This is one complete set of data for a Entry in this calling table
    struct MEZZ_LIB CommandLineCallingTableRecord {
        const KeyType& ShortKey;                ///< The single letter arg that might be passed to the executable.
        const KeyType& LongKey;                 ///< An arg that might be passed to the executable of any length.
        const HelpMessageType& BriefHelp;       ///< A short (~40 chars) help message used in a summary.
        const HelpMessageType& LongHelp;        ///< A detailed help message used in the complete help.
        const ArgumentHandlerType& Handler;     ///< A function to be called if either flag is passed.
    };


    /* This is unused functionality, we used a template for now, before this is complete we will
        using VisitorType = std::function<void(const KeyType& ShortKey,
                                               const KeyType& LongKey,
                                               const HelpMessageType& BriefHelp,
                                               const HelpMessageType& LongHelp,
                                               const ArgumentHandlerType& Handler)>;
        using VisitorType = std::function<void(const CommandLineCallingTableRecord&)>;
    */


private:
    using CallingTableType = std::map<KeyType, ArgumentHandlerType>;
    using ShortKeyDelegationTableType = std::map<KeyType, KeyType>;
    using HelpTableType = std::map<KeyType, HelpMessageType>;

    CallingTableType LongCallingTable;
    ShortKeyDelegationTableType ShortCallingTable;
    ShortKeyDelegationTableType ShortKeyReverseLookupTable;
    HelpTableType LongArgToBriefTable;
    HelpTableType LongArgToHelpMessageTable;

    void ValidateLongKeyForAddition(const KeyType& LongKey);
    void ValidateShortKeyForAddition(const KeyType& ShortKey);
    void ValidateKeysForAddition(const KeyType& ShortKey, const KeyType& LongKey);

public:

    /// @brief Is the passed key text a valid string for use as a short key.
    /// @param Key A string that might be a short key
    /// @return True if passed string is a single ArgToken and one other char, "-z"
    static Mezzanine::Boole IsShortKey(const KeyType& Key);
    /// @brief Is the passed key text a valid string for use as a long key.
    /// @param Key A string that might be a long key
    /// @return True if passed string is exactly two ArgToken and at least one other char, "--zip"
    static Mezzanine::Boole IsLongKey(const KeyType& Key);
    /// @brief Is this a long or a short key?
    /// @param Key A string that might be a short or long key
    /// @return True if either IsShortKey or IsLongKey returns true
    static Mezzanine::Boole isValidKey(const KeyType& Key);

    /// @brief Is the given key stored and connected to a valid item to call.
    /// @param Key A Short key that might exist.
    /// @return True If the Short key exists and false if it does not.
    Mezzanine::Boole ShortEntryExists(const KeyType& Key) const;
    /// @brief Is the given key stored and connected to a valid item to call.
    /// @param Key A Long key that might exist.
    /// @return True If the Long key exists and false if it does not.
    Mezzanine::Boole LongEntryExists(const KeyType& Key) const;
    /// @brief Check if a name exists.
    /// @param Key The name to check examples: "-a", "--arg".
    /// @return True if the name exists, false if it doesn't.
    Mezzanine::Boole EntryExists(const KeyType& Key) const;

    /// @brief Add an entry to this calling table with this.
    /// @param ShortKey The short name to add, examples: "-a", "-p".
    /// @param LongKey The long name to add, examples: "--Arg", "--Parameter".
    /// @param BriefMessage Short help message for user display in summaries.
    /// @param HelpMessage A full description of what this does.
    /// @param ArgumentHandler A function to called when the given name is supplied.
    /// @throw std::runtime_error This throws when a duplicate key or invalid ket is attempted to be added and the
    /// state of the calling table is unchanged.
    void AddTableEntry(const KeyType& ShortKey,
                       const KeyType& LongKey,
                       const HelpMessageType& BriefMessage,
                       const HelpMessageType& HelpMessage,
                       ArgumentHandlerType ArgumentHandler);

    /// @brief Add an entry to this calling table with this.
    /// @param LongKey The long name to add, examples: "--Arg", "--Parameter".
    /// @param BriefMessage Short help message for user display in summaries.
    /// @param HelpMessage A full description of what this does.
    /// @param ArgumentHandler A function to called when the given name is supplied.
    /// @throw std::runtime_error This throws when a duplicate key or invalid ket is attempted to be added and the
    /// state of the calling table is unchanged.
    void AddTableEntry(const KeyType& LongKey,
                       const HelpMessageType& BriefMessage,
                       const HelpMessageType& HelpMessage,
                       ArgumentHandlerType ArgumentHandler);

    /// @brief Call the specified name.
    /// @param KeyToRun The name of the function to invoke.
    /// @param Args A vector strings presumably parameters from the command line, defaults to {}.
    /// @throw std::runtime_error This throws when a missing key is called and the state is unchanged.
    void ExecuteEntry(const KeyType& KeyToRun, const ArgVectorType& Args = {});

    /// @brief Call the passed function once for each entry in the calling table.
    /// @tparam VisitorType Normally this, the type of the passed functor, is automatically deduced.
    /// @param Visitor The function to call for each entry.
    template<typename VisitorType>
    void ForEachEntry(VisitorType& Visitor) const
    {
        for(const auto& NameHandlerPair : LongCallingTable)
        {
            const KeyType& LongKey = NameHandlerPair.first;

            ShortKeyDelegationTableType::const_iterator ShortIter = ShortKeyReverseLookupTable.find(LongKey);
            const KeyType& ShortKey =
                (ShortIter != ShortKeyReverseLookupTable.cend()) ? ShortIter->second : KeyType("");

            ShortKeyDelegationTableType::const_iterator BriefIter = LongArgToBriefTable.find(LongKey);
            const HelpMessageType& BriefHelpMessage = (BriefIter != LongArgToBriefTable.cend()) ?
                BriefIter->second : HelpMessageType("");

            ShortKeyDelegationTableType::const_iterator HelpIter = LongArgToHelpMessageTable.find(LongKey);
            const HelpMessageType& LongHelpMessage = (HelpIter != LongArgToHelpMessageTable.cend()) ?
                HelpIter->second : HelpMessageType("");

            CommandLineCallingTableRecord DataForVisitor =
                { ShortKey, LongKey, BriefHelpMessage, LongHelpMessage, NameHandlerPair.second };

            Visitor(DataForVisitor);
        }
    }

};


/// @brief A simple class for getting all the data in one place for Help messages
/// @details Instances of this are intended to be passed to the CommandLineCallingTable#ForEachEntry
/// member function
class MEZZ_LIB CommandLineCallingTableVisitorHelpMessager
{
private:
    String Name;

    String Heading;
    String BriefTable;
    String HelpTable;

    CommandLineCallingTable& CallingTable;

    String CleanExecutableName(const String& RawName) const;
public:

    /// @brief This constructs one of these starting with the name of the current executable
    /// @param ExecutableName The name of the executable, used to help with help message.
    /// @param TargetCallingTable A reference to this calling table this should work with when
    explicit CommandLineCallingTableVisitorHelpMessager(String ExecutableName,
                                                        CommandLineCallingTable& TargetCallingTable);


    /// @brief Get the stored name of the executable.
    /// @return Whatever string was passed for the name is returned here but cleaned a little.
    String GetName() const;

    /// @brief Get the whole line for the
    /// @details If this hasn't visited a CommandLineCallingTable this visit
    /// @return A string including the long and short argus
    String GetHeading();
    String GetBriefTable();
    String GetHelpTable();

    String GetDefaultHelpMessage();

    //CommandLineCallingTableVisitorHelpMessager(const CommandLineCallingTableVisitorHelpMessager&) = default;
    //CommandLineCallingTableVisitorHelpMessager& operator=(const CommandLineCallingTableVisitorHelpMessager&) = default;
    //CommandLineCallingTableVisitorHelpMessager(CommandLineCallingTableVisitorHelpMessager&&) = default;
    //~CommandLineCallingTableVisitorHelpMessager() = default;

    void operator()(const CommandLineCallingTable::CommandLineCallingTableRecord& Handler);

};




}}

#endif
