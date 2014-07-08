/*
 example.cpp

 Created by Ralph-Gordon Paul on 10. May 2014.
 
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

#include <rgp/Config.h>

using namespace rgp;

int main (int argc, const char **argv)
{
    try {
        Config config { Config("example.conf") };
        
        std::string username = config.getOptionForKey( "username" );
        
        std::cout << "username: " << username << std::endl;
        
    } catch (ConfigException &exception) {
        std::cout << "error parsing config file: "
                  << exception.what() << std::endl;
    }
    
    return EXIT_SUCCESS;
}
