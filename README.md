# GotoXY
It makes Some functions of Windows.h (about console) available on POSIX Terminal!

In XCode, you should follow this answer (because of XCode default terminal)
https://stackoverflow.com/questions/19554822/xcode-command-line-tool-how-to-run-in-terminal

It was written because of Programming class in Dimigo.

This projects only contains functions used in Programming class in Dimigo, so there might be functions that is not perfectly implemented.


Written by Donghyun Lee (KDMHS 21WP)

# Available functions

- SetConsoleCursorPosition

- SetConsoleCursorInfo

- SetConsoleTextAttribute

- Sleep

- _kbhit, _getch


# How should I use this?

- First, you should change 
   #include <Windows.h>
to
   #include "Windows.h"
   
- You should replace every system("cls") to system("clear")

- system("mode con ....") is not available on posix terminal. you should erase that. 
