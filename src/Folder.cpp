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

rgp::Folder::Folder(const std::string &path) : _path(path)
{
    // TODO: check if _path points to a directory
    // throw an exception if _path is not readable or not a directory

#if defined(_WIN32)
    if (!(GetFileAttributes(_path.c_str()) & FILE_ATTRIBUTE_DIRECTORY)) {
        std::stringstream message;
        message << "Path \"" << path << "\"  is not a directory." << std::endl << std::flush;
        throw FolderException { message.str() };
    }
#endif // defined(_WIN32)

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
        if (bufLen) {
            LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
            std::string result(lpMsgStr, lpMsgStr + bufLen);

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

    // get the first file from the folder
    hFind = FindFirstFile(_path.c_str(), &ffd);
    if (hFind == INVALID_HANDLE_VALUE)  {
        throw FolderException { GetLastErrorAsString() };
        return list;
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
        
    } while (FindNextFile(hFind, &ffd) != 0);
    
    if (GetLastError() != ERROR_NO_MORE_FILES) {
        // TODO: throw error ?
    }
    
    // deinit data structures
    FindClose(hFind);
    hFind = INVALID_HANDLE_VALUE;
    
    return list;
}

std::shared_ptr<rgp::Folder> rgp::Folder::createFolder(const std::string &path)
{
    // create folder using WinAPI
    bool success = CreateDirectory(path.c_str(), NULL);

    // on success return the newly created folder
    if (success) {
        return std::make_shared<rgp::Folder>(rgp::Folder(path));
    }

    // on error we just return a nullptr
    return nullptr;
}

std::shared_ptr<rgp::Folder> rgp::Folder::getFolder(const FolderType &type)
{
    LPWSTR wszPath = nullptr;
    std::shared_ptr<rgp::Folder> folder;
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

        // convert LPWSTR to string
        std::stringstream ss;
        ss << wszPath << std::flush;

        // create folder object for the found directory
        folder = std::make_shared<rgp::Folder>(ss.str());

        // memory management
        CoTaskMemFree(wszPath);
    }

    return folder;
}

std::string rgp::Folder::pathSeparator()
{
    return std::string("\\");
}

#endif // // defined(__APPLE__) || defined(__unix__) // defined(_WIN32)

