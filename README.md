# Woodpecker

A 3D solid modelling system, designed to aid in woodworking.

## License

Copyright © 2021 Jesse Stricker

Woodpecker is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or
(at your option) any later version.

Woodpecker is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
Woodpecker. If not, see <https://www.gnu.org/licenses/>.

## Development

This project is build with CMake. It uses Qt as one of its dependencies, so for
CMake to find the Qt library files, the prefix path variable must be set.  
To do that, create a file `cmake/QtEnv.cmake` and set the `QT_INSTALL_DIR`
variable there, for example:

```cmake
set(QT_INSTALL_DIR "C:/Qt/6.2.1/msvc2019_64")
```

This file is included by the main `CMakeLists.txt` and appends the Qt
installation directory to the CMake prefix path.

In addition to that, when running the executable `woodpecker_app.exe` on
Windows, you need to set the `PATH` variable to include `${QT_INSTALL_DIR}/bin`
directory. Unfortunately, this can not be done via CMake, instead set this in
your IDE when debugging. For example, in CLion this can be set in the
*Environment* configuration field in the *Run Configurations*.
