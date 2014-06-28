/*
 example.cpp

 Created by Ralph-Gordon Paul on 27. June 2014.
 
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

#include <rgp/Folder.h>

using namespace rgp;

int main (int argc, const char **argv)
{
    try {
        
        // create folder object
#if defined(__unix__) || defined(__APPLE__)
        Folder folder { "/" };
#elif defined(_WIN32)
        Folder folder { "C:\\" };
#endif
        
        // get all entries in that folder
        std::shared_ptr<std::vector<FolderEntry>> list { folder.listEntries() };
        
        // feedback if the folder is empty
        // ( in unix there should be at least . and .. )
        if (list->size() == 0) {
            std::cout << "No entries found" << std::endl;
        }
        
        // print all entries from the folder
        for ( FolderEntry entry : *list) {
            std::cout << "File: " << entry.name() << std::endl;
        }
        
    } catch (FolderException &exception) {
        std::cout << "error creating folder object: "
        << exception.what() << std::endl;
    }
    
    return EXIT_SUCCESS;
}
