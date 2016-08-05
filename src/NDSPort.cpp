#ifdef __NDS__

#include "NDSPort.hpp"


#include <stdio.h>
#include <stdarg.h>

#include "font.h"


namespace NDS {

	int inputTimeout = -1;
	int backgroundColor = 0;
    int currentColor = 0;
	
    bool shiftIsDown = false;
	
	PrintConsole* pcTop;
	//PrintConsole pcBottom;

	int bgTop;
	//int bgBottom;

	int scaleX, scaleY;
	
	touchPosition touchXY;
	
	uint32 keystate;
	
	volatile int frame = 0;

	void Vblank() {
		frame++;
	}

	bool initNDS() {
		touchPosition touchXY;

		irqSet(IRQ_VBLANK, Vblank);
		
		videoSetMode(MODE_5_2D);
		//videoSetModeSub(MODE_5_2D);
		
		vramSetBankA(VRAM_A_MAIN_BG);
		//vramSetBankC(VRAM_C_SUB_BG);

		pcTop = consoleInit(0, 3, BgType_ExRotation, BgSize_ER_256x256, 20, 0, true, false);
		//consoleInit(&pcBottom, 3, BgType_ExRotation, BgSize_ER_1024x1024, 20, 0, false, true);

		ConsoleFont font;

		font.gfx = (u16*)fontTiles;
		font.pal = (u16*)fontPal;
		font.numChars = 95;
		font.numColors =  fontPalLen / 2;
		font.bpp = 8;
		font.asciiOffset = 32;
		font.convertSingleColor = false;
		
		consoleSetFont(pcTop, &font);
		//consoleSetFont(&pcBottom, &font);

		bgTop = pcTop->bgId;
		//bgBottom = pcBottom.bgId;

		scaleX = intToFixed(1,8);
		scaleY = intToFixed(1,8);

		return true;
	}

	bool cleanupNDS() {
		return true;
	}
	
	void update(){
		swiWaitForVBlank();
		scanKeys();
		//consoleSelect(&pcBottom);
		//move(0, 0);
		//printf("Keyboard here\n");
		//consoleSelect(&pcTop);

		//scaleX += 1;
		//scaleY += 1;

		bgSetRotateScale(bgTop, 0, scaleX, scaleY);
		bgSetScroll(bgTop, 0, 0);
		bgUpdate();
	}
	
	int getCode(){
		
		keystate = keysDown();
		
		bool shift = (keystate & KEYPAD_BITS::KEY_L) || (keystate & KEYPAD_BITS::KEY_R);
		
		if(keystate & KEYPAD_BITS::KEY_A){
			return shift?'A':'a';
		}
		if(keystate & KEYPAD_BITS::KEY_B){
			return shift?'B':'b';
		}
		if(keystate & KEYPAD_BITS::KEY_X){
			return shift?'X':'x';
		}
		if(keystate & KEYPAD_BITS::KEY_Y){
			return shift?'Y':'y';
		}
		if(keystate & BIT(6)){
			return KEY_UP;
		}
		if(keystate & BIT(7)){
			return KEY_DOWN;
		}
		if(keystate & BIT(5)){
			return KEY_LEFT;
		}
		if(keystate & BIT(4)){
			return KEY_RIGHT;
		}
		if(keystate & KEYPAD_BITS::KEY_SELECT){
			return '\n';
		}
		if(keystate & KEYPAD_BITS::KEY_START){
			return KEY_ESCAPE;
		}
		if(keystate & KEYPAD_BITS::KEY_LID){
			return -1;
		}
		if(keystate & KEYPAD_BITS::KEY_TOUCH){
			touchRead(&touchXY);
			return -1; // Make on screen keyboard for touch
		}
		
		return -1;

		//iprintf("\x1b[10;0HFrame = %d",frame);
		//iprintf("\x1b[16;0HTouch x = %04X, %04X\n", touchXY.rawx, touchXY.px);
		//iprintf("Touch y = %04X, %04X\n", touchXY.rawy, touchXY.py);
	}
	
	
}	

using namespace NDS;

int min(int a, int b){
	return a < b ? a : b;
}

bool initscr(){
    return initNDS();
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
    inputTimeout = timeout;
}

int getch(){
    //update();
    int c = getCode();
    //if(c!=-1)printf("Key code: 0x%X\n", c);
    return c;
}

int	move(int y, int x){
    pcTop->cursorX = x;
	pcTop->cursorY = y;
    return 0;
}

int addch(const char c){
    /*if(cursor >= screenCharBuffer.size()){
        return 0;
    }
    if(cursor > 0 && screenCharBuffer[cursor-1] > 127 && c < 0){
        screenCharBuffer[cursor-1] = (screenCharBuffer[cursor-1]<<8)+(unsigned char)c;
    }else{
    	screenCharBuffer[cursor] = (unsigned char)c;
    	screenColorBuffer[cursor] = currentColor;
    	cursor++;
    }*/
	printf("%c", c);
    return 0;
}

int mvaddch(int y, int x, const char c){
    move(y, x);
    return addch(c);
}

int addstr(const char * s){
	printf("%s", s);
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
    for(int j=0;j<min(l, pcTop->consoleHeight-pcTop->cursorY);j++){
		move(pcTop->cursorY+j, pcTop->cursorX);
		addch(c);
	}
    return 0;
}

int	mvhline(int y, int x, const char c, int l){
    move(y, x);
    return hline(c, l);
}

int	vline(const char c, int l){
    for(int i=0;i<min(l, pcTop->consoleWidth-pcTop->cursorX);i++){
		addch(c);
	}
    return 0;
}

int	mvvline(int y, int x, const char c, int l){
    move(y, x);
    return vline(c, l);
}

int clrtoeol(){
    for(int i=pcTop->cursorX;i<pcTop->consoleWidth;i++){
		addch(' ');
	}
    return 0;
}

int clrtobot(){
	for(int j=pcTop->cursorY;j<pcTop->consoleHeight;j++){
		for(int i=pcTop->cursorX;i<pcTop->consoleWidth;i++){
			addch(' ');
		}
	}
    return 0;
}

int refresh(){
    update();
    return 0;
}

int endwin(){
    cleanupNDS();
    return 0;
}

int getcurx(int scr){
    return pcTop->cursorX;
}

int getcury(int scr){
    return pcTop->cursorY;
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
    return pcTop->consoleWidth;
}

int getmaxy(int scr){
    return pcTop->consoleHeight;
}

int attrset(int attr){
    currentColor = attr >> 17;
    return 0;
}

#endif