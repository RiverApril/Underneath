
#include "LC_SDLLightcurses.hpp"

namespace MainWindow{


    SDL_Window* mainWindow;
    SDL_Surface* mainScreenSurface;
    SDL_Surface* fontSurface;

    std::thread operationThread;

    int cursor = 0;

    int width = 80;
    int height = 60;

    std::vector<char> screenCharBuffer = std::vector<char>(width * height);
    std::vector<int> screenAttrBuffer = std::vector<int>(width * height);

    int charHeight = 8;
    int charWidth = 8;

    bool quit = false;

    int inputTimeout = -1;

    int backgroundColor = 0;

    int currentAttr = 0;

    int rgbColorWhite;
    int rgbColorBlack;

    //From SDL docs
    Uint32 getpixel(SDL_Surface *surface, int x, int y) {
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
                return 0;
            case 0x1: //Dark Red
            case 0x3: //Dark Yellow
            case 0x5: //Dark Magenta
                return 0x80;

            case 0x7: //Dark White
                return 0xC0;

            case 0x8: //Light Black
                return 0x40;

            case 0xA: //Light Green
            case 0xC: //Light Blue
            case 0xE: //Light Cyan
                return 0;
            case 0x9: //Light Red
            case 0xB: //Light Yellow
            case 0xD: //Light Magenta
            case 0xF: //Light White
                return 0xFF;
        }
    }

    int greenFromCode(int colorCode){
        switch(colorCode){
            case 0x0: //Dark Black
            case 0x1: //Dark Red
            case 0x4: //Dark Blue
            case 0x5: //Dark Magenta
                return 0;
            case 0x2: //Dark Green
            case 0x3: //Dark Yellow
            case 0x6: //Dark Cyan
                return 0x80;

            case 0x7: //Dark White
                return 0xC0;

            case 0x8: //Light Black
                return 0x40;

            case 0x9: //Light Red
            case 0xC: //Light Blue
            case 0xD: //Light Magenta
                return 0;
            case 0xA: //Light Green
            case 0xB: //Light Yellow
            case 0xE: //Light Cyan
            case 0xF: //Light White
                return 0xFF;
        }
    }

    int blueFromCode(int colorCode){
        switch(colorCode){
            case 0x0: //Dark Black
            case 0x2: //Dark Green
            case 0x1: //Dark Red
            case 0x3: //Dark Yellow
                return 0;
            case 0x4: //Dark Blue
            case 0x6: //Dark Cyan
            case 0x5: //Dark Magenta
                return 0x80;

            case 0x7: //Dark White
                return 0xC0;

            case 0x8: //Light Black
                return 0x40;

            case 0xA: //Light Green
            case 0x9: //Light Red
            case 0xB: //Light Yellow
                return 0;
            case 0xC: //Light Blue
            case 0xE: //Light Cyan
            case 0xD: //Light Magenta
            case 0xF: //Light White
                return 0xFF;
        }
    }


    bool initSDL(){
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

                c = ((i*width)+j);

                src.x = (((int)screenCharBuffer[c])%16)*8;
                src.y = (((int)screenCharBuffer[c])/16)*8;

                dst.x = (c%width)*charWidth;
                dst.y = (c/width)*charHeight;

                SDL_BlitSurface(fontSurface, &src, mainScreenSurface, &dst);

                SDL_LockSurface(mainScreenSurface);


                fgColorCode = ((screenAttrBuffer[c] & A_COLOR) >> 17) & 0x10;
                bgColorCode = (((screenAttrBuffer[c] & A_COLOR) >> 17) >> 4) & 0x10;

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
            SDL_Delay(1);
        }

        switch(e.type){
            case SDL_QUIT:{
                return KEY_ESCAPE;
            }
            case SDL_WINDOWEVENT:{
                if(e.window.event == SDL_WINDOWEVENT_RESIZED){
                    width = (int)ceil(e.window.data1 / (double)charWidth);
                    height = (int)ceil(e.window.data2 / (double)charHeight);
                    screenCharBuffer = std::vector<char>(width * height);
                    screenAttrBuffer = std::vector<int>(width * height);
                    return KEY_RESIZE;
                }
                break;
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

                    default:
                        return e.key.keysym.sym;
                }
                break;
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
    return MainWindow::getCode();
}

int	move(int y, int x){
    cursor = (y*width)+x;
    return 0;
}

int addch(const char c){
    cursor++;
    screenCharBuffer[cursor] = c;
    screenAttrBuffer[cursor] = currentAttr;
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
    for(int i=0;cursor%width>0 && i<l;cursor++){
        screenCharBuffer[cursor] = c;
        screenAttrBuffer[cursor] = currentAttr;
        i++;
    }
    return 0;
}

int	mvhline(int y, int x, const char c, int l){
    move(y, x);
    return hline(c, l);
}

int	vline(const char c, int l){
    for(int i=0;cursor>0 && i<l;cursor+=width){
        screenCharBuffer[cursor] = c;
        screenAttrBuffer[cursor] = currentAttr;
        i++;
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
}

int init_pair(int i, int a, int b){
    //Dummy
}

int getmaxx(int scr){
    return width;
}

int getmaxy(int scr){
    return height;
}

int attrset(int attr){
    currentAttr = attr;
}
