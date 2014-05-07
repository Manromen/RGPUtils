/*
 Log.cpp

 Created by Ralph-Gordon Paul on 08.07.13.

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

#include <rgp/Log.h>

#include <iostream> // cout / cerr / cin ...
#include <fstream>  // file writing
#include <ctime>    // adding date to log file
#include <cstring>  // strerror

using namespace rgp;

// init instance to nullptr
Log *Log::_sharedInstance = nullptr;

Log *Log::sharedLog () {
    
    // if first used -> alloc instance
    if (_sharedInstance == nullptr) {
        _sharedInstance = new Log();
    }
    
    return _sharedInstance;
}

Loglevel Log::loglevel () const
{
    return _logLevel;
}

void Log::setLoglevel (const Loglevel level)
{
    _logLevel = level;
}

// verbose level 1 print
void Log::printv (const std::string text)
{
    if (_logLevel >= LoglevelVerbose) {
        print(text);
    }
}

// normal print
void Log::print (const std::string text)
{
    if (_logLevel >= LoglevelNormal) {
        
        _cout_mutex.lock();
        
        // use log file if possible
        if (_hasLogfile) {
            
            // open log file
            std::ofstream file;
            file.open(_logFilePath, std::ios::app);
            
            // write output to file
            if (file.is_open()) {
                
                // add time to file
                time_t timestamp = time(NULL); // get time now
                tm *current_time = localtime( &timestamp );
                file
                << (current_time->tm_year + 1900) << '-'
                << (current_time->tm_mon + 1) << '-'
                <<  current_time->tm_mday << ' '
                <<  current_time->tm_hour << ':'
                <<  current_time->tm_min << ':'
                <<  current_time->tm_sec << ' ';
                
                // append text
                file << text << std::endl;
            }
            
            // close log file
            file.close();
            
        } else {
            
            // output to stdout
            std::cout << text << std::endl << std::flush;
        }
        
        _cout_mutex.unlock();
    }
}

// outputs text and reads line from stdin
std::string Log::getline (const std::string text)
{
    char line_c[1024];
    
    _cout_mutex.lock();
    
    std::cout << text << std::flush;
    std::cin.getline(line_c, 1024);
    
    _cout_mutex.unlock();
    
    return std::string { line_c };
}

// outputs text and reads one character from stdin
char Log::getc (const std::string text)
{
    char c = ' ';
    
    _cout_mutex.lock();
    
    std::cout << text << std::flush;
    std::cin >> c;
    
    _cout_mutex.unlock();
    
    return c;
}

// error print
void Log::error (const std::string text)
{
    _cerr_mutex.lock();
    
    // use log file if possible
    if (_hasErrorfile) {
        
        // open log file
        std::ofstream file;
        file.open(_errorFilePath, std::ios::app);
        
        // write output to file
        if (file.is_open()) {
            
            // add time to file
            time_t timestamp = time(NULL); // get time now
            tm *current_time = localtime( &timestamp );
            file
            << (current_time->tm_year + 1900) << '-'
            << (current_time->tm_mon + 1) << '-'
            <<  current_time->tm_mday << ' '
            <<  current_time->tm_hour << ':'
            <<  current_time->tm_min << ':'
            <<  current_time->tm_sec << ' ';
            
            // append text
            file << text << std::endl;
        }
        
        // close log file
        file.close();
        
    } else {
        
        // output to stderr
        std::cerr << text << std::endl;
    }
    
    _cerr_mutex.unlock();
}

// error print with error number (errno)
void Log::errorWithErrno (const std::string text, const int err)
{
    // create error string with text + errno
    std::string error_string { text };
    error_string += " ";
    error_string += strerror(err);
    
    // output error with errno error string
    error( error_string );
}

// using log file for print
void Log::useLogfile (const std::string filePath)
{
    _logFilePath = filePath;
    
    // check if file is usable
    std::ofstream file;
    file.open(_logFilePath, std::ios::app);
    if (file.is_open()) {
        _hasLogfile = true;
    }
    file.close();
}

// using error file
void Log::useErrorfile (const std::string filePath)
{
    _errorFilePath = filePath;
    
    // check if file is usable
    std::ofstream file;
    file.open(_errorFilePath, std::ios::app);
    if (file.is_open()) {
        _hasErrorfile = true;
    }
    file.close();
}
