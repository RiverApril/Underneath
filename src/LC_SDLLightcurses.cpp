
#ifdef useSDLLightCurses
#include "LC_SDLLightcurses.hpp"

#include <iostream>
#include <sstream>
#include <ostream>
#include <vector>
#include <queue>
#include <map>
#include <functional>
#include <algorithm>
#include <string>

#ifdef __MINGW32__
    #include "thread.h"
#else
    #include <thread>
#endif

#include <time.h>
#include <locale.h>
#include <stdlib.h>
#include <cmath>
#include <fstream>
#include <assert.h>

#include <stdio.h>

namespace MainWindow{


    SDL_Window* mainWindow;
    SDL_Surface* mainScreenSurface;
    SDL_Surface* fontSurface;

    std::thread operationThread;

    int cursor = 0;

    int width = 100;
    int height = 30;

    std::vector<char> screenCharBuffer = std::vector<char>(width * height);
    std::vector<int> screenAttrBuffer = std::vector<int>(width * height);

    int charWidth = 7;
    int charHeight = 12;

    int imageCharsPerLine = 16;
    int imageLineCount = 16;
    int imageCharWidth = 7;
    int imageCharHeight = 12;

    bool quit = false;

    int inputTimeout = -1;

    int backgroundColor = 0;

    int currentAttr = 0;

    int rgbColorWhite;
    int rgbColorBlack;

    bool shiftIsDown = false;

    //From SDL docs
    Uint32 getpixel(SDL_Surface *surface, int x, int y) {
        if(x > surface->w || y > surface->h){
            return 0;
        }
        int bpp = surface->format->BytesPerPixel;
        /* Here p is the address to the pixel we want to retrieve */
        Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

        switch(bpp) {
        case 1:
            return *p;
            break;

        case 2:
            return *(Uint16 *)p;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
            break;

        case 4:
            return *(Uint32 *)p;
            break;

        default:
            return 0;       /* shouldn't happen, but avoids warnings */
        }
    }

    void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {
        if(x > surface->w || y > surface->h){
            return;
        }
        int bpp = surface->format->BytesPerPixel;
        /* Here p is the address to the pixel we want to set */
        Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

        switch(bpp) {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
        }
    }
    //


    int redFromCode(int colorCode){
        switch(colorCode){
            case 0x0: //Dark Black
            case 0x2: //Dark Green
            case 0x4: //Dark Blue
            case 0x6: //Dark Cyan
                return 0x01;
            case 0x1: //Dark Red
            case 0x3: //Dark Yellow
            case 0x5: //Dark Magenta
                return 0xBB;

            case 0x7: //Dark White
                return 0xBB;

            case 0x8: //Light Black
                return 0x55;

            case 0xA: //Light Green
            case 0xC: //Light Blue
            case 0xE: //Light Cyan
                return 0x55;
            case 0x9: //Light Red
            case 0xB: //Light Yellow
            case 0xD: //Light Magenta
            case 0xF: //Light White
                return 0xFE;
        }
        return 1;
    }

    int greenFromCode(int colorCode){
        switch(colorCode){
            case 0x0: //Dark Black
            case 0x1: //Dark Red
            case 0x4: //Dark Blue
            case 0x5: //Dark Magenta
                return 0x01;
            case 0x2: //Dark Green
            case 0x3: //Dark Yellow
            case 0x6: //Dark Cyan
                return 0xBB;

            case 0x7: //Dark White
                return 0xBB;

            case 0x8: //Light Black
                return 0x55;

            case 0x9: //Light Red
            case 0xC: //Light Blue
            case 0xD: //Light Magenta
                return 0x55;
            case 0xA: //Light Green
            case 0xB: //Light Yellow
            case 0xE: //Light Cyan
            case 0xF: //Light White
                return 0xFE;
        }
        return 1;
    }

    int blueFromCode(int colorCode){
        switch(colorCode){
            case 0x0: //Dark Black
            case 0x2: //Dark Green
            case 0x1: //Dark Red
            case 0x3: //Dark Yellow
                return 0x01;
            case 0x4: //Dark Blue
            case 0x6: //Dark Cyan
            case 0x5: //Dark Magenta
                return 0xBB;

            case 0x7: //Dark White
                return 0xBB;

            case 0x8: //Light Black
                return 0x55;

            case 0xA: //Light Green
            case 0x9: //Light Red
            case 0xB: //Light Yellow
                return 0x55;
            case 0xC: //Light Blue
            case 0xE: //Light Cyan
            case 0xD: //Light Magenta
            case 0xF: //Light White
                return 0xFE;
        }
        return 1;
    }


    bool initSDL(){
        printf("Init SDL\n");
        bool success = true;

        if(SDL_Init(SDL_INIT_VIDEO) < 0){
            printf("Failed to initalize SDL. Error: %s\n", SDL_GetError());
            success = false;
        }else{
            mainWindow = SDL_CreateWindow("Underneath", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width*charWidth, height*charHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
            if(mainWindow == NULL){
                printf("Failed to create window. Error: %s\n", SDL_GetError());
                success = false;
            }else{
                int imgFlags = IMG_INIT_PNG;
                if(!(IMG_Init(imgFlags) & imgFlags) ){
                    printf("Failed to initalize SDL Image. Error: %s\n", IMG_GetError());
                    success = false;
                }else{
                    mainScreenSurface = SDL_GetWindowSurface(mainWindow);
                    rgbColorWhite = SDL_MapRGB(mainScreenSurface->format, 0xFF, 0xFF, 0xFF);
                    rgbColorBlack = SDL_MapRGB(mainScreenSurface->format, 0, 0, 0);
                }
            }
        }

        return success;
    }

    bool initMedia(){
    	bool success = true;

    	fontSurface = IMG_Load("font.png");
    	if(fontSurface == NULL) {
    		printf("Failed to load image\n");
    		success = false;
    	}

    	return success;
    }

    void update(){

        SDL_Rect src;
        SDL_Rect dst;

        src.w = charWidth;
        src.h = charHeight;

        dst.w = charWidth;
        dst.h = charHeight;

        int c, color, fgColorCode, bgColorCode;

        for(int i=0;i<width;i++){
            for(int j=0;j<height;j++){

                c = ((j*width)+i);

                src.x = (((int)screenCharBuffer[c]-32)%imageCharsPerLine)*imageCharWidth;
                src.y = (((int)screenCharBuffer[c]-32)/imageLineCount)*imageCharHeight;

                dst.x = (c%width)*charWidth;
                dst.y = (c/width)*charHeight;

                SDL_BlitSurface(fontSurface, &src, mainScreenSurface, &dst);

                
                SDL_LockSurface(mainScreenSurface);

                fgColorCode = ((screenAttrBuffer[c] & A_COLOR) >> 17) & 0xF;
                bgColorCode = (((screenAttrBuffer[c] & A_COLOR) >> 17) >> 4) & 0xF;

                //if((fgColorCode != 0x10 && fgColorCode != 0x0) || (bgColorCode != 0x10 && bgColorCode != 0x0)){
                    //printf("Color Codes: 0x%X, 0x%X\n", fgColorCode, bgColorCode);
                //}

                if(fgColorCode == 0 && bgColorCode == 0){
                    fgColorCode = 0xF;
                }

                color = SDL_MapRGB(mainScreenSurface->format, redFromCode(fgColorCode), greenFromCode(fgColorCode), blueFromCode(fgColorCode));

                for(int x=0;x<charWidth;x++){
                    for(int y=0;y<charHeight;y++){
                        if(getpixel(mainScreenSurface, dst.x+x, dst.y+y) == rgbColorWhite) {
		                    putpixel(mainScreenSurface, dst.x+x, dst.y+y, color);
                        }
                    }
                }

                color = SDL_MapRGB(mainScreenSurface->format, redFromCode(bgColorCode), greenFromCode(bgColorCode), blueFromCode(bgColorCode));

                for(int x=0;x<charWidth;x++){
                    for(int y=0;y<charHeight;y++){
                        if(getpixel(mainScreenSurface, dst.x+x, dst.y+y) == rgbColorBlack) {
		                    putpixel(mainScreenSurface, dst.x+x, dst.y+y, color);
                        }
                    }
                }


                SDL_UnlockSurface(mainScreenSurface);

            }
        }


		SDL_UpdateWindowSurface(mainWindow);
    }

    /*SDL_Surface* loadImage(const char* path){
        SDL_Surface* optimizedSurface = NULL;

        SDL_Surface* loadedSurface = IMG_Load(path);
        if(loadedSurface == NULL){
            printf("Unable to load image %s. Error: %s\n", path, IMG_GetError());
        }else{
            optimizedSurface = SDL_ConvertSurface(loadedSurface, mainScreenSurface->format, 0);
            if(optimizedSurface == NULL){
                printf("Unable to optimize image %s. Error: %s\n", path, SDL_GetError());
            }

            SDL_FreeSurface(loadedSurface);
        }

        return optimizedSurface;
    }*/

    int getCode(){
        int tick = SDL_GetTicks();
        SDL_Event e;
        while(SDL_PollEvent(&e)==0){
            int lastTick = SDL_GetTicks();
            if(lastTick - tick > inputTimeout){
                return ERR;
            }
            //SDL_Delay(1);
        }

        switch(e.type){
            case SDL_QUIT:{
                return KEY_EXIT;
            }
            case SDL_WINDOWEVENT:{
                if(e.window.event == SDL_WINDOWEVENT_RESIZED){
                    width = (int)ceil(e.window.data1 / (double)charWidth);
                    height = (int)ceil(e.window.data2 / (double)charHeight);
                    SDL_SetWindowSize(mainWindow, width*charWidth, height*charHeight);
                    mainScreenSurface = SDL_GetWindowSurface(mainWindow);
                    screenCharBuffer = std::vector<char>(width * height);
                    screenAttrBuffer = std::vector<int>(width * height);
                    return KEY_RESIZE;
                }
                return ERR;
            }
            case SDL_KEYDOWN:{
                switch(e.key.keysym.sym){
                    case SDLK_UP:
                        return KEY_UP;

                    case SDLK_DOWN:
                        return KEY_DOWN;

                    case SDLK_LEFT:
                        return KEY_LEFT;

                    case SDLK_RIGHT:
                        return KEY_RIGHT;

                    case SDLK_RETURN:
                        return '\n';

                    case SDLK_ESCAPE:
                        return KEY_ESCAPE;

                    case SDLK_BACKSPACE:
                        return KEY_BACKSPACE;

                    case SDLK_LSHIFT:
                    case SDLK_RSHIFT:
                        shiftIsDown = true;
                        return ERR;

                    default:
                        if(e.key.keysym.sym >= 32 && e.key.keysym.sym <= 127){
                            if(shiftIsDown){
                                if(e.key.keysym.sym >= 'a' && e.key.keysym.sym <= 'z'){
                                    return e.key.keysym.sym += ('A'-'a');
                                }
                                switch (e.key.keysym.sym) {
                                    case '`':
                                        return '~';
                                    case '1':
                                        return '!';
                                    case '2':
                                        return '@';
                                    case '3':
                                        return '#';
                                    case '4':
                                        return '$';
                                    case '5':
                                        return '%';
                                    case '6':
                                        return '^';
                                    case '7':
                                        return '&';
                                    case '8':
                                        return '*';
                                    case '9':
                                        return '(';
                                    case '0':
                                        return ')';
                                    case '-':
                                        return '_';
                                    case '=':
                                        return '+';
                                    case '[':
                                        return '{';
                                    case ']':
                                        return '}';
                                    case '\\':
                                        return '|';
                                    case ';':
                                        return ':';
                                    case '\'':
                                        return '"';
                                    case ',':
                                        return '<';
                                    case '.':
                                        return '>';
                                    case '/':
                                        return '?';
                                        
                                    default:
                                        break;
                                }
                            }
                            return e.key.keysym.sym;
                        }else{
                            return ERR;
                        }
                }
                break;
            }
            case SDL_KEYUP:{
                switch (e.key.keysym.sym) {

                    case SDLK_LSHIFT:
                    case SDLK_RSHIFT:
                        shiftIsDown = true;
                        return ERR;

                    default:
                        return ERR;
                }
                break;
            }
            default:{
                return ERR;
            }
        }
    }
}

using namespace MainWindow;


bool initscr(){
    return initSDL() && initMedia();
}

void start_color(){
    //Dummy
}

void use_default_colors(){
    //Dummy
}

void keypad(int scr, bool a){
    //Dummy
}

void noecho(){
    //Dummy
}

void cbreak(){
    //Dummy
}

void curs_set(int a){
    //Dummy
}

void timeout(int timeout){
    MainWindow::inputTimeout = timeout;
}

int getch(){
    update();
    int c = MainWindow::getCode();
    //if(c!=-1)printf("Key code: 0x%X\n", c);
    return c;
}

int	move(int y, int x){
    cursor = (y*width)+x;
    return 0;
}

int addch(const char c){
    screenCharBuffer[cursor] = c;
    screenAttrBuffer[cursor] = currentAttr;
    cursor++;
    return 0;
}

int mvaddch(int y, int x, const char c){
    move(y, x);
    return addch(c);
}

int addstr(const char * s){
    unsigned int i = 0;
    while(s[i] != '\0'){
        addch(s[i]);
        i++;
    }
    return 0;
}

int	printw(const char * s, ...){
    char buff[256];
    va_list args;
    va_start(args, s);
    vsprintf(buff, s, args);
    addstr(buff);
    va_end(args);
    return 0;
}

int	mvaddstr(int y, int x, const char * s){
    move(y, x);
    return addstr(s);
}


int	mvprintw(int y, int x, const char * s, ...){
    move(y, x);
    char buff[256];
    va_list args;
    va_start(args, s);
    vsprintf(buff, s, args);
    addstr(buff);
    va_end(args);
    return 0;
}

int	hline(const char c, int l){
    for(int i=0;cursor%width>0 && i<=l;i++){
        screenCharBuffer[cursor] = c;
        screenAttrBuffer[cursor] = currentAttr;
        cursor++;
    }
    return 0;
}

int	mvhline(int y, int x, const char c, int l){
    move(y, x);
    return hline(c, l);
}

int	vline(const char c, int l){
    for(int i=0;cursor<width*height && i<=l;i++){
        screenCharBuffer[cursor] = c;
        screenAttrBuffer[cursor] = currentAttr;
        cursor+=width;
    }
    return 0;
}

int	mvvline(int y, int x, const char c, int l){
    move(y, x);
    return vline(c, l);
}

int clrtoeol(){
    for(;cursor%width>0;cursor++){
        screenCharBuffer[cursor] = 0;
        screenAttrBuffer[cursor] = 0;
    }
    return 0;
}

int clrtobot(){
    for(;cursor<screenCharBuffer.size();cursor++){
        screenCharBuffer[cursor] = ' ';
        screenAttrBuffer[cursor] = 0;
    }
    return 0;
}

int refresh(){
    update();
    return 0;
}

int endwin(){
    //Dummy
    return 0;
}

int getcurx(int scr){
    return cursor % width;
}

int getcury(int scr){
    return cursor / width;
}

int bkgd(int c){
    backgroundColor = c;
    return 0;
}

int init_pair(int i, int a, int b){
    //Dummy
    return 0;
}

int getmaxx(int scr){
    return width;
}

int getmaxy(int scr){
    return height;
}

int attrset(int attr){
    currentAttr = attr;
    return 0;
}

#endif
