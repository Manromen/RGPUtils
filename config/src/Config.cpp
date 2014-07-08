/*
 Config.cpp
 
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

#include <rgp/Config.h>

#include <fstream>

using namespace rgp;

// Constructor
Config::Config(std::string configPath) : _configPath(configPath)
{
    parseConfig();
}

void Config::parseConfig()
{
    std::string line;
    std::ifstream configFile (_configPath);
    
    if (configFile.is_open()) {
        
        int lineNumber { 0 }; // tracking line number for error output
        
        while ( configFile.good() ) { // read all lines
            
            lineNumber++;
            getline (configFile,line);
            
            // skip empty lines
            if (line.size() == 0) {
                continue;
            }
            // skip comments
            if (line[0] == '#') {
                continue;
            }
            
            // TODO: erase comment in middle of line #
            size_t commentStart = line.find_first_of('#');
            if (commentStart != line.npos) {
                line.erase(commentStart);
            }
            
            // trim beginning
            size_t first = line.find_first_not_of("\t \r\n");
            if (first == line.npos) continue; // nothing useful found
            if (first != 0) {
                // erase unwanted characters from beginning
                line.erase(0, first);
            }
            
            // get key
            size_t endOfKey = line.find_first_of("\t \r\n");
            if (endOfKey == line.npos) { // not found
                throw ConfigException {
                    std::string("config file corrupt at line: ") += lineNumber
                };
            }
            
            std::string key = line.substr(0, endOfKey);
            
            // get value
            std::string value;
            
            // get begin of value
            size_t beginOfValue = line.find_first_not_of("\t \r\n",
                                                         endOfKey + 1);
            if (beginOfValue == line.npos) { // not found
                throw ConfigException {
                    std::string("config file corrupt at line: ") += lineNumber
                };
            }
            
            // check if value starts with "
            if (line[beginOfValue] == '\"') {
                size_t endOfValue = line.find_first_of("\"", beginOfValue + 1);
                
                if (endOfValue == line.npos) { // not found
                    throw ConfigException {
                        std::string("config file corrupt at line: ")
                        += lineNumber
                    };
                }
                
                // value without the quotes "
                value = line.substr(beginOfValue + 1,
                                    endOfValue - 1 - beginOfValue);
            
            } else { // don't starts with " --> value is only one word
                
                size_t endOfValue = line.find_first_of("\t \r\n", beginOfValue);
                
                if (endOfValue == line.npos) {
                    value = line.substr(beginOfValue);
                } else {
                    value = line.substr(beginOfValue,
                                        endOfValue - beginOfValue);
                }
            }
            
            // add key and value to options map
            if (key.size() > 0 && value.size() > 0) {
                
                _options[key] = value;
                
            } else {
                throw ConfigException {
                    std::string("config file corrupt at line: ") += lineNumber
                };
            }
        }
        
        configFile.close();
        
    } else throw ConfigException {
        std::string("Unable to open config file: ") += _configPath
    };
}

std::string Config::getOptionForKey(std::string key)
{
    std::map<std::string,std::string>::iterator it { _options.find(key) };
    if (it != _options.end())
        return it->second;
    
    return std::string("");
}
