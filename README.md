RGPUtils
=======
A Collection of lightweight Classes to assist in common tasks.  
Supported platforms are Linux, Mac OS X and Windows.

Current Modules are:  
* Log    - A Singleton Class that provides thread-safe logging (output or logfile).
* Config - Reads in a config file and provides access to the values via a dictionary (std::map).
* Folder - Provides a platform independent way of accessing folders.

Installation
=======
There is a provided CMake configure file.  
#### Linux / Mac OS X  ####
On Unix you can use the following example commands (from within the project folder):
```
mkdir build
cd build
cmake -i ..
make
sudo make install
```
#### Windows  ####
On Windows you can create a Visual Studio project which will produce the dll.

For more information please visit: http://www.cmake.org  
There is also a very easy to use GUI Application for all platforms.

LICENSE
=======
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
