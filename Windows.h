//
//  Windows.h
//  WindowsH
//
//  Created by 이동현 on 2022/11/04.
//
//  Windows.h Console Compatible Library for macOS
//  v1.0
//
//  https://stackoverflow.com/questions/19554822/xcode-command-line-tool-how-to-run-in-terminal
//  In XCode, You should use this to open in terminal!
//  (It seems XCode default terminal doesn't support console methods)
//

#ifndef Windows_h
#define Windows_h

#include <unistd.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <termios.h>

#define FALSE 0
#define TRUE 1

#define STD_OUTPUT_HANDLE 1

#define FOREGROUND_BLUE      0x0001 // text color contains blue.
#define FOREGROUND_GREEN     0x0002 // text color contains green.
#define FOREGROUND_RED       0x0004 // text color contains red.
#define FOREGROUND_INTENSITY 0x0008 // text color is intensified.
#define BACKGROUND_BLUE      0x0010 // background color contains blue.
#define BACKGROUND_GREEN     0x0020 // background color contains green.
#define BACKGROUND_RED       0x0040 // background color contains red.
#define BACKGROUND_INTENSITY 0x0080 // background color is intensified.

typedef struct
{
    int x, y;
} COORD;

typedef struct
{
    int bVisible;
    int dwSize;
} CONSOLE_CURSOR_INFO;

int GetStdHandle(int a)
{
    return 1;
}

void SetConsoleCursorPosition(int handle, COORD pos)
{
    printf("%c[%d;%df", 0x1B,  pos.y+1, pos.x+1);
}

void SetConsoleCursorInfo(int handle, CONSOLE_CURSOR_INFO* c)
{
    if(c->bVisible)
    {
        printf("%c[?25h", 0x1B);
    }
    else
    {
        printf("%c[?25l", 0x1B);
    }
    
    return;
}

void printColorString(int color)
{
    printf("\033[%dm", color);
}

const int dcolor[] = { 0, 4, 2, 6, 1, 5, 3, 7, 0, 4, 2, 6, 1, 5, 3, 7 };

void ColorChange(unsigned short color)
{
    int FG = (color & 0x000F);
    int BG = (color & 0x00F0) >> 4;

    int FG_isIntensity = (color & FOREGROUND_INTENSITY) ? 60 : 0;
    int BG_isIntensity = (color & BACKGROUND_INTENSITY) ? 60 : 0;

    int result_foreground = 30 + FG_isIntensity, result_background = 40 + BG_isIntensity;

    result_foreground += dcolor[FG];
    result_background += dcolor[BG];

    
    printColorString(result_foreground);
    printColorString(result_background);
}

void SetConsoleTextAttribute(int handle, int color)
{
    ColorChange(color);
}

void Sleep(int ms)
{
    fflush(stdout);
    usleep(ms * 1000);
}

static struct termios initial_settings, new_settings;

static int peek_character = -1;


void init_keyboard()
{
    tcgetattr(0,&initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
}

void close_keyboard()
{
    tcsetattr(0, TCSANOW, &initial_settings);
}

int _kbhit()
{
    unsigned char ch;
    int nread;

    if (peek_character != -1) return 1;
    new_settings.c_cc[VMIN]=0;
    tcsetattr(0, TCSANOW, &new_settings);
    nread = read(0,&ch,1);
    new_settings.c_cc[VMIN]=1;
    tcsetattr(0, TCSANOW, &new_settings);

    if(nread == 1)
    {
        peek_character = ch;
        return 1;
    }

    return 0;
}


int getch_()
{
    char ch;

    if(peek_character != -1)
    {
        ch = peek_character;
        peek_character = -1;
        return ch;
    }
    read(0,&ch,1);
    return ch;
}

// for arrow input
int _getch()
{
    int a = getch_();
    
    if(a == 27)
    {
        int b = getch_();
        
        if(b == 91)
        {
            int c = getch_();
            
            // 이거 원래 224 + 72, 224 + 75 이런식인데,,
            // 학교에서 하는 코드로는 이거만해도 충분히 돌아갈 듯
            
            if(c == 'A')
                return 72;
            if(c == 'D')
                return 75;
            if(c == 'C')
                return 77;
            if(c == 'B')
                return 80;
        }
        
        return b;
    }
    
    return a;
}

#endif /* Windows_h */
