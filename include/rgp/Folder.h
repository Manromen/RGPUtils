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
#include <string>
#include <vector>
#include <memory>

// Unix
#if defined(__APPLE__) || defined(__unix__)
// on unixes we use dirent to iterate through the folder
#include <dirent.h>
#endif // defined(__APPLE__) || defined(__linux__)

// Windows
#if defined(_WIN32)
#include <Windows.h>
#endif // defined(_WIN32)

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
    
    ///< type of an entry inside a folder
    enum EntryType {
        EntryTypeUnknown = 0, /**< Unknown file type */
        EntryTypeFolder, /**< File is a folder */
        EntryTypeRegularFile /**< A regular file */
    };
    
    /**
     @brief Data of an entry inside a folder.
     */
    class RGPFOLDER_EXPORT FolderEntry {
        
    public:
        ///< The type of the entry (f.e. a folder)
        EntryType type() const {
            return _type;
        };

        ///< The filename of the entry
        std::string name() const{
            return _name;
        };

        ///< The path to the entry (without the name)
        std::string path() const{
            return _path;
        };
        
        ///< The path to the entry (with the name)
        std::string fullpath() const{
            return _fullpath;
        };

    private:
        EntryType _type;
        std::string _name;
        std::string _path;
        std::string _fullpath;
        
        friend class Folder;
    };
    
    /**
     @brief A Class that represents a folder in the filesytem
     */
    class RGPFOLDER_EXPORT Folder {
        
    public:
        /**
         @brief Create a folder object with the given folder path.
         @details Constructor for a folder object. Throws a FolderException
         if the path is invalid or points to a file that is not a folder.
         @param path The path to the folder that should be referenced.
         */
        Folder(const std::string &path);
        
        /**
         @brief List of all entries in the folder.
         @details This won't list any files from inside child folders.
		 You have to iterate through the child folders if you want to get the files from there too.
         @return Shared Pointer to a vector that holds all the entries.
         */
        std::shared_ptr<std::vector<FolderEntry>> listEntries() const;
        
    private:
        std::string _path;
        
        // Don't allow creating an object without a path
        Folder() = delete;
    };
    
    /**
     @brief Folder Exception class.
     @details Will be thrown on error.
     */
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
