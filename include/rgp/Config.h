/*
Config.h

Created by Ralph-Gordon Paul on 08. June 2013.

-------------------------------------------------------------------------------
GNU Lesser General Public License Version 3, 29 June 2007

Copyright (c) 2013 Ralph-Gordon Paul. All rights reserved.

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

#ifndef __RGPConfig__Config_H__
#define __RGPConfig__Config_H__

#include <iostream>
#include <string>
#include <map>

namespace rgp {
    
    class Config {
        
    public:
        // create a config object with the given path to the config file
        // throws ConfigException on error
        Config(std::string configPath);
        
        // get a config value for given key
        std::string getOptionForKey(std::string key);
        
    private:
        std::string _configPath;
        std::map<std::string,std::string> _options;
        void parseConfig();
    };
    
    /** Exception Class **/
    class ConfigException : std::exception {
        
    public:
        ConfigException() {};
        ConfigException(std::string exception) : _exceptionString(exception) {};
        
        std::string what() { return _exceptionString; };
        virtual const char* what() const throw() {
            return _exceptionString.c_str();
        };
        
    private:
        std::string _exceptionString { "Unknown Error" };
    };
}

#endif /* defined(__RGPConfig__Config_H__) */
