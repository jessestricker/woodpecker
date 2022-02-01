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

This project is build with CMake and uses Qt6 and Boost as its dependencies,
so you must have these installed.
On some operating systems (especially Windows), CMake can not find these dependencies on its own.
Set the following variables using the `-D<name>=<value>` flag when running CMake:

- for Qt, `CMAKE_PREFIX_PATH`: path to compiler-specific library directory, e.g. `C:\Qt\6.2.1\msvc2019_64`
- for Boost, `BOOST_ROOT`: path to root directory of the installation, e.g. `C:\Libs\boost_1_78_0`

See [top-level build file](CMakeLists.txt) for the minimum required versions.

## Running

On Windows, you need to set the `PATH` variable to include `<qt_path>/bin`
directory. Unfortunately, this can not be easily done with CMake. Instead, set this in
your IDE settings. For example, in JetBrains CLion this can be set in the
_Environment_ text field in the _Run/Debug Configurations_ window.
