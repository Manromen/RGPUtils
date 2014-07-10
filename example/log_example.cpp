/*
 RGPUtils
 log_example.cpp

 Created by Ralph-Gordon Paul on 24.07.13.
 
 Example usage of the Log Class.
 
 -------------------------------------------------------------------------------
 GNU Lesser General Public License Version 3, 29 June 2007
 
 Copyright (c) 2014 Ralph-Gordon Paul. All rights reserved.
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this library.
 -------------------------------------------------------------------------------
*/

#include <iostream>
#include <sstream>

#include <rgp/Log.h>

using namespace rgp;

int main (int argc, const char **argv)
{
    // will logout (if we are in debug mode) that we are in main function
    RGPLOGMETHODNAME();
    
    Log::sharedLog()->print("normal logout");
    
    RGPLOG("This will only be logged if compiled in debug mode");
    
    // output using stringstream
    std::stringstream logout;
    logout << "this ";
    logout << "is ";
    logout << "another method " << 42 << std::endl;
    Log::sharedLog()->print(logout.str());
    
    Log::sharedLog()->printv("logout on verbose mode ... not set yet => this" \
        "won't be printed");

    // enable ANSI SGR Color Codes
#if !defined(_WIN32)
    Log::sharedLog()->setUseAnsiSgrCodes(true);
    
    Log::sharedLog()->print("This text is red!", AnsiSgrFgColorRed);
    Log::sharedLog()->print("This text is black on white background!",
                            AnsiSgrFgColorBlack, AnsiSgrBgColorWhite);
#endif // !defined(_WIN32)
    Log::sharedLog()->setLoglevel(LoglevelVerbose);
    
    RGPLOGV("This will only be logged if compiled in debug mode and logging " \
            "is on verbose");
    
    // getline will get the input from the user
    std::string yourName = Log::sharedLog()->getline("What is your name?: ");
    
    Log::sharedLog()->print(std::string("So, Your name is: ") += yourName);
    
    return EXIT_SUCCESS;
}
