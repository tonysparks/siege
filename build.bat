@echo off

del /S /Q "siege.exe" >nul 2>&1
del /S /Q "*.ilk" >nul 2>&1
del /S /Q "*.pdb" >nul 2>&1

set exe_name=siege.exe
set output=-o ./bin/%exe_name%
set compiler_flags=-Wdeprecated-declarations -Wall -pedantic-errors -Wno-error=unused-variable -Wextra -Wunreachable-code -Wuninitialized -Wpragmas
set libs=-lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lphysfs
set include_dir=-I./include
set lib_dir=-L./lib
set src=./src/main.c ^
    ./src/common.c ^
    ./src/file_system.c ^
    ./src/texture_manager.c ^
    ./src/renderer.c ^
    ./src/input_system.c ^
    ./src/console.c ^
    ./src/sound_manager.c ^
    ./src/animation.c ^
    ./src/particle_system.c ^
    ./src/model.c ^
    ./src/world.c ^
    ./src/game.c

gcc -std=c11 -g %src% %output% %include_dir% %lib_dir% %libs% %compiler_flags%
cd bin
%exe_name% %*