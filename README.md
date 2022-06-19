
## Installation 

Install [msys2](https://www.msys2.org) and add `{msys64dir}\mingw64\bin\` to `$PATH`.

### Install using msys2

```
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-glew
pacman -S mingw-w64-x86_64-glfw
pacman -S mingw-w64-x86_64-glm
pacman -S mingw-w64-x86_64-assimp
```

### Compile 

Copy specified dlls inside `{project}\bin\`. DLLs are located under `{msys64dir}\mingw64\bin\`
  - `glew32.dll`
  - `glfw3.dll`
  - `libassimp-5.dll`

Compile code using: 

```
g++ .\src\*.cpp -o .\bin\main.exe -llibassimp -lglfw3 -lglew32 -lopengl32
```

Execute in `.\bin\` folder:

```
cd bin 
.\main.exe
```


