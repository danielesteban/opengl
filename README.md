# opengl

> My attempt at a "modern" c++ opengl engine

### install deps & compile
```bash
git clone --recursive https://github.com/danielesteban/opengl.git engine
cd engine
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
mkdir build
cd build
conan install ..
cmake .. -G "Visual Studio 15 Win64"
cmake --build . --config Release
```
