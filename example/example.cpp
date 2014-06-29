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
        rgp::Folder folder { "C:" };
#endif

        // check if just created folder object has a legit corresponding folder.
        if (folder.isFolder ()) {
            // get all entries in that folder
            std::shared_ptr<std::vector<FolderEntry>> list { folder.listEntries () };

            if (list != nullptr)
            {
                // feedback if the folder is empty
                // ( in unix there should be at least . and .. )
                if (list->size () == 0) {
                    std::cout << "No entries found" << std::endl;
                }

                // print all entries from the folder
                for (FolderEntry entry : *list) {
                    std::cout << "File: " << entry.name () << std::endl;
                }
            }
            else {
                std::cout << folder.path () << " couldn't list folder entries." << std::endl;
            }
        }
        else {
            std::cout << folder.path () << " isn't a folder." << std::endl;
        }

        // print folder for app data
        std::shared_ptr<rgp::Folder> appData { rgp::Folder::getFolder (FolderTypeAppData) };
        std::cout << "AppData: " << appData->path () << std::endl << std::flush;

        // print home folder
        std::shared_ptr<rgp::Folder> home { rgp::Folder::getFolder (FolderTypeHome) };
        std::cout << "Home: " << home->path () << std::endl << std::flush;

        // create a new folder
        std::string newPath { home->path () + rgp::Folder::pathSeparator () + "TestFolder" };
        std::shared_ptr<rgp::Folder> newFolder { rgp::Folder::createFolder (newPath) };

        if (newFolder != nullptr)
        {
            std::cout << "Created new folder: " << newFolder->path () << std::endl << std::flush;
        }
    }
    catch (rgp::FolderException &exception) {
        std::cerr << "There was an error with a folder object: " << exception.what () << std::endl << std::flush;
    }
    
    return EXIT_SUCCESS;
}
