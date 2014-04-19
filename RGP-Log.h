/*
 RGP-Log.h

 Created by Ralph-Gordon Paul on 08.07.13.

 A Singleton Log Class to use for coordinated multithreaded logging
 (output or logfile)

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

#ifndef __RGP__Log__
#define __RGP__Log__

#include <mutex>
#include <atomic>
#include <string>

// hide debug information (and string data) for better security (less information for an attacker)
#ifdef DEBUG
#define RGPLOG(xx) rgp::Log::sharedLog()->print(xx)
#else
#define RGPLOG(xx) ((void)0)
#endif // DEBUG

// hide debug information (and string data) for better security (less information for an attacker)
// verbose version of RGPLOG
#ifdef DEBUG
#define RGPLOGV(xx) rgp::Log::sharedLog()->printv(xx)
#else
#define RGPLOGV(xx) ((void)0)
#endif // DEBUG

// hide debug information (and function name) for better security (less information for an attacker)
#ifdef DEBUG
#define RGPLOGMETHODNAME() RGPLOG(__PRETTY_FUNCTION__)
#else
#define RGPLOGMETHODNAME() ((void)0)
#endif // DEBUG

// log errno
#ifdef DEBUG
#define RGPLOG_ERRNO(xx,yy) rgp::Log::sharedLog()->errorWithErrno(xx, yy)
#else
#define RGPLOG_ERRNO(xx,yy) ((void)0)
#endif // DEBUG

#ifdef DEBUG
#define RGPLOG_ERROR(xx) rgp::Log::sharedLog()->error(xx)
#else
#define RGPLOG_ERROR(xx) ((void)0)
#endif // DEBUG

namespace rgp {
    
    class Log {
        
    public:
        // log level specifies verbose mode
        // logLevel = 0 -> normal mode
        // logLevel = 1 -> verbose mode
        std::atomic<int> logLevel { 0 };
        
        // singleton
        static Log *sharedLog();
        
        void printv(const std::string text);  // verbose print
        void print(const std::string text);   // normal print
        
        std::string getline( const std::string text ); // outputs text and reads line from stdin
        char getc(const std::string text); // outputs text and reads one character from stdin
        
        void error(const std::string text);                   // error print
        void errorWithErrno( const std::string text, int err); // print errno
        
        void useLogfile( const std::string filePath );  // using log file for print
        void useErrorfile( const std::string filePath );// using error file for error print
        
    private:
        Log() {}                // disallow constructor from outside
        Log(const Log &log) = delete;    // disallow copy constructor
        Log &operator = (Log const &) = delete;
        
        static Log *_sharedInstance;   // instance of singleton
        
        std::mutex _cout_mutex;  // protect cout to make it thread-safe
        std::mutex _cerr_mutex;  // protect cerr to make it thread-safe
        
        // using log file
        std::atomic<bool> _hasLogfile { false };
        std::string _logFilePath { "" };
        
        // using error file
        std::atomic<bool> _hasErrorfile { false };
        std::string _errorFilePath { "" };
    };
    
    /** Exception Class **/
    class LogException : std::exception {
        
    public:
        LogException() {};
        LogException(std::string exception) : _exceptionString(exception) {};
        
        std::string what() { return _exceptionString; };
        virtual const char *what() const throw() { return _exceptionString.c_str(); };
        
    private:
        std::string _exceptionString { "Unknown Error" };
    };
}

#endif // /* defined(__RGP__Log__) */
