opengl
[![Build Status](https://travis-ci.org/danielesteban/opengl.svg?branch=master)](https://travis-ci.org/danielesteban/opengl)
===

> My attempt at a "modern" c++ opengl engine

[![screenshot](https://user-images.githubusercontent.com/806111/71917650-d35ec600-3180-11ea-993d-5abd4aefbc9f.png)](https://github.com/danielesteban/opengl/releases/latest)

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
