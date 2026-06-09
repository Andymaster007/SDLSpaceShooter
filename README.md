# SDLSpaceShooter

基于 SDL2 的 2D 太空射击游戏。

## 环境要求

- CMake >= 4.1
- C++17 兼容编译器（MSVC / GCC / Clang）
- SDL2、SDL2_image、SDL2_ttf、SDL2_mixer
  （Windows 上已安装到 `C:/Library/SDL2`）

## 编译

```bash
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=C:/Library/SDL2
cmake --build .
```
