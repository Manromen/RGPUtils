/*
 Folder.cpp
 
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

#include <rgp/Folder.h>

using namespace rgp;


Folder::Folder(const std::string &path) : _path(path)
{
    // TODO: check if _path points to a directory
    // throw an exception if _path is not readable or not a directory
}

// Unix version
#if defined(__APPLE__) || defined(__unix__)
std::shared_ptr<std::vector<FolderEntry>> Folder::listEntries() const
{
    // create new list
    std::shared_ptr<std::vector<FolderEntry>> list {
        std::make_shared<std::vector<FolderEntry>>()
    };
    
    // init dirent
    struct dirent *dir_entry { NULL };
    DIR *directory { opendir( _path.c_str() ) };
    
    // iterate through directory and fill the list
    if (directory != NULL) {
        
        while ( (dir_entry = readdir(directory)) != NULL ) {
            
            // create new folder entry
            FolderEntry entry {};
            entry._name = dir_entry->d_name;
            entry._path = _path + entry._name;
            
            // determine file type
            switch (dir_entry->d_type) {
                case DT_DIR: {
                    entry._type = EntryTypeFolder;
                } break;
                    
                case DT_REG: {
                    entry._type = EntryTypeRegularFile;
                } break;
                    
                default: {
                    entry._type = EntryTypeUnknown;
                } break;
            }
            
            // append entry to the list
            list->push_back(entry);
        }
        
        // finally close the directory
        closedir( directory );
    }
    
    return list;
}
#endif // defined(__APPLE__) || defined(__linux__)

// Windows version
#if defined(_WIN32)
std::vector<FolderEntry> Folder::listEntries() const
{
    std::shared_ptr<std::vector<FolderEntry>> list { std::make_shared<std::vector<FolderEntry>>() };
    
    // TODO: implement windows version
    
    return list;
}
#endif // defined(_WIN32)

