/*
 RGP-Log.cpp

 Created by Ralph-Gordon Paul on 08.07.13.

 -------------------------------------------------------------------------------
 The MIT License (MIT)
 
 Copyright (c) 2013 Ralph-Gordon Paul. All rights reserved.
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 -------------------------------------------------------------------------------
*/


#include "RGP-Log.h"
#include <iostream> // cout / cerr
#include <fstream>  // file writing
#include <ctime>    // adding date / time to log file
#include <cstring>  // strerror

using namespace rgp;

// init instance to nullptr
Log *Log::_sharedInstance = nullptr;

Log *Log::sharedLog() {
    
    // if first used -> alloc instance
    if (_sharedInstance == nullptr) {
        _sharedInstance = new Log();
    }
    
    return _sharedInstance;
}

// verbose level 1 print
void Log::printv(const std::string text)
{
    if (logLevel >= 1) {
        print(text);
    }
}

// normal print
void Log::print(const std::string text)
{
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

// outputs text and reads line from stdin
std::string Log::getline( const std::string text )
{
    char line_c[1024];
    
    _cout_mutex.lock();
    
    std::cout << text << std::flush;
    std::cin.getline(line_c, 1024);
    
    _cout_mutex.unlock();
    
    return std::string { line_c };
}

// outputs text and reads one character from stdin
char Log::getc( const std::string text )
{
    char c = ' ';
    
    _cout_mutex.lock();
    
    std::cout << text << std::flush;
    std::cin >> c;
    
    _cout_mutex.unlock();
    
    return c;
}

// error print
void Log::error(const std::string text)
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
void Log::errorWithErrno(const std::string text, int err)
{
    // create error string with text + errno
    std::string error_string { text };
    error_string += " ";
    error_string += strerror(err);
    
    // output error with errno error string
    error( error_string );
}

// using log file for print
void Log::useLogfile( const std::string filePath )
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
void Log::useErrorfile( const std::string filePath )
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
