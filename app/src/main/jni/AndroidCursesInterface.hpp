//
// Created by Braeden Atlee on 11/9/15.
//

#ifndef UNDERNEATH_ANDROIDCURSESINTERFACE_HPP
#define UNDERNEATH_ANDROIDCURSESINTERFACE_HPP


#define stdscr 1

#define KEY_ESCAPE 27
#define KEY_RESIZE 0x200
#define KEY_DOWN 0x102
#define KEY_UP 0x103
#define KEY_LEFT 0x104
#define KEY_RIGHT 0x105
#define KEY_BACKSPACE 0x107
#define KEY_ENTER 0x157
#define KEY_EXIT 0x169
#define KEY_F(n)((n)+0410)
#define KEY_DC 0512
#define ERR (-1)

#define A_BLINK 0x00000800
#define A_COLOR 0x03fe0000

#define COLORS 0x1000000

#define COLOR_PAIR(n) ((((int)n) << 17) & A_COLOR)

#include <jni.h>

bool initscr();
void start_color();
void use_default_colors();
void keypad(int, bool);
void noecho();
void cbreak();
void curs_set(int);
void timeout(int);
int getch();
int refresh();
int	move(int, int);
int	addch(const char);
int	mvaddch(int, int, const char);
int	addstr(const char *);
int	printw(const char *, ...);
int	mvaddstr(int, int, const char *);
int	mvprintw(int, int, const char *, ...);
int	hline(const char, int);
int	mvhline(int, int, const char, int);
int	vline(const char, int);
int	mvvline(int, int, const char, int);
int clrtoeol();
int clrtobot();
int endwin();
int getcurx(int);
int getcury(int);
int bkgd(int);
int init_pair(int, int, int);
int getmaxx(int);
int getmaxy(int);
int attrset(int);

#endif //UNDERNEATH_ANDROIDCURSESINTERFACE_HPP
