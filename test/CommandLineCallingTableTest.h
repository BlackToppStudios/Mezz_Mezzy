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
#ifndef Mezz_Mezzy_CommandLineParserTest_h
#define Mezz_Mezzy_CommandLineParserTest_h

/// @file
/// @brief Tests for the stuff in SampleHeader and SampleSource


// Add other headers you need here
#include "CommandLineCallingTable.h"

#include "MezzTest.h"

// This boilerplate sucks, but it is as small as was reasonable to make it. This always matches the filename. Just
// Put FilenameTest in for the first parameter and Filename in for the second. There are reasons for this but even
// The developers of this havn't found a good reason to deviate, dig into the Mezz_Test docs if you want details.
DEFAULT_TEST_GROUP(CommandLineCallingTableTest, CommandLineCallingTable)
{
    // Make life easier for us writing this test.
    using CallingTable = Mezzanine::Mezzy::CommandLineCallingTable;
    using vecsize = CallingTable::ArgVectorType::size_type;
    //using ArgVectorSize= CallingTable::ArgVectorType::size_type;

    //Test Static Methods
    TEST_EQUAL("isShortKeyBadLong",     false, CallingTable::IsShortKey("--long"));
    TEST_EQUAL("isShortKeyBadNone",     false, CallingTable::IsShortKey("none"));
    TEST_EQUAL("isShortKeyGood",        true,  CallingTable::IsShortKey("-s"));
    TEST_EQUAL("isShortKeyBadEmpty",    false, CallingTable::IsShortKey(""));
    TEST_EQUAL("isShortKeyBadSingle",   false, CallingTable::IsShortKey("-"));
    TEST_EQUAL("isShortKeyBadDouble",   false, CallingTable::IsShortKey("--"));
    TEST_EQUAL("isShortKeyBadXShort",   false, CallingTable::IsShortKey("-badshort"));

    TEST_EQUAL("isLongKeyGood",         true,  CallingTable::IsLongKey("--long"));
    TEST_EQUAL("isLongKeyBadNone",      false, CallingTable::IsLongKey("none"));
    TEST_EQUAL("isLongKeyBadShort",     false, CallingTable::IsLongKey("-s"));
    TEST_EQUAL("isLongKeyBadEmpty",     false, CallingTable::IsLongKey(""));
    TEST_EQUAL("isLongKeyBadSingle",    false, CallingTable::IsLongKey("-"));
    TEST_EQUAL("isLongKeyBadDouble",    false, CallingTable::IsLongKey("--"));
    TEST_EQUAL("isLongKeyBadXShort",    false, CallingTable::IsLongKey("-badshort"));

    TEST_EQUAL("isValidKeyGoodLong",    true,  CallingTable::isValidKey("--long"));
    TEST_EQUAL("isValidKeyBadNone",     false, CallingTable::isValidKey("none"));
    TEST_EQUAL("isValidKeyGoodShort",   true,  CallingTable::isValidKey("-s"));
    TEST_EQUAL("isValidKeyBadEmpty",    false, CallingTable::isValidKey(""));
    TEST_EQUAL("isValidKeyBadSingle",   false, CallingTable::isValidKey("-"));
    TEST_EQUAL("isValidKeyBadDouble",   false, CallingTable::isValidKey("--"));
    TEST_EQUAL("isValidKeyBadXShort",   false, CallingTable::isValidKey("-badshort"));

    // Test Command line Arguments
    const CallingTable::KeyType ShortA = "-A";
    const CallingTable::KeyType LongA = "--Argument";
    const CallingTable::HelpMessageType BriefA = "Alphabetically the first argument";
    const CallingTable::HelpMessageType HelpA = "A longer more thorough explanation of A, there argument that comes "
                                                "first among arguments when sorted lexicographically.";

    const CallingTable::KeyType ShortB = "-L";
    const CallingTable::KeyType LongB = "--OnlyExistsAsLongArgument";
    const CallingTable::HelpMessageType BriefB = "Alphabetically the Second argument";
    const CallingTable::HelpMessageType HelpB = "B through K are unused so this is the second argument.";

    const CallingTable::KeyType ShortC = "-Z";
    const CallingTable::KeyType LongC = "--Zed";
    const CallingTable::HelpMessageType BriefC = "Alphabetically the Last argument";
    const CallingTable::HelpMessageType HelpC = "M through Y are unused so this is the second argument.";

    // Some bad ones too
    const CallingTable::KeyType ShortInvalid1 = "-Foobar";
    const CallingTable::KeyType ShortInvalid2 = "";

    const CallingTable::KeyType LongInvalid1 = "--";
    const CallingTable::KeyType LongInvalid2 = "";


    // Places to store passed Vectors
    CallingTable::ArgVectorType CaptureVectorA;
    CallingTable::ArgVectorType CaptureVectorB;
    CallingTable::ArgVectorType CaptureVectorC;

    // Test functions for our test calling tables
    CallingTable::ArgumentHandlerType TestHandlerA = [&CaptureVectorA](const CallingTable::ArgVectorType Args)
        { CaptureVectorA = Args; };
    CallingTable::ArgumentHandlerType TestHandlerB = [&CaptureVectorB](const CallingTable::ArgVectorType Args)
        { CaptureVectorB = Args; };
    CallingTable::ArgumentHandlerType TestHandlerC = [&CaptureVectorC](const CallingTable::ArgVectorType Args)
        { CaptureVectorC = Args; };

    // Create a Calling table with nothing in it
    CallingTable UnderTest;

    // Demonstrate there is nothing in it
    TEST_EQUAL("ShortEntryExists-StartEmptyAShort0",    false, UnderTest.ShortEntryExists(ShortA));
    TEST_EQUAL("ShortEntryExists-StartEmptyALong0",     false, UnderTest.ShortEntryExists(LongA));
    TEST_EQUAL("ShortEntryExists-StartEmptyBShort0",    false, UnderTest.ShortEntryExists(ShortB));
    TEST_EQUAL("ShortEntryExists-StartEmptyBLong0",     false, UnderTest.ShortEntryExists(LongB));
    TEST_EQUAL("ShortEntryExists-StartEmptyCShort0",    false, UnderTest.ShortEntryExists(ShortC));
    TEST_EQUAL("ShortEntryExists-StartEmptyCLong",      false, UnderTest.ShortEntryExists(LongC));

    TEST_EQUAL("LongEntryExists-StartEmptyAShort0",     false, UnderTest.LongEntryExists(ShortA));
    TEST_EQUAL("LongEntryExists-StartEmptyALong0",      false, UnderTest.LongEntryExists(LongA));
    TEST_EQUAL("LongEntryExists-StartEmptyBShort0",     false, UnderTest.LongEntryExists(ShortB));
    TEST_EQUAL("LongEntryExists-StartEmptyBLong0",      false, UnderTest.LongEntryExists(LongB));
    TEST_EQUAL("LongEntryExists-StartEmptyCShort0",     false, UnderTest.LongEntryExists(ShortC));
    TEST_EQUAL("LongEntryExists-StartEmptyCLong0",      false, UnderTest.LongEntryExists(LongC));

    TEST_EQUAL("EntryExists-StartEmptyAShort0",         false, UnderTest.EntryExists(ShortA));
    TEST_EQUAL("EntryExists-StartEmptyALong0",          false, UnderTest.EntryExists(LongA));
    TEST_EQUAL("EntryExists-StartEmptyBShort0",         false, UnderTest.EntryExists(ShortB));
    TEST_EQUAL("EntryExists-StartEmptyBLong0",          false, UnderTest.EntryExists(LongB));
    TEST_EQUAL("EntryExists-StartEmptyCShort0",         false, UnderTest.EntryExists(ShortC));
    TEST_EQUAL("EntryExists-StartEmptyCLong0",          false, UnderTest.EntryExists(LongC));

    // Lets test a bunch of bogus adding
    TEST_THROW("ThrowOnBadAdd5ParamShort1",
               std::runtime_error,
               [&](){ UnderTest.AddTableEntry(ShortInvalid1, LongA, BriefA, HelpA, TestHandlerA); }
    );
    TEST_THROW("ThrowOnBadAdd5ParamShort2",
               std::runtime_error,
               [&](){ UnderTest.AddTableEntry(ShortInvalid2, LongA, BriefA, HelpA, TestHandlerA); }
    );
    TEST_THROW("ThrowOnBadAdd5ParamLong1",
               std::runtime_error,
               [&](){ UnderTest.AddTableEntry(ShortA, LongInvalid1, BriefA, HelpA, TestHandlerA); }
    );
    TEST_THROW("ThrowOnBadAdd5ParamLong2",
               std::runtime_error,
               [&](){ UnderTest.AddTableEntry(LongInvalid2, LongA, BriefA, HelpA, TestHandlerA); }
    );

    TEST_THROW("ThrowOnBadAdd4ParamLong1",
               std::runtime_error,
               [&](){ UnderTest.AddTableEntry(LongInvalid1, BriefA, HelpA, TestHandlerA); }
    );
    TEST_THROW("ThrowOnBadAdd4ParamLong2",
               std::runtime_error,
               [&](){ UnderTest.AddTableEntry(LongInvalid2, BriefA, HelpA, TestHandlerA); }
    );

    // Verify the bogus entries are not present and show that the calling table is unchanged.`
    TEST_EQUAL("ShortEntryExists-BogusShort1",      false, UnderTest.ShortEntryExists(ShortInvalid1));
    TEST_EQUAL("ShortEntryExists-BogusShort2",      false, UnderTest.ShortEntryExists(ShortInvalid2));
    TEST_EQUAL("ShortEntryExists-BogusLong1",       false, UnderTest.ShortEntryExists(LongInvalid1));
    TEST_EQUAL("ShortEntryExists-BogusLong2",       false, UnderTest.ShortEntryExists(LongInvalid2));

    TEST_EQUAL("LongEntryExists-BogusShort1",       false, UnderTest.LongEntryExists(ShortInvalid1));
    TEST_EQUAL("LongEntryExists-BogusShort2",       false, UnderTest.LongEntryExists(ShortInvalid2));
    TEST_EQUAL("LongEntryExists-BogusLong1",        false, UnderTest.LongEntryExists(LongInvalid1));
    TEST_EQUAL("LongEntryExists-BogusLong2",        false, UnderTest.LongEntryExists(LongInvalid2));

    TEST_EQUAL("EntryExists-BogusShort1",           false, UnderTest.EntryExists(ShortInvalid1));
    TEST_EQUAL("EntryExists-BogusShort2",           false, UnderTest.EntryExists(ShortInvalid2));
    TEST_EQUAL("EntryExists-BogusLong1",            false, UnderTest.EntryExists(LongInvalid1));
    TEST_EQUAL("EntryExists-BogusLong2",            false, UnderTest.EntryExists(LongInvalid2));

    // Demonstrate there is still nothing legit in it
    TEST_EQUAL("ShortEntryExists-StartEmptyAShort1",    false, UnderTest.ShortEntryExists(ShortA));
    TEST_EQUAL("ShortEntryExists-StartEmptyALong1",     false, UnderTest.ShortEntryExists(LongA));
    TEST_EQUAL("ShortEntryExists-StartEmptyBShort1",    false, UnderTest.ShortEntryExists(ShortB));
    TEST_EQUAL("ShortEntryExists-StartEmptyBLong1",     false, UnderTest.ShortEntryExists(LongB));
    TEST_EQUAL("ShortEntryExists-StartEmptyCShort1",    false, UnderTest.ShortEntryExists(ShortC));
    TEST_EQUAL("ShortEntryExists-StartEmptyCLong1",     false, UnderTest.ShortEntryExists(LongC));

    TEST_EQUAL("LongEntryExists-StartEmptyAShort1",     false, UnderTest.LongEntryExists(ShortA));
    TEST_EQUAL("LongEntryExists-StartEmptyALong1",      false, UnderTest.LongEntryExists(LongA));
    TEST_EQUAL("LongEntryExists-StartEmptyBShort1",     false, UnderTest.LongEntryExists(ShortB));
    TEST_EQUAL("LongEntryExists-StartEmptyBLong1",      false, UnderTest.LongEntryExists(LongB));
    TEST_EQUAL("LongEntryExists-StartEmptyCShort1",     false, UnderTest.LongEntryExists(ShortC));
    TEST_EQUAL("LongEntryExists-StartEmptyCLong1",      false, UnderTest.LongEntryExists(LongC));

    TEST_EQUAL("EntryExists-StartEmptyAShort1",         false, UnderTest.EntryExists(ShortA));
    TEST_EQUAL("EntryExists-StartEmptyALong1",          false, UnderTest.EntryExists(LongA));
    TEST_EQUAL("EntryExists-StartEmptyBShort1",         false, UnderTest.EntryExists(ShortB));
    TEST_EQUAL("EntryExists-StartEmptyBLong1",          false, UnderTest.EntryExists(LongB));
    TEST_EQUAL("EntryExists-StartEmptyCShort1",         false, UnderTest.EntryExists(ShortC));
    TEST_EQUAL("EntryExists-StartEmptyCLong1",          false, UnderTest.EntryExists(LongC));

    // Add The A entry to the table.
    UnderTest.AddTableEntry(ShortA, LongA, BriefA, HelpA, TestHandlerA);

    // A should exist in long and short
    TEST_EQUAL("ShortEntryExists-AAddedAShort1",    true,  UnderTest.ShortEntryExists(ShortA));
    TEST_EQUAL("ShortEntryExists-AAddedALong1",     false, UnderTest.ShortEntryExists(LongA));
    TEST_EQUAL("ShortEntryExists-AAddedBShort1",    false, UnderTest.ShortEntryExists(ShortB));
    TEST_EQUAL("ShortEntryExists-AAddedBLong1",     false, UnderTest.ShortEntryExists(LongB));
    TEST_EQUAL("ShortEntryExists-AAddedCShort1",    false, UnderTest.ShortEntryExists(ShortC));
    TEST_EQUAL("ShortEntryExists-AAddedCLong1",     false, UnderTest.ShortEntryExists(LongC));

    TEST_EQUAL("LongEntryExists-AAddedAShort1",     false, UnderTest.LongEntryExists(ShortA));
    TEST_EQUAL("LongEntryExists-AAddedALong1",      true,  UnderTest.LongEntryExists(LongA));
    TEST_EQUAL("LongEntryExists-AAddedBShort1",     false, UnderTest.LongEntryExists(ShortB));
    TEST_EQUAL("LongEntryExists-AAddedBLong1",      false, UnderTest.LongEntryExists(LongB));
    TEST_EQUAL("LongEntryExists-AAddedCShort1",     false, UnderTest.LongEntryExists(ShortC));
    TEST_EQUAL("LongEntryExists-AAddedCLong1",      false, UnderTest.LongEntryExists(LongC));

    TEST_EQUAL("EntryExists-AAddedAShort1",         true,  UnderTest.EntryExists(ShortA));
    TEST_EQUAL("EntryExists-AAddedALong1",          true,  UnderTest.EntryExists(LongA));
    TEST_EQUAL("EntryExists-AAddedBShort1",         false, UnderTest.EntryExists(ShortB));
    TEST_EQUAL("EntryExists-AAddedBLong1",          false, UnderTest.EntryExists(LongB));
    TEST_EQUAL("EntryExists-AAddedCShort1",         false, UnderTest.EntryExists(ShortC));
    TEST_EQUAL("EntryExists-AAddedCLong1",          false, UnderTest.EntryExists(LongC));

    // Does adding A a second time fail no matter how it is added?
    TEST_THROW("ThrowOnDoubleAddSingleA",
               std::runtime_error,
               [&](){  UnderTest.AddTableEntry(ShortA, LongA, BriefA, HelpA, TestHandlerA); }
    );
    TEST_THROW("ThrowOnDoubleAddLongA",
               std::runtime_error,
               [&](){  UnderTest.AddTableEntry(LongA, BriefA, HelpA, TestHandlerA); }
    );

    // A should still exist and not have its short key removed.
    TEST_EQUAL("ShortEntryExists-AAddedAShort2",    true,  UnderTest.ShortEntryExists(ShortA));
    TEST_EQUAL("ShortEntryExists-AAddedALong2",     false, UnderTest.ShortEntryExists(LongA));
    TEST_EQUAL("ShortEntryExists-AAddedBShort2",    false, UnderTest.ShortEntryExists(ShortB));
    TEST_EQUAL("ShortEntryExists-AAddedBLong2",     false, UnderTest.ShortEntryExists(LongB));
    TEST_EQUAL("ShortEntryExists-AAddedCShort2",    false, UnderTest.ShortEntryExists(ShortC));
    TEST_EQUAL("ShortEntryExists-AAddedCLong2",     false, UnderTest.ShortEntryExists(LongC));

    TEST_EQUAL("LongEntryExists-AAddedAShort2",     false, UnderTest.LongEntryExists(ShortA));
    TEST_EQUAL("LongEntryExists-AAddedALong2",      true,  UnderTest.LongEntryExists(LongA));
    TEST_EQUAL("LongEntryExists-AAddedBShort2",     false, UnderTest.LongEntryExists(ShortB));
    TEST_EQUAL("LongEntryExists-AAddedBLong2",      false, UnderTest.LongEntryExists(LongB));
    TEST_EQUAL("LongEntryExists-AAddedCShort2",     false, UnderTest.LongEntryExists(ShortC));
    TEST_EQUAL("LongEntryExists-AAddedCLong2",      false, UnderTest.LongEntryExists(LongC));

    TEST_EQUAL("EntryExists-AAddedAShort2",         true,  UnderTest.EntryExists(ShortA));
    TEST_EQUAL("EntryExists-AAddedALong2",          true,  UnderTest.EntryExists(LongA));
    TEST_EQUAL("EntryExists-AAddedBShort2",         false, UnderTest.EntryExists(ShortB));
    TEST_EQUAL("EntryExists-AAddedBLong2",          false, UnderTest.EntryExists(LongB));
    TEST_EQUAL("EntryExists-AAddedCShort2",         false, UnderTest.EntryExists(ShortC));
    TEST_EQUAL("EntryExists-AAddedCLong2",          false, UnderTest.EntryExists(LongC));

    // Add B entry to the table without its short
    UnderTest.AddTableEntry(LongB, BriefB, HelpB, TestHandlerB);

    // Now we should have a long B
    TEST_EQUAL("ShortEntryExists-BAddedAShort1",    true,  UnderTest.ShortEntryExists(ShortA));
    TEST_EQUAL("ShortEntryExists-BAddedALong1",     false, UnderTest.ShortEntryExists(LongA));
    TEST_EQUAL("ShortEntryExists-BAddedBShort1",    false, UnderTest.ShortEntryExists(ShortB));
    TEST_EQUAL("ShortEntryExists-BAddedBLong1",     false, UnderTest.ShortEntryExists(LongB));
    TEST_EQUAL("ShortEntryExists-BAddedCShort1",    false, UnderTest.ShortEntryExists(ShortC));
    TEST_EQUAL("ShortEntryExists-BAddedCLong1",     false, UnderTest.ShortEntryExists(LongC));

    TEST_EQUAL("LongEntryExists-BAddedAShort1",     false, UnderTest.LongEntryExists(ShortA));
    TEST_EQUAL("LongEntryExists-BAddedALong1",      true,  UnderTest.LongEntryExists(LongA));
    TEST_EQUAL("LongEntryExists-BAddedBShort1",     false, UnderTest.LongEntryExists(ShortB));
    TEST_EQUAL("LongEntryExists-BAddedBLong1",      true,  UnderTest.LongEntryExists(LongB));
    TEST_EQUAL("LongEntryExists-BAddedCShort1",     false, UnderTest.LongEntryExists(ShortC));
    TEST_EQUAL("LongEntryExists-BAddedCLong1",      false, UnderTest.LongEntryExists(LongC));

    TEST_EQUAL("EntryExists-BAddedAShort1",         true,  UnderTest.EntryExists(ShortA));
    TEST_EQUAL("EntryExists-BAddedALong1",          true,  UnderTest.EntryExists(LongA));
    TEST_EQUAL("EntryExists-BAddedBShort1",         false, UnderTest.EntryExists(ShortB));
    TEST_EQUAL("EntryExists-BAddedBLong1",          true,  UnderTest.EntryExists(LongB));
    TEST_EQUAL("EntryExists-BAddedCShort1",         false, UnderTest.EntryExists(ShortC));
    TEST_EQUAL("EntryExists-BAddedCLong1",          false, UnderTest.EntryExists(LongC));

    // Does adding B a second time fail no matter how it is added?
    TEST_THROW("ThrowOnDoubleAddSingleB",
               std::runtime_error,
               [&](){ UnderTest.AddTableEntry(LongB, BriefB, HelpB, TestHandlerB); }
    );
    TEST_THROW("ThrowOnDoubleAddLongB",
               std::runtime_error,
               [&](){ UnderTest.AddTableEntry(LongB, BriefB, HelpB, TestHandlerB); }
    );

    // Check that the state is unchanged by failed B double adds
    TEST_EQUAL("ShortEntryExists-BAddedAShort2",    true,  UnderTest.ShortEntryExists(ShortA));
    TEST_EQUAL("ShortEntryExists-BAddedALong2",     false, UnderTest.ShortEntryExists(LongA));
    TEST_EQUAL("ShortEntryExists-BAddedBShort2",    false, UnderTest.ShortEntryExists(ShortB));
    TEST_EQUAL("ShortEntryExists-BAddedBLong2",     false, UnderTest.ShortEntryExists(LongB));
    TEST_EQUAL("ShortEntryExists-BAddedCShort2",    false, UnderTest.ShortEntryExists(ShortC));
    TEST_EQUAL("ShortEntryExists-BAddedCLong2",     false, UnderTest.ShortEntryExists(LongC));

    TEST_EQUAL("LongEntryExists-BAddedAShort2",     false, UnderTest.LongEntryExists(ShortA));
    TEST_EQUAL("LongEntryExists-BAddedALong2",      true,  UnderTest.LongEntryExists(LongA));
    TEST_EQUAL("LongEntryExists-BAddedBShort2",     false, UnderTest.LongEntryExists(ShortB));
    TEST_EQUAL("LongEntryExists-BAddedBLong2",      true,  UnderTest.LongEntryExists(LongB));
    TEST_EQUAL("LongEntryExists-BAddedCShort2",     false, UnderTest.LongEntryExists(ShortC));
    TEST_EQUAL("LongEntryExists-BAddedCLong2",      false, UnderTest.LongEntryExists(LongC));

    TEST_EQUAL("EntryExists-BAddedAShort2",         true,  UnderTest.EntryExists(ShortA));
    TEST_EQUAL("EntryExists-BAddedALong2",          true,  UnderTest.EntryExists(LongA));
    TEST_EQUAL("EntryExists-BAddedBShort2",         false, UnderTest.EntryExists(ShortB));
    TEST_EQUAL("EntryExists-BAddedBLong2",          true,  UnderTest.EntryExists(LongB));
    TEST_EQUAL("EntryExists-BAddedCShort2",         false, UnderTest.EntryExists(ShortC));
    TEST_EQUAL("EntryExists-BAddedCLong2",          false, UnderTest.EntryExists(LongC));

    // Adding C
    UnderTest.AddTableEntry(ShortC, LongC, BriefC, HelpC, TestHandlerC);

    TEST_EQUAL("ShortEntryExists-CAddedAShort",    true,  UnderTest.ShortEntryExists(ShortA));
    TEST_EQUAL("ShortEntryExists-CAddedALong",     false, UnderTest.ShortEntryExists(LongA));
    TEST_EQUAL("ShortEntryExists-CAddedBShort",    false, UnderTest.ShortEntryExists(ShortB));
    TEST_EQUAL("ShortEntryExists-CAddedBLong",     false, UnderTest.ShortEntryExists(LongB));
    TEST_EQUAL("ShortEntryExists-CAddedCShort",    true,  UnderTest.ShortEntryExists(ShortC));
    TEST_EQUAL("ShortEntryExists-CAddedCLong",     false, UnderTest.ShortEntryExists(LongC));

    TEST_EQUAL("LongEntryExists-CAddedAShort",     false, UnderTest.LongEntryExists(ShortA));
    TEST_EQUAL("LongEntryExists-CAddedALong",      true,  UnderTest.LongEntryExists(LongA));
    TEST_EQUAL("LongEntryExists-CAddedBShort",     false, UnderTest.LongEntryExists(ShortB));
    TEST_EQUAL("LongEntryExists-CAddedBLong",      true,  UnderTest.LongEntryExists(LongB));
    TEST_EQUAL("LongEntryExists-CAddedCShort",     false, UnderTest.LongEntryExists(ShortC));
    TEST_EQUAL("LongEntryExists-CAddedCLong",      true,  UnderTest.LongEntryExists(LongC));

    TEST_EQUAL("EntryExists-CAddedAShort",         true,  UnderTest.EntryExists(ShortA));
    TEST_EQUAL("EntryExists-CAddedALong",          true,  UnderTest.EntryExists(LongA));
    TEST_EQUAL("EntryExists-CAddedBShort",         false, UnderTest.EntryExists(ShortB));
    TEST_EQUAL("EntryExists-CAddedBLong",          true,  UnderTest.EntryExists(LongB));
    TEST_EQUAL("EntryExists-CAddedCShort",         true,  UnderTest.EntryExists(ShortC));
    TEST_EQUAL("EntryExists-CAddedCLong",          true,  UnderTest.EntryExists(LongC));

    // Check that the holding site for results is clear
    TEST_EQUAL("EmptyCaptureA",     vecsize{0},  CaptureVectorA.size());
    TEST_EQUAL("EmptyCaptureB",     vecsize{0},  CaptureVectorB.size());
    TEST_EQUAL("EmptyCaptureC",     vecsize{0},  CaptureVectorC.size());

    // launch each function and see if it worked
    UnderTest.ExecuteEntry(LongA, {"1"});
    UnderTest.ExecuteEntry(LongB, {"2", "3"});
    UnderTest.ExecuteEntry(LongC, {"4", "5", "6"});

    // That the args are passed through is good enough
    TEST_EQUAL("CaptureAWorkedLong",     vecsize{1},  CaptureVectorA.size());
    TEST_EQUAL("CaptureBWorkedLong",     vecsize{2},  CaptureVectorB.size());
    TEST_EQUAL("CaptureCWorkedLong",     vecsize{3},  CaptureVectorC.size());

    // launch each function and see if it cleared out the capture groups
    UnderTest.ExecuteEntry(LongA);
    UnderTest.ExecuteEntry(LongB);
    UnderTest.ExecuteEntry(LongC);

    // The default calls should clear this
    TEST_EQUAL("EmptyCaptureAWorked",     vecsize{0},  CaptureVectorA.size());
    TEST_EQUAL("EmptyCaptureBWorked",     vecsize{0},  CaptureVectorB.size());
    TEST_EQUAL("EmptyCaptureCWorked",     vecsize{0},  CaptureVectorC.size());

    // launch each function by short and see if it worked or failed as expected.
    UnderTest.ExecuteEntry(ShortA, {"7", "8", "9", "10", "11"});

    TEST_THROW("ThrowOnNoExecuteShortB",
               std::runtime_error,
               [&](){ UnderTest.ExecuteEntry(ShortB, {"12", "13", "14", "15", "16", "17"}); }
    );

    UnderTest.ExecuteEntry(ShortC, {"18", "19", "20", "21", "22", "23", "24"});

    // That the args are passed through is good enough, and unchanged where no short arg exists.
    TEST_EQUAL("CaptureAWorkedShort",     vecsize{5},  CaptureVectorA.size());
    TEST_EQUAL("CaptureBWorkedShort",     vecsize{0},  CaptureVectorB.size());
    TEST_EQUAL("CaptureCWorkedShort",     vecsize{7},  CaptureVectorC.size());

    // Vistor testing, passing these to the handler is normally dumb, reasonable, but dunb.
    auto TestVisitor = [](  const CallingTable::CommandLineCallingTableRecord& Visited)
        { Visited.Handler({Visited.ShortKey, Visited.LongKey, Visited.BriefHelp, Visited.LongHelp}); };
    UnderTest.ForEachEntry(TestVisitor);

    // The visitation should have invoked
    TEST_EQUAL("CaptureAWorkedVisit",     vecsize{4},  CaptureVectorA.size());
    TEST_EQUAL("CaptureBWorkedVisit",     vecsize{4},  CaptureVectorB.size());
    TEST_EQUAL("CaptureCWorkedVisit",     vecsize{4},  CaptureVectorC.size());

    TEST_EQUAL("VisitAShort",   ShortA,  CallingTable::KeyType(CaptureVectorA[0]));
    TEST_EQUAL("VisitALong",    LongA,   CallingTable::KeyType(CaptureVectorA[1]));
    TEST_EQUAL("VisitABrief",   BriefA,  CallingTable::KeyType(CaptureVectorA[2]));
    TEST_EQUAL("VisitAHelp",    HelpA,   CallingTable::KeyType(CaptureVectorA[3]));

    TEST_EQUAL("VisitBShort",   CallingTable::KeyType(""),
                                         CallingTable::KeyType(CaptureVectorB[0]));
    TEST_EQUAL("VisitBLong",    LongB,   CallingTable::KeyType(CaptureVectorB[1]));
    TEST_EQUAL("VisitBBrief",   BriefB,  CallingTable::KeyType(CaptureVectorB[2]));
    TEST_EQUAL("VisitBHelp",    HelpB,   CallingTable::KeyType(CaptureVectorB[3]));

    TEST_EQUAL("VisitCShort",   ShortC,  CallingTable::KeyType(CaptureVectorC[0]));
    TEST_EQUAL("VisitCLong",    LongC,   CallingTable::KeyType(CaptureVectorC[1]));
    TEST_EQUAL("VisitCBrief",   BriefC,  CallingTable::KeyType(CaptureVectorC[2]));
    TEST_EQUAL("VisitCHelp",    HelpC,   CallingTable::KeyType(CaptureVectorC[3]));

    // Test the help message generation and visitation
    Mezzanine::Mezzy::CommandLineCallingTableVisitorHelpMessager HelpMakerPreinvoke("./Tester", UnderTest);

    // This is optional, but can
    UnderTest.ForEachEntry(HelpMakerPreinvoke);

    TEST_EQUAL("HelpVisitName-Preinvoke", Mezzanine::String("Tester"), HelpMakerPreinvoke.GetName());
    TEST_STRING_CONTAINS("HelpVisitorHeaderHasBracketA-Preinvoke",
                         Mezzanine::String("["),
                         HelpMakerPreinvoke.GetHeading());
    TEST_STRING_CONTAINS("HelpVisitorHeaderHasPipe-Preinvoke",
                         Mezzanine::String("|"),
                         HelpMakerPreinvoke.GetHeading());


    TEST_STRING_CONTAINS("HelpVisitorHeaderHasShortA-Preinvoke",
                         ShortA,
                         HelpMakerPreinvoke.GetHeading());
    TEST_STRING_CONTAINS("HelpVisitorHeaderHasLongA-Preinvoke",
                         LongA,
                         HelpMakerPreinvoke.GetHeading());

                // There is no short B
    TEST_STRING_CONTAINS("HelpVisitorHeaderHasLongB-Preinvoke",
                         LongB,
                         HelpMakerPreinvoke.GetHeading());

    TEST_STRING_CONTAINS("HelpVisitorHeaderHasShortC-Preinvoke",
                         ShortC,
                         HelpMakerPreinvoke.GetHeading());
    TEST_STRING_CONTAINS("HelpVisitorHeaderHasLongC-Preinvoke",
                         LongC,
                         HelpMakerPreinvoke.GetHeading());


    TEST_STRING_CONTAINS("HelpVisitorBriefHasA-Preinvoke",
                         BriefA,
                         HelpMakerPreinvoke.GetBriefTable());
    TEST_STRING_CONTAINS("HelpVisitorBriefHasB-Preinvoke",
                         BriefB,
                         HelpMakerPreinvoke.GetBriefTable());
    TEST_STRING_CONTAINS("HelpVisitorBriefHasC-Preinvoke",
                         BriefC,
                         HelpMakerPreinvoke.GetBriefTable());

    TEST_STRING_CONTAINS("HelpVisitorHelpHasA-Preinvoke",
                         HelpA,
                         HelpMakerPreinvoke.GetHelpTable());
    TEST_STRING_CONTAINS("HelpVisitorHelpHasB-Preinvoke",
                         HelpB,
                         HelpMakerPreinvoke.GetHelpTable());
    TEST_STRING_CONTAINS("HelpVisitorHelpHasC-Preinvoke",
                         HelpC,
                         HelpMakerPreinvoke.GetHelpTable());


    // Test the help message generation with the implied visitation that the help generator should have.
    Mezzanine::Mezzy::CommandLineCallingTableVisitorHelpMessager HelpMakerImpliedVisitForName("./Tester", UnderTest);
    TEST_EQUAL("HelpVisitName-ImpliedVisit", Mezzanine::String("Tester"), HelpMakerImpliedVisitForName.GetName());

    Mezzanine::Mezzy::CommandLineCallingTableVisitorHelpMessager HelpMakerImpliedVisitForHeader("./Tester", UnderTest);
    TEST_STRING_CONTAINS("HelpVisitorHeaderHasBracketA-ImpliedVisit",
                         Mezzanine::String("["),
                         HelpMakerImpliedVisitForHeader.GetHeading());
    TEST_STRING_CONTAINS("HelpVisitorHeaderHasPipe-ImpliedVisit",
                         Mezzanine::String("|"),
                         HelpMakerImpliedVisitForHeader.GetHeading());
    TEST_STRING_CONTAINS("HelpVisitorHeaderHasShortA-ImpliedVisit",
                         ShortA,
                         HelpMakerImpliedVisitForHeader.GetHeading());
    TEST_STRING_CONTAINS("HelpVisitorHeaderHasLongA-ImpliedVisit",
                         LongA,
                         HelpMakerImpliedVisitForHeader.GetHeading());

                // There is no short B
    TEST_STRING_CONTAINS("HelpVisitorHeaderHasLongB-ImpliedVisit",
                         LongB,
                         HelpMakerImpliedVisitForHeader.GetHeading());

    TEST_STRING_CONTAINS("HelpVisitorHeaderHasShortC-ImpliedVisit",
                         ShortC,
                         HelpMakerImpliedVisitForHeader.GetHeading());
    TEST_STRING_CONTAINS("HelpVisitorHeaderHasLongC-ImpliedVisit",
                         LongC,
                         HelpMakerImpliedVisitForHeader.GetHeading());

    Mezzanine::Mezzy::CommandLineCallingTableVisitorHelpMessager HelpMakerImpliedVisitForBrief("./Tester", UnderTest);
    TEST_STRING_CONTAINS("HelpVisitorBriefHasA-ImpliedVisit",
                         BriefA,
                         HelpMakerImpliedVisitForBrief.GetBriefTable());
    TEST_STRING_CONTAINS("HelpVisitorBriefHasB-ImpliedVisit",
                         BriefB,
                         HelpMakerImpliedVisitForBrief.GetBriefTable());
    TEST_STRING_CONTAINS("HelpVisitorBriefHasC-ImpliedVisit",
                         BriefC,
                         HelpMakerImpliedVisitForBrief.GetBriefTable());

    Mezzanine::Mezzy::CommandLineCallingTableVisitorHelpMessager HelpMakerImpliedVisitForHelp("./Tester", UnderTest);
    TEST_STRING_CONTAINS("HelpVisitorHelpHasA-ImpliedVisit",
                         HelpA,
                         HelpMakerImpliedVisitForHelp.GetHelpTable());
    TEST_STRING_CONTAINS("HelpVisitorHelpHasB-ImpliedVisit",
                         HelpB,
                         HelpMakerImpliedVisitForHelp.GetHelpTable());
    TEST_STRING_CONTAINS("HelpVisitorHelpHasC-ImpliedVisit",
                         HelpC,
                         HelpMakerImpliedVisitForHelp.GetHelpTable());

}

#endif
