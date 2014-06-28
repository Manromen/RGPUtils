/*
Folder.h

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

#ifndef __RGPFolder__Folder_H__
#define __RGPFolder__Folder_H__

// Standard C++
#include <iostream>
#include <string>
#include <map>
#include <vector>

// Unix
#if defined(__APPLE__) || defined(__linux__)
#include <dirent.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#endif // defined(__APPLE__) || defined(__linux__)

// on windows we need the exports for creating the dll
#if defined(_WIN32)
  #if defined(RGPFOLDER_EXPORTS)
    #define RGPFOLDER_EXPORT __declspec(dllexport)
  #else
    #define RGPFOLDER_EXPORT __declspec(dllimport)
  #endif /* defined (RGPFOLDER_EXPORTS) */
#else /* defined (_WIN32) */
 #define RGPFOLDER_EXPORT
#endif

namespace rgp {
    
    // type of an entry inside the folder
    enum EntryType {
        EntryTypeUnknown = 0,
        EntryTypeFolder,
        EntryTypeRegularFile
    };
    
    class RGPFOLDER_EXPORT FolderEntry {
        
    public:
        EntryType type() const {
            return _type;
        };
        std::string name() const{
            return _name;
        };
        std::string path() const{
            return _path;
        };
        
    private:
        EntryType _type;
        std::string _name;
        std::string _path;
        
        friend class Folder;
    };
    
    class RGPFOLDER_EXPORT Folder {
        
    public:
        // create a folder object with the given folder path
        // throws FolderException on error
        Folder(const std::string &path);
        
        //
        std::shared_ptr<std::vector<FolderEntry>> listEntries() const;
        
    private:
        std::string _path;
        
        Folder() = delete;
    };
    
    /** Exception Class **/
    class RGPFOLDER_EXPORT FolderException : std::exception {
        
    public:
        FolderException() {};
        FolderException(std::string exception) : _exceptionString(exception) {};
        
        std::string what() { return _exceptionString; };
        virtual const char* what() const throw() {
            return _exceptionString.c_str();
        };
        
    private:
        std::string _exceptionString { "Unknown Error" };
    };
}

#endif /* defined(__RGPFolder__Folder_H__) */
