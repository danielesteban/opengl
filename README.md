opengl
[![Build Status](https://travis-ci.org/danielesteban/opengl.svg?branch=master)](https://travis-ci.org/danielesteban/opengl)
===

> My attempt at a "modern" c++ opengl engine

```bash
# download source
git clone --recursive https://github.com/danielesteban/opengl.git engine
cd engine
# install deps & build
mkdir build
cd build
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
conan install ..
cmake .. -G "Visual Studio 15 Win64"
cmake --build . --config Release
# demo scene
./bin/engine.exe
```
