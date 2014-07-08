/*
 RGPUtils
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

#if defined(_WIN32)
// on windows we need to convert wstring to string
#include <locale>
#include <codecvt>
std::string to_string (const std::wstring &origString);
#endif // defined(_WIN32)

rgp::Folder::Folder(const std::string &path) : _path(path)
{
}

bool rgp::Folder::isFolder () const
{
#if defined(__APPLE__) || defined(__unix__)
    
    struct stat statbuf;
    stat(_path.c_str(), &statbuf);
    return S_ISDIR(statbuf.st_mode);
    
#elif defined(_WIN32)
    
    if ((GetFileAttributes (_path.c_str ()) & FILE_ATTRIBUTE_DIRECTORY)) {
        return true;
    }
#endif // defined(__APPLE__) || defined(__unix__) // defined(_WIN32)
    
    return false;
}

std::string rgp::Folder::pathSeparator()
{
#if defined(__APPLE__) || defined(__unix__)
    // we are on a unix system - so return a / as result
    return std::string("/");

#elif defined(_WIN32)
    // we are on windows - so we return a \ as result
    return std::string("\\");
#endif // defined(__APPLE__) || defined(__unix__) // defined(_WIN32)
}

std::shared_ptr<rgp::Folder> rgp::Folder::createFolder(const std::string &path)
{
#if defined(__APPLE__) || defined(__unix__)
    
    // create new folder with mkdir()
    if (mkdir(path.c_str(), 0777) == 0) {
        // on success return the newly created folder
        return std::make_shared<rgp::Folder>(rgp::Folder(path));
    } // if folder already existent, just return a folder from that
    else if (errno == EEXIST) {
        return std::make_shared<rgp::Folder>(rgp::Folder(path));
    }
    
#elif defined(_WIN32)
    
    // create folder using WinAPI
    BOOL success = CreateDirectory(path.c_str(), NULL);
    
    // on success (or already existent folder) return the newly created folder
    if (success == TRUE || GetLastError() == ERROR_ALREADY_EXISTS) {
        return std::make_shared<rgp::Folder>(rgp::Folder(path));
    }
#endif // defined(__APPLE__) || defined(__unix__) // defined(_WIN32)
    
    // on error we just return a nullptr
    return nullptr;
}

std::shared_ptr<rgp::Folder>
rgp::Folder::createSubFolder (const std::string &name)
{
    std::shared_ptr<rgp::Folder> subFolder {
        rgp::Folder::createFolder (_path + pathSeparator() + name)
    };
    
    return subFolder;
}

std::shared_ptr<rgp::Folder> rgp::Folder::getFolder(const FolderType &type)
{
    std::shared_ptr<rgp::Folder> folder;
    
#if defined(__APPLE__) || defined(__unix__)
    
    struct passwd *passwdEnt = getpwuid(getuid());
    
    switch (type)
    {
        case FolderTypeAppData: {
#if defined(__APPLE__)
            #include "TargetConditionals.h"
#if TARGET_OS_MAC
            std::string path {passwdEnt->pw_dir};
            path += "/Library/Application Support";
            return std::make_shared<rgp::Folder>(path);
#endif // TARGET_OS_MAC
#endif // defined(__APPLE__)
#if defined(__linux__)
            // get executable name from /proc filesytem
            size_t exepath_size = 2048;
            char exepath[exepath_size];
            ssize_t writtenBytes = readlink("/proc/self/exe",
                                            exepath,
                                            exepath_size);

            // error check
            if (writtenBytes > 0) {
                // readlink don't adds the terminating symbol '\0'
                exepath[writtenBytes] = '\0';
                
                // we just want the name
                char *exename { nullptr };

                // determine the name by looking for '/' from the end to the
                // beginning
                for (int i=writtenBytes; i>0; i++) {
                    if (exepath[i] == '/') {
                        // we found the start of the executable name
                        exename = &exepath[i+1];
                        break;
                    }
                }
                
                // home folder + . + executable name will form our config path
                // f.e. /home/user/.appname
                std::string path {passwdEnt->pw_dir};
                path += "/.";
                path += std::string(exename);
                
                // we may need to create the folder
                return rgp::Folder::createFolder(path);
            }
#endif // defined(__linux__)
        } break;
        
        case FolderTypeHome: {
            return std::make_shared<rgp::Folder>(passwdEnt->pw_dir);
        } break;
            
        default: break;
    }
    
#elif defined(_WIN32)
    LPWSTR wszPath = nullptr;
    switch (type)
    {
        case FolderTypeAppData: {
            SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &wszPath);
        } break;
            
        case FolderTypeHome: {
            SHGetKnownFolderPath(FOLDERID_Profile, 0, NULL, &wszPath);
        } break;
            
        default: break;
    }
    
    // check if something was found
    if (wszPath != nullptr) {
        
        // convert LPWSTR to wstring
        std::wstring path { wszPath };
        
        // create folder object for the found directory
        folder = std::make_shared<rgp::Folder>(to_string(path));
        
        // memory management
        CoTaskMemFree(wszPath);
    }
#endif // defined(__APPLE__) // defined(__unix__) // defined(_WIN32)
    
    return folder;
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
            FolderEntry entry;
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

// Windows version
#elif defined(_WIN32)

// on windows we need to convert wstring to string
std::string to_string (const std::wstring &origString)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    return std::string(converter.to_bytes (origString));
}

// create a string with last error message
std::string GetLastErrorAsString()
{
    DWORD error = GetLastError();
    if (error) {
        LPVOID lpMsgBuf;
        DWORD bufLen = FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0, NULL);
        if (bufLen != 0) {
            LPCSTR lpMsgStr = (LPCSTR) lpMsgBuf;
            std::string result { lpMsgStr };

            LocalFree(lpMsgBuf);
            return result;
        }
    }
    return std::string();
}

std::shared_ptr<std::vector<FolderEntry>> rgp::Folder::listEntries() const
{
    std::shared_ptr<std::vector<FolderEntry>> list {
        std::make_shared<std::vector<FolderEntry>>()
    };
    
    // create structures to gather data
    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA ffd;

    // we need to search for all files inside our folder
    // so we need a search string like C:\our\folder\*
    std::string searchString { _path + "\\*" };

    // get the first file from the folder
    hFind = FindFirstFile(searchString.c_str(), &ffd);
    if (hFind == INVALID_HANDLE_VALUE)  {
        return nullptr;
    }
    
    // iterate through folder and fill the list vector
    do {
        FolderEntry entry;
        
        entry._name = ffd.cFileName;
        entry._type = EntryTypeUnknown;
        entry._path = _path;
        entry._fullpath = _path + std::string("\\") + ffd.cFileName;
        
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            entry._type = EntryTypeFolder;
        }
        else if (ffd.dwFileAttributes & FILE_ATTRIBUTE_NORMAL) {
            entry._type = EntryTypeRegularFile;
        }

        // add entry to the list
        list->push_back (entry);
        
    } while (FindNextFile(hFind, &ffd) != 0);
    
    if (GetLastError() != ERROR_NO_MORE_FILES) {
        // TODO: throw error ?
    }
    
    // deinit data structures
    FindClose(hFind);
    hFind = INVALID_HANDLE_VALUE;
    
    return list;
}

#endif // defined(__APPLE__) || defined(__unix__) // defined(_WIN32)

