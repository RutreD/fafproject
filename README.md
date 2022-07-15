[![Windows](https://github.com/RutreD/fafproject/actions/workflows/windows-build.yml/badge.svg)](https://github.com/RutreD/fafproject/actions/workflows/windows-build.yml)
# fafproject

Now it's just a toy and a little demonstration of what it can do.
In addition to ImGui, it can hook any function of the game runtime, you only need the address and the prototype. 
Full access to the DirectD3D9 device, you can modify the texture (recolor for example), prevent it from rendering or add something to it(look at ultramegaagrresivefpsup.hpp for example).

# How to test

* Install an ASI-Loader. For example: https://github.com/ThirteenAG/Ultimate-ASI-Loader/releases/download/Win32-latest/d3d9.zip , put to C:\ProgramData\FAForever\bin
* Download project.asi from release or build it yourself and put to C:\ProgramData\FAForever\bin or C:\ProgramData\FAForever\bin\plugins

# Building

* clone 
  * `git clone --recursive https://github.com/RutreD/fafproject.git`
* compile
  * need visual studio compiler with support C++17 and cmake
  * open folder in visual studio or visual studio code
  * set x86 architecture and build
