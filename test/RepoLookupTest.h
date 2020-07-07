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
    using ArgVectorSize= CallingTable::ArgVectorType::size_type;

    // Places to store passed Vectors
    CallingTable::ArgVectorType CaptureVectorA;
    CallingTable::ArgVectorType CaptureVectorB;



    // Test functions for our test calling tables
    CallingTable::ArgumentHandlerType TestHandlerA = [&CaptureVectorA](const CallingTable::ArgVectorType Args)
        { CaptureVectorA = Args; };
    CallingTable::ArgumentHandlerType TestHandlerB = [&CaptureVectorB](const CallingTable::ArgVectorType Args)
        { CaptureVectorB = Args; };


    // Create a Calling table with nothing in it
    CallingTable UnderTest;
    TEST_EQUAL("StartEmptyA1",          false, UnderTest.EntryExists("A"));
    TEST_EQUAL("StartEmptyB1",          false, UnderTest.EntryExists("B"));


    // Wiring up the test table with the test fixtures we just made.
    UnderTest.AddTableEntry("A", std::move(TestHandlerA));
    TEST_EQUAL("PresentAfterAddA",      true,  UnderTest.EntryExists("A"));
    TEST_EQUAL("NotPresentAfterAddA",   false, UnderTest.EntryExists("B"));

    UnderTest.AddTableEntry("B", std::move(TestHandlerB));
    TEST_EQUAL("UnchangeedAfterAddB",   true, UnderTest.EntryExists("A"));
    TEST_EQUAL("PresentAfterAddB",      true, UnderTest.EntryExists("B"));

    // Double adds should throw and shouldn't change anything.
    TEST_THROW("ThrowOnDoubleAdd", std::runtime_error, [&](){ UnderTest.AddTableEntry("A", std::move(TestHandlerA)); });
    TEST_EQUAL("GoodPostDoubleAddA",    true, UnderTest.EntryExists("A"));


    // Let's try invoking the calling table and see if it accepts the args and does what our lambdas do.
    UnderTest.ExecuteEntry("A");
    TEST_EQUAL("AExecutionSanityCheck0Size",        ArgVectorSize(0), CaptureVectorA.size());
    TEST_EQUAL("BExecutionSanityCheck0Size",        ArgVectorSize(0), CaptureVectorB.size());

    UnderTest.ExecuteEntry("A", {});
    TEST_EQUAL("AExecutionSanityCheck1Size",        ArgVectorSize(0), CaptureVectorA.size());
    TEST_EQUAL("BExecutionSanityCheck1Size",        ArgVectorSize(0), CaptureVectorB.size());

    UnderTest.ExecuteEntry("A", {"SampleArg"});
    TEST_EQUAL("AExecutionSanityCheck2Size",        ArgVectorSize(1), CaptureVectorA.size());
    TEST_EQUAL("AExecutionSanityCheck2Index0",      Mezzanine::String("SampleArg"), CaptureVectorA[0]);
    TEST_EQUAL("BExecutionSanityCheck2Size",        ArgVectorSize(0), CaptureVectorB.size());

    UnderTest.ExecuteEntry("A", {"File1.txt","File2.txt"});
    TEST_EQUAL("AExecutionSanityCheck3Size",        ArgVectorSize(2), CaptureVectorA.size());
    TEST_EQUAL("AExecutionSanityCheck3Index0",      Mezzanine::String("File1.txt"), CaptureVectorA[0]);
    TEST_EQUAL("AExecutionSanityCheck3Index1",      Mezzanine::String("File2.txt"), CaptureVectorA[1]);
    TEST_EQUAL("BExecutionSanityCheck3Size",        ArgVectorSize(0), CaptureVectorB.size());

    UnderTest.ExecuteEntry("B", {"1", "2", "3"});
    TEST_EQUAL("AExecutionSanityCheck4Size",        ArgVectorSize(2), CaptureVectorA.size());
    TEST_EQUAL("AExecutionSanityCheck4Index0",      Mezzanine::String("File1.txt"), CaptureVectorA[0]);
    TEST_EQUAL("AExecutionSanityCheck4Index1",      Mezzanine::String("File2.txt"), CaptureVectorA[1]);
    TEST_EQUAL("BExecutionSanityCheck4Size",        ArgVectorSize(3), CaptureVectorB.size());
    TEST_EQUAL("BExecutionSanityCheck4Index0",      Mezzanine::String("1"), CaptureVectorB[0]);
    TEST_EQUAL("BExecutionSanityCheck4Index1",      Mezzanine::String("2"), CaptureVectorB[1]);
    TEST_EQUAL("BExecutionSanityCheck4Index2",      Mezzanine::String("3"), CaptureVectorB[2]);

    // This should throw on calling a bad table entry.
    TEST_THROW("ThrowBadExecuteName", std::runtime_error, [&](){ UnderTest.ExecuteEntry("NotArg"); });
}

#endif
