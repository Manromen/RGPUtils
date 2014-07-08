/*
 Log.h
 
 Created by Ralph-Gordon Paul on 08. July 2013.
 
 A Singleton Log Class to use for coordinated multithreaded logging
 (output or logfile)
 
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

#ifndef __RGPUtils__Log__
#define __RGPUtils__Log__

#include <mutex>
#include <atomic>
#include <string>

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

// hide debug information (and string data) for better security
// (less information for an attacker)
#ifdef DEBUG
#define RGPLOG(xx) rgp::Log::sharedLog()->print(xx)
#else
#define RGPLOG(xx) ((void)0)
#endif // DEBUG

// hide debug information (and string data) for better security
// (less information for an attacker)
// verbose version of RGPLOG
#ifdef DEBUG
#define RGPLOGV(xx) rgp::Log::sharedLog()->printv(xx)
#else
#define RGPLOGV(xx) ((void)0)
#endif // DEBUG

// hide debug information (and function name) for better security
// (less information for an attacker)
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
    
    /** Describes a Loglevel. */
    typedef enum : uint8_t {
        /** Turns logging off. Nothing will be outputted. */
        LoglevelOff = 0,
        /** Turns on output for normal logs. */
        LoglevelNormal,
        /** Verbose mode. Typically used for more detailed logs. */
        LoglevelVerbose
    } Loglevel;
    
    /** Describes a ANSI Foregorund Color used for
     SGR (Select Graphic Rendition) parameters */
    typedef enum : uint8_t {
        /** terminal default -> will use terminal setting */
        AnsiSgrFgColorDefault = 39,
        AnsiSgrFgColorBlack = 30,
        AnsiSgrFgColorRed,
        AnsiSgrFgColorGreen,
        AnsiSgrFgColorYellow,
        AnsiSgrFgColorBlue,
        AnsiSgrFgColorMagenta,
        AnsiSgrFgColorCyan,
        AnsiSgrFgColorGray,
        AnsiSgrFgColorDarkGray = 90,
        AnsiSgrFgColorLightRed,
        AnsiSgrFgColorLightGreen,
        AnsiSgrFgColorLightYellow,
        AnsiSgrFgColorLightBlue,
        AnsiSgrFgColorLightMagenta,
        AnsiSgrFgColorLightCyan,
        AnsiSgrFgColorWhite
    } AnsiSgrFgColor;
    
    /** Describes a ANSI Background Color used for
     SGR (Select Graphic Rendition) parameters */
    typedef enum : uint8_t {
        /** terminal default -> will use terminal setting */
        AnsiSgrBgColorDefault = 49,
        AnsiSgrBgColorBlack = 40,
        AnsiSgrBgColorRed,
        AnsiSgrBgColorGreen,
        AnsiSgrBgColorYellow,
        AnsiSgrBgColorBlue,
        AnsiSgrBgColorMagenta,
        AnsiSgrBgColorCyan,
        AnsiSgrBgColorGray,
        AnsiSgrBgColorDarkGray = 100,
        AnsiSgrBgColorLightRed,
        AnsiSgrBgColorLightGreen,
        AnsiSgrBgColorLightYellow,
        AnsiSgrBgColorLightBlue,
        AnsiSgrBgColorLightMagenta,
        AnsiSgrBgColorLightCyan,
        AnsiSgrBgColorWhite
    } AnsiSgrBgColor;
    
    /**
     @brief A Singleton Log class for thread-safe logging
     @details This class uses std::cout / std::cerr for log and error outputs.
     */
    class RGPUTILS_EXPORT Log {
        
    public:
        
        /** 
         @brief The Log class is build as a singleton.
         @details There is only one instance at a time. This method will give you this instance.
         On first call this will create the singleton instance.
         @return The shared log object.
         */
        static Log *sharedLog ();
        
        /**
         @brief The current LogLevel (Default: LoglevelNormal).
         @return The current LogLevel.
         @sa setLoglevel()
         */
        Loglevel loglevel () const;
        
        /**
         @brief Sets a new LogLevel.
         @details The loglevel describes how detailed the output should be.
         This will affect output to logfiles in the same way.
         Input methods like getline or getc will be unaffected.
         @param level The new loglevel.
         @sa loglevel(), print() and printv()
         */
        void setLoglevel (const Loglevel level);
        
        /**
         @brief This will logout the given text.
         @details The given text will be printed to std::cout if the required
         loglevel (LoglevelNormal or higher) is set. If there is a logfile
         specified, the output will be written to that file instead.
         @param text The text that should be logged out.
         @param fgcolor The foreground color of the text (only for supported
         terminals). This parameter is optional.
         @param bgcolor The background color of the text (only for supported
         terminals). This parameter is optional.
         @sa loglevel() and useLogfile()
         */
        void print (const std::string text,
                    const AnsiSgrFgColor fgcolor=AnsiSgrFgColorDefault,
                    const AnsiSgrBgColor bgcolor=AnsiSgrBgColorDefault);
        
        /**
         @brief This will logout the given text.
         @details The given text will be printed to std::cout if the required
         loglevel (LoglevelVerbose or higher) is set. If there is a logfile
         specified, the output will be written to that file instead.
         @param text The text that should be logged out.
         @param fgcolor The foreground color of the text (only for supported
         terminals). This parameter is optional.
         @param bgcolor The background color of the text (only for supported
         terminals). This parameter is optional.
         @sa loglevel() and useLogfile()
         */
        void printv (const std::string text,
                     const AnsiSgrFgColor fgcolor = AnsiSgrFgColorDefault,
                     const AnsiSgrBgColor bgcolor = AnsiSgrBgColorDefault);
        
        /**
         @brief Read a line from std::cin.
         @details Shows a given text and wait's on std::cin till the user gave
         some input. All output through std::cout will be blocked until this
         method returns. Output to std::cerr will still be shown.
         @param text The text that will be shown before the user can input his
         text.
         @return The input from std::cin. The input will be a whole line.
         @sa getc()
         */
        std::string getline (const std::string text);
        
        /**
         @brief Read a character from std::cin.
         @details Shows a given text and wait's on std::cin till the user gave
         his input. All output through std::cout will be blocked until this
         method returns. Output to std::cerr will still be shown.
         @param text The text that will be shown before the user can input a
         character.
         @return The input from std::cin. The input just be a character.
         @sa getc()
         */
        char getc (const std::string text);
        
        /**
         @brief This will logout the given text.
         @details The given text will be printed to std::cerr. If there is a
         errorfile specified, the output will be written to that file instead.
         @param text The text that should be logged out.
         @sa errorWithErrno() and useErrorfile()
         */
        void error (const std::string text);
        
        /**
         @brief This will logout the given text with the given errno code.
         @details The given text will be printed to std::cerr. The text from the
         errno code will be appended. If there is a errorfile specified, the
         output will be written to that file instead.
         @param text The text that should be logged out.
         @param err The errno code. Will be used with strerror() to generate the
         error text.
         @sa error() and useErrorfile()
         */
        void errorWithErrno (const std::string text, const int err);
        
        /**
         @brief Set logfile for output.
         @details If this logfile is set, all outputs that would go to std::cout
         will go into this file instead (except input methods like getline).
         @param filePath The path to the file that should be used.
         Preferably the full path.
         @sa useErrorfile()
         */
        void useLogfile (const std::string filePath);
        
        /**
         @brief Set logfile for error output.
         @details If this logfile is set, all outputs that would go to std::cerr
         will go into this file instead.
         @param filePath The path to the file that should be used.
         Preferably the full path.
         @sa useLogfile()
         */
        void useErrorfile (const std::string filePath);

        /**
        @brief Enables or disables the use of ANSI SGR Codes.
        @details The Terminal have to support the ANSI SGR Codes. Default: Disabled.
        @param useAnsiSgrCodes Setting this to true will enable the usage of ANSI SGR Codes.
        @sa useAnsiSgrCodes()
        */
        void setUseAnsiSgrCodes (const bool useAnsiSgrCodes);

        /**
        @brief Determines if ANSI SGR Codes are enabled.
        @return True if ANSI SGR Codes are enables, false otherwise.
        @sa setUseAnsiSgrCodes()
        */
        bool useAnsiSgrCodes () const;
        
    private:
        
        // make constructor private (we are a singleton class)
        Log () {}
        
        // disallow copy constructor
        Log (const Log &log) = delete;
        Log &operator = (Log const &) = delete;
        
        // singleton instance
        static Log *_sharedInstance;
        
        // protect cout with a mutex to make it thread-safe
        std::mutex _cout_mutex;
        
        // protect cerr with a mutex to make it thread-safe
        std::mutex _cerr_mutex;
        
        // store the current loglevel
        std::atomic<Loglevel> _logLevel { LoglevelNormal };
        
        // if we are able to use the given logfile, this variable will be true
        std::atomic<bool> _hasLogfile { false };
        
        // stores the path to the given logfile
        std::string _logFilePath { "" };
        
        // if we are able to use the given error logfile,
        // this variable will be true
        std::atomic<bool> _hasErrorfile { false };
        
        // stores the path to the given error logfile
        std::string _errorFilePath { "" };

        // determines if ANSI SGR Codes should be used or not
        bool _useAnsiSgrCodes { false };
        
        // creates ANSI SGR Code for given foreground and background colors
        std::string createSelectGraphicRenditionCode(const AnsiSgrFgColor fgcolor,
                                                     const AnsiSgrBgColor bgcolor);
        // creates ANSI SGR Reset Code
        std::string resetSelectGraphicRenditionCode();
    };
    
    /** Exception class for Log */
    class RGPUTILS_EXPORT LogException : std::exception {
        
    public:
        LogException () {};
        LogException (std::string exception) : _exceptionString(exception) {};
        
        std::string what () { return _exceptionString; };
        virtual const char *what () const throw() {
            return _exceptionString.c_str();
        };
        
    private:
        std::string _exceptionString { "Unknown Error" };
    };
}

#endif // defined(__RGPUtils__Log__) header guard
