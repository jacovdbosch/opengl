@echo off

set Source=.\src\game.cpp .\src\platform.cpp .\src\renderer_opengl.cpp
set ExternalSource=.\lib\glad\src\glad.c
set LibraryPaths=-L.\lib\glfw\lib-mingw-w64
set Libraries=-lglfw3 -luser32 -lgdi32
set Includes=-I.\lib\glfw\include -I.\lib\glad\include -I.\lib\glm -I.\src
set Output=-o build\opengl

g++ -g %Source% %ExternalSource% %Includes% %LibraryPaths% %Libraries% %Output%
