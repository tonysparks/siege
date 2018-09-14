@echo off

del /S /Q "siege.exe" >nul 2>&1
del /S /Q "*.ilk" >nul 2>&1
del /S /Q "*.pdb" >nul 2>&1

REM -Wconversion
REM -Wdeprecated-declarations -Wall -pedantic-errors -Wno-error=unused-variable -Wextra -Wunreachable-code -Wuninitialized -Wpragma-pack
clang -std=c11  -g ./src/main.c -o ./bin/siege.exe -I./include -L./lib -lSDL2main -lSDL2 -lSDL2_image
cd bin/
siege.exe %*