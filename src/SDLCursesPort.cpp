//
//  SDLCursesPort.cpp
//  Underneath
//
//  Created by Braeden Atlee on 1/10/16.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//


#ifdef useSDLGraphics
#include "SDLCursesPort.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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

using namespace std;

namespace MainWindow{


    SDL_Window* mainWindow;
    SDL_Renderer* mainRenderer;
    //SDL_Surface* mainScreenSurface;
    //SDL_Surface* fontSurface;
    SDL_Texture* fontFgTexture;
    SDL_Texture* fontBgTexture;

    thread operationThread;

    int cursor = 0;

    int width = 100;
    int height = 30;

    vector<unsigned int> prevScreenCharBuffer;
    vector<unsigned char> prevScreenColorBuffer;
    vector<unsigned int> screenCharBuffer;
    vector<unsigned char> screenColorBuffer;

    int charWidth = 7;
    int charHeight = 12;

    int imageCharsPerLine = 16;
    int imageLineCount = 11;
    int imageCharWidth = 7;
    int imageCharHeight = 12;

    bool quit = false;

    int inputTimeout = -1;

    int backgroundColor = 0;

    int currentColor = 0;

    Uint32 colorWhite;
    Uint32 colorBlack;
    Uint32 colorClear;

    bool shiftIsDown = false;

    //From SDL docs
    Uint32 getpixel(SDL_Surface *surface, int x, int y) {
        if(x > surface->w || y > surface->h){
            return 0;
        }

        Uint32 *pixels = (Uint32 *)surface->pixels;

        return pixels[ ( y * surface->w ) + x ];

        /*int bpp = surface->format->BytesPerPixel;
        // Here p is the address to the pixel we want to retrieve
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
            return 0;       // shouldn't happen, but avoids warnings
        }*/
    }

    void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {
        if(x > surface->w || y > surface->h){
            return;
        }

        Uint32 *pixels = (Uint32 *)surface->pixels;

        pixels[ ( y * surface->w ) + x ] = pixel;

        /*int bpp = surface->format->BytesPerPixel;

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
        }*/
    }
    //


    int redFromCode(int colorCode){
        switch(colorCode){
            case 0x0: //Dark Black
            case 0x2: //Dark Green
            case 0x4: //Dark Blue
            case 0x6: //Dark Cyan
                return 0x00;
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
                return 0xFF;
        }
        return 0x00;
    }

    int greenFromCode(int colorCode){
        switch(colorCode){
            case 0x0: //Dark Black
            case 0x1: //Dark Red
            case 0x4: //Dark Blue
            case 0x5: //Dark Magenta
                return 0x00;
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
                return 0xFF;
        }
        return 0x00;
    }

    int blueFromCode(int colorCode){
        switch(colorCode){
            case 0x0: //Dark Black
            case 0x2: //Dark Green
            case 0x1: //Dark Red
            case 0x3: //Dark Yellow
                return 0x00;
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
                return 0xFF;
        }
        return 0x00;
    }


    bool initSDL(){
        printf("Init SDL\n");

        if(SDL_Init(SDL_INIT_VIDEO) < 0){
            printf("Failed to initalize SDL. Error: %s\n", SDL_GetError());
            return false;
        }
        mainWindow = SDL_CreateWindow("Underneath", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width*charWidth, height*charHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if(mainWindow == NULL){
            printf("Failed to create window. Error: %s\n", SDL_GetError());
            return false;
        }
        int imgFlags = IMG_INIT_PNG;
        if(!(IMG_Init(imgFlags) & imgFlags) ){
            printf("Failed to initalize SDL Image. Error: %s\n", IMG_GetError());
            return false;
        }

        mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
        if(!mainRenderer){
            printf("SDL_CreateRenderer: %s\n", SDL_GetError());
            return false;
        }

        colorWhite = 0xFFFFFFFF;
        colorBlack = 0xFF000000;
        colorClear = 0x00000000;

        makeBuffers();

        return initMedia();
    }

    void cleanupSDL(){
        SDL_DestroyWindow(mainWindow);
        //IMG_Quit();
        SDL_Quit();
    }

    /*void makeFontSurface(SDL_Surface* surf, int original, int color, int trans){
        SDL_LockSurface(surf);

        for(int x=0;x<width*charWidth;x++){
            for(int y=0;y<height*charHeight;y++){
                putpixel(surf, x, y, (getpixel(surf, x, y) == original)?color:trans);
            }
        }

        SDL_UnlockSurface(surf);

        SDL_SetColorKey(surf, SDL_TRUE, trans);
    }*/

    bool initMedia(){

        string img = "font.png";

        SDL_Surface* surfaceFg = IMG_Load(img.c_str());
        SDL_Surface* surfaceBg = IMG_Load(img.c_str());
    	if(!surfaceFg) {
    		printf("Failed to load image: \"%s\"\n", img.c_str());
            return false;
        }
        if(!surfaceBg) {
            printf("Failed to load image: \"%s\"\n", img.c_str());
            return false;
        }


        SDL_LockSurface(surfaceFg);
        for(int i=0;i<surfaceFg->w;i++){
            for(int j=0;j<surfaceFg->h;j++){
                //printf("Pixel before [%d, %d] = 0x%X\n", i, j, getpixel(surfaceFg, i, j));
                if(getpixel(surfaceFg, i, j) == colorBlack){
                    putpixel(surfaceFg, i, j, colorClear);
                }
                //printf("Pixel after [%d, %d] = 0x%X\n", i, j, getpixel(surfaceFg, i, j));
            }
        }
        SDL_UnlockSurface(surfaceFg);


        SDL_LockSurface(surfaceBg);
        for(int i=0;i<surfaceBg->w;i++){
            for(int j=0;j<surfaceBg->h;j++){
                if(getpixel(surfaceBg, i, j) == colorWhite){
                    putpixel(surfaceBg, i, j, colorClear);
                }
                if(getpixel(surfaceBg, i, j) == colorBlack){
                    putpixel(surfaceBg, i, j, colorWhite);
                }
            }
        }
        SDL_UnlockSurface(surfaceBg);

        fontFgTexture = SDL_CreateTextureFromSurface(mainRenderer, surfaceFg);
        fontBgTexture = SDL_CreateTextureFromSurface(mainRenderer, surfaceBg);
        if(!fontFgTexture){
            printf("Failed to convert surface to texture for image: \"%s\"\n", img.c_str());
            return false;
        }
        if(!fontBgTexture){
            printf("Failed to convert surface to texture for image: \"%s\"\n", img.c_str());
            return false;
        }

        imageCharWidth = surfaceFg->w / imageCharsPerLine;
        imageCharHeight = surfaceFg->h / imageLineCount;
        charWidth = imageCharWidth;
        charHeight = imageCharHeight;

        printf("Font Image Width = %d\n", surfaceFg->w);
        printf("Font Image Height = %d\n", surfaceFg->h);

        printf("Char Width (%d/%d) = %d\n", surfaceFg->w, imageCharsPerLine, imageCharWidth);
        printf("Char Height (%d/%d) = %d\n", surfaceFg->h, imageLineCount, imageCharHeight);


        SDL_FreeSurface(surfaceFg);
        SDL_FreeSurface(surfaceBg);

    	return true;
    }

    int srcX(int cc){
        switch (cc) {
            case 0xC2A2://¢
                return 0*imageCharWidth;
                
            case 0xE2A796://⧖
                return 1*imageCharWidth;
                
            default:
                return (((int)cc-32)%imageCharsPerLine)*imageCharWidth;
        }
    }

    int srcY(int cc){
        switch (cc) {
            case 0xC2A2://¢
            case 0xE2A796://⧖
                return 6*imageCharHeight;
                
            default:
                return (((int)cc-32)/imageCharsPerLine)*imageCharHeight;
        }
    }

    void update(){

        int wh = (width*height);

        bool changed = false;

        for(int c=0;c<wh;c++){
            if(prevScreenCharBuffer[c] != screenCharBuffer[c] || prevScreenColorBuffer[c] != screenColorBuffer[c]){
                changed = true;
                break;
            }
        }

        if(changed){
            copy(screenCharBuffer.begin(), screenCharBuffer.end(), prevScreenCharBuffer.begin());
            copy(screenColorBuffer.begin(), screenColorBuffer.end(), prevScreenColorBuffer.begin());
        }else{
            return;
        }

        //printf("Redraw ");

        SDL_RenderClear(mainRenderer);

        SDL_Rect src;
        SDL_Rect dst;

        src.w = imageCharWidth;
        src.h = imageCharHeight;

        dst.w = charWidth;
        dst.h = charHeight;

        int c, fgColorCode, bgColorCode;

        for(int i=0;i<width;i++){
            for(int j=0;j<height;j++){

                c = ((j*width)+i);

                src.x = srcX(screenCharBuffer[c]);
                src.y = srcY(screenCharBuffer[c]);

                dst.x = (c%width)*charWidth;
                dst.y = (c/width)*charHeight;

                fgColorCode = ((screenColorBuffer[c])) & 0xF;
                bgColorCode = (((screenColorBuffer[c])) >> 4) & 0xF;

                if(fgColorCode == 0 && bgColorCode == 0){
                    fgColorCode = 0xF;
                }

                if(bgColorCode > 0){

                    SDL_SetTextureColorMod(fontBgTexture, redFromCode(bgColorCode), greenFromCode(bgColorCode), blueFromCode(bgColorCode));

                    SDL_RenderCopy(mainRenderer, fontBgTexture, &src, &dst);

                }

                if(fgColorCode > 0){

                    SDL_SetTextureColorMod(fontFgTexture, redFromCode(fgColorCode), greenFromCode(fgColorCode), blueFromCode(fgColorCode));

                    SDL_RenderCopy(mainRenderer, fontFgTexture, &src, &dst);
                }



                /*SDL_BlitSurface(fontSurface, &src, mainScreenSurface, &dst);

                //SDL_BlitSurface(fontFgSurface[fgColorCode], &src, mainScreenSurface, &dst);
                //SDL_BlitSurface(fontBgSurface[bgColorCode], &src, mainScreenSurface, &dst);


                SDL_LockSurface(mainScreenSurface);

                //if((fgColorCode != 0x10 && fgColorCode != 0x0) || (bgColorCode != 0x10 && bgColorCode != 0x0)){
                    //printf("Color Codes: 0x%X, 0x%X\n", fgColorCode, bgColorCode);
                //}

                if(fgColorCode == 0 && bgColorCode == 0){
                    fgColorCode = 0xF;
                }

                colorfg = SDL_MapRGB(mainScreenSurface->format, redFromCode(fgColorCode), greenFromCode(fgColorCode), blueFromCode(fgColorCode));

                colorbg = SDL_MapRGB(mainScreenSurface->format, redFromCode(bgColorCode), greenFromCode(bgColorCode), blueFromCode(bgColorCode));

                //printf("0,0 pixel: 0x%X\n", getpixel(mainScreenSurface, dst.x, dst.y));

                for(int x=0;x<charWidth;x++){
                    for(int y=0;y<charHeight;y++){
                        putpixel(mainScreenSurface, dst.x+x, dst.y+y, (getpixel(mainScreenSurface, dst.x+x, dst.y+y) == rgbColorWhite)?colorfg:colorbg);
                    }
                }


                SDL_UnlockSurface(mainScreenSurface);*/

            }
        }

        SDL_RenderPresent(mainRenderer);

		//SDL_UpdateWindowSurface(mainWindow);
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

    void makeBuffers(){
        screenCharBuffer = vector<unsigned int>(width * height);
        screenColorBuffer = vector<unsigned char>(width * height);
        prevScreenCharBuffer = vector<unsigned int>(width * height);
        prevScreenColorBuffer = vector<unsigned char>(width * height);
    }

    int getCodeFromEvent(SDL_Event e){
        switch(e.type){
            case SDL_QUIT:{
                return KEY_EXIT;
            }
            case SDL_WINDOWEVENT:{
                if(e.window.event == SDL_WINDOWEVENT_RESIZED){
                    width = (int)ceil(e.window.data1 / (double)charWidth);
                    height = (int)ceil(e.window.data2 / (double)charHeight);
                    SDL_RenderSetLogicalSize(mainRenderer, width*charWidth, height*charHeight);
                    //SDL_SetWindowSize(mainWindow, width*charWidth, height*charHeight);
                    //mainScreenSurface = SDL_GetWindowSurface(mainWindow);
                    makeBuffers();
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
                                }else{
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
                            }else{
                                if(e.key.keysym.sym >= 'A' && e.key.keysym.sym <= 'Z'){
                                    return e.key.keysym.sym -= ('A'-'a');
                                }else{
                                    return e.key.keysym.sym;
                                }
                            }
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
                        shiftIsDown = false;
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
        return ERR;
    }

    int getCode(){
        int tick = SDL_GetTicks();
        SDL_Event e;
        while(SDL_WaitEventTimeout(&e, inputTimeout)==0){
            int lastTick = SDL_GetTicks();
            if(lastTick - tick > inputTimeout){
                return ERR;
            }
        }

        int result = ERR;

        do{
            result = getCodeFromEvent(e);
        }while(result == ERR && SDL_PollEvent(&e) != 0);

        return result;
    }
}

using namespace MainWindow;


bool initscr(){
    return initSDL();
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
    //update();
    int c = MainWindow::getCode();
    //if(c!=-1)printf("Key code: 0x%X\n", c);
    return c;
}

int	move(int y, int x){
    cursor = (y*width)+x;
    return 0;
}

int addch(const char c){
    if(cursor >= screenCharBuffer.size()){
        return 0;
    }
    if(cursor > 0 && screenCharBuffer[cursor-1] > 127 && c < 0){
        screenCharBuffer[cursor-1] = (screenCharBuffer[cursor-1]<<8)+(unsigned char)c;
    }else{
    	screenCharBuffer[cursor] = (unsigned char)c;
    	screenColorBuffer[cursor] = currentColor;
    	cursor++;
    }
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
    int j = 0;
    for(int i=cursor%width && j<=l;i<width;i++){
        screenCharBuffer[cursor] = c;
        screenColorBuffer[cursor] = currentColor;
        cursor++;
        j++;
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
        screenColorBuffer[cursor] = currentColor;
        cursor+=width;
    }
    return 0;
}

int	mvvline(int y, int x, const char c, int l){
    move(y, x);
    return vline(c, l);
}

int clrtoeol(){
    for(int i=cursor%width;i<width;i++){
        screenCharBuffer[cursor] = ' ';
        screenColorBuffer[cursor] = 0;
        cursor++;
    }

    return 0;
}

int clrtobot(){
    for(;cursor<width*height;cursor++){
        screenCharBuffer[cursor] = ' ';
        screenColorBuffer[cursor] = 0;
    }
    return 0;
}

int refresh(){
    update();
    return 0;
}

int endwin(){
    cleanupSDL();
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
    currentColor = attr >> 17;
    return 0;
}

#endif
