/*
 RGPUtils
 Folder.h
 
 Created by Ralph-Gordon Paul on 27. June 2014.
 
 A Lightweight Class that provides cross-plattform access to folders.
 
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

#ifndef __RGPUtils__Folder_H__
#define __RGPUtils__Folder_H__

// Standard C++
#include <string>
#include <vector>
#include <memory>
#include <sstream>

// Unix
#if defined(__APPLE__) || defined(__unix__)
// on unixes we use dirent to iterate through the folder
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#endif // defined(__APPLE__) || defined(__linux__)

// Windows
#if defined(_WIN32)
#include <Windows.h>
#include <winerror.h>
#include <ShlObj.h>

#endif // defined(_WIN32)

// on windows we need the exports for creating the dll
#if defined(_WIN32)
  #if defined(RGPUTILS_EXPORTS)
    #define RGPUTILS_EXPORT __declspec(dllexport)
  #else
    #define RGPUTILS_EXPORT __declspec(dllimport)
  #endif /* defined (RGPUTILS_EXPORTS) */
#else /* defined (_WIN32) */
 #define RGPUTILS_EXPORT
#endif

namespace rgp {

    class Folder;
    
    ///< type of an entry inside a folder
    enum EntryType {
        EntryTypeUnknown = 0, /**< Unknown file type */
        EntryTypeFolder, /**< File is a folder */
        EntryTypeRegularFile /**< A regular file */
    };

    enum FolderType {
        FolderTypeDefault = 0, /**<  */
        FolderTypeAppData,
        FolderTypeHome
    };
    
    /**
     @brief Data of an entry inside a folder.
     */
    class RGPUTILS_EXPORT FolderEntry {
        
    public:
        ///< The type of the entry (f.e. a folder)
        EntryType type () const {
            return _type;
        };

        ///< The filename of the entry
        std::string name () const{
            return _name;
        };

        /**< The path to the entry
         (without the name and without the trailing path separator) */
        std::string path () const{
            return _path;
        };
        
        ///< The path to the entry (with the name)
        std::string fullpath () const{
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
    class RGPUTILS_EXPORT Folder {
        
    public:
        /**
         @brief Create a folder object with the given folder path.
         @details Constructor for a folder object.
         @param path The path to the folder that should be referenced.
         */
        Folder (const std::string &path);

        /**
         @brief Checks if the folder path is actually a folder.
         @details The existence of a folder may change while the c++ object
         still exist. You can also check this just after creating the object to
         check that the path really
         point to a folder.
         @return true if the folder path points currently to a folder.
        */
        bool isFolder () const;
        
        /**
         @brief List of all entries in the folder.
         @details This won't list any files from inside child folders.
         You have to iterate through the child folders if you want to get the
         files from there too.
         @return Shared Pointer to a vector that holds all the entries.
         On error the pointer will be a nullptr.
         */
        std::shared_ptr<std::vector<FolderEntry>> listEntries () const;

        /**
         @brief The path to this folder object.
        */
        std::string path () const {
            return _path;
        };

        /**
         @brief Holds the path separator for the current operating system.
         @details Contains "/" on Unix and "\" on Windows.
        */
        static std::string pathSeparator ();

        /**
         @brief Create a folder at a given path.
         @param path The path to the folder that should be created.
         @return Folder object to the created folder or nullptr on error.
         */
        static std::shared_ptr<Folder> createFolder (const std::string &path);

        /**
         @brief Creates a subfolder with the given name.
         @param name The name of the folder that should be created.
         @return Folder object to the created folder or nullptr on failure.
         */
        std::shared_ptr<Folder> createSubFolder (const std::string &name);

        /**
         @brief Gets an os specific folder for a given use case.
         @details This Method gets the folder for a given use case
         (f.e.: home folder: /home/user on linux or /Users/user on max os x)
         @param type The folder type (use case of the folder)
         @return Folder object or nullptr on failure
         */
        static std::shared_ptr<Folder> getFolder (const FolderType &type);
        
    private:
        std::string _path;
        
        // Don't allow creating an object without a path
        Folder() = delete;
    };
    
    /**
     @brief Folder Exception class.
     @details Will be thrown on error.
     */
    class RGPUTILS_EXPORT FolderException : std::exception {
        
    public:
        FolderException() {};
        FolderException(std::string exception) : _exceptionString(exception) {};
        ~FolderException() throw () {};

        const char* what() const throw() {
            return _exceptionString.c_str();
        };
        
    private:
        std::string _exceptionString { "Unknown Error" };
    };
}

#endif // defined(__RGPUtils__Folder_H__) header guard
