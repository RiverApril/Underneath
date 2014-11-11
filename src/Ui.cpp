//
//  Ui.cpp
//  Underneath
//
//  Created by Braeden Atlee on 10/19/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Ui.h"
#include "MenuGame.h"
#include "Entity.h"
#include "AiEntity.h"
#include "Player.h"
#include "Math.h"

vector<string> consoleBuffer;

void debug(string s){
    if(Ui::printDebugEnabled){
    	print("DEBUG: "+s);
    }
}

void print(string s){
    if(s.find('\n') == string::npos){

        int lastI = 0;
        for(int i=0;i<s.length();i++){
            if(s[i] == '\n'){
                consoleBuffer.push_back(s.substr(lastI, i-lastI));
                lastI = i+1;
            }
        }
        consoleBuffer.push_back(s.substr(lastI));

    }else{
    	consoleBuffer.push_back(s);
    }
}

namespace Ui {

    const color COLOR_DEFAULT_ENTITY = C_LIGHT_RED;

    unsigned long tick = 0;

    /*double ms = 0;

    int fps = 0;
    long l = 0;
    long lastL = 0;
    long startTime = 0;
    int frames = 0;*/

    bool limitedColorMode = true;

    Point2 terminalSize;

    Menu* currentMenu;

    bool printDebugEnabled = true;

    bool consoleInputMode = false;
    string consoleInput = "";
    int consoleScroll = 0;

    vector<Command*> commandList;

    void changeMenu(Menu* newMenu) {
        if(currentMenu != nullptr){
        	currentMenu->_closeUi(newMenu);
        }
        newMenu->_openUi(currentMenu);
        currentMenu = newMenu;
    }

    void initNCurses() {

        setlocale(LC_ALL, "");

        initscr();
        start_color();
        use_default_colors();
        setTerminalSizeVar();
        keypad(stdscr, TRUE);
        noecho();
        curs_set(0);
        //timeout(-1);
        ESCDELAY = 1;


        limitedColorMode = COLORS<256;

        initColorPairs();

        refresh();

    }

    void initColorPairs() {

        int a = 0;

        for(int i = 0; i<0x10; i++) {
            for(int j = 0; j<0x10; j++) {
            	init_pair(a, j, i);
                a++;
            }
        }

    }

    void exitProgram() {
        running = false;
        delete currentMenu;
        endwin();
    }

    void setTerminalSizeVar(){
        terminalSize.x = getmaxx(stdscr);
        terminalSize.y = getmaxy(stdscr);
    }

    void setColor(color fg, color bg, int attr) {
        if(limitedColorMode){
            if(fg == 0x8){
                fg = 0x7;
            }else if(fg >= 0x8){
                fg -= 0x8;
            }
            if(bg == 0x8){
                bg = 0x7;
            }else if(bg >= 0x8){
                bg -= 0x8;
            }
        }
        attrset(COLOR_PAIR(fg + (bg*0x10)) | attr);
    }


    Menu::Menu(bool temp) {
        isTemp = temp;
    }

    void Menu::_openUi(Menu* oldMenu) {
        //timeout(inputTimeout);
        move(0, 0);
        clrtobot();
        refresh();
        openUi(oldMenu);
        update();
        refresh();
        if(isTemp){
            parentMenu = oldMenu;
        }
    }

    void Menu::_closeUi(Menu* newMenu){
        closeUi(newMenu);
        if(isTemp && newMenu == parentMenu){
            delete this;
        }
    }

    void Menu::_handleInput(int in) {
        
        switch (in) {
            case KEY_RESIZE:
                setTerminalSizeVar();
                move(0, 0);
                clrtobot();
                break;
        }
        if(consoleInputMode){
            switch (in) {
                case '`':
                    consoleInputMode = false;
                    break;

                case KEY_BACKSPACE:
                case 8: //Backspace
                case 127: //Delete
                    if(consoleInput.length() > 0){
                        consoleInput = consoleInput.substr(0, consoleInput.length()-1);
                    }
                    break;

                case KEY_ENTER:
                case 13:
                case '\n':
                    consoleInputMode = false;
                    execute(consoleInput, this);
                    consoleInput = "";
                    consoleScroll = 0;
                    break;

                case KEY_UP:
                    consoleScroll--;
                    break;

                case KEY_DOWN:
                    consoleScroll++;
                    break;

                default:
                    if((in>=32 && in<=126)){
                        consoleInput += in;
                    }
                    break;
            }
        }else{
            switch (in) {
                case '`':
                    consoleInputMode = true;
                    break;
                    
                default:
                    handleInput(in);
                    break;
            }
        }
    }

    void Menu::printConsole(int topY, int bottomY){
        setColor(C_WHITE);
        int j = 1;
        for(int i=bottomY-1;i>=topY;i--){
            move(i, 2);
            clrtoeol();
            move(i, 2);
            if(((int)(consoleBuffer.size())-j) < 0){
                break;
            }
            auto p = consoleBuffer.size()-j+consoleScroll;
            if(p < consoleBuffer.size()){
            	printw(consoleBuffer[p].c_str());
            }
            j++;
        }
    	mvprintw(topY, 0, (consoleScroll > -((int)consoleBuffer.size())+(bottomY-topY))?"^":" ");
		mvprintw(bottomY-1, 0, (consoleScroll < 0)?"v":" ");

        move(bottomY, 2);
        clrtoeol();
        if(consoleInputMode){
            Ui::setColor(C_DARK_GREEN);
            mvprintw(bottomY, 2, "> %s", consoleInput.c_str());
            Ui::setColor(C_LIGHT_GREEN, C_BLACK, A_BLINK);
            mvprintw(bottomY, 4+(int)consoleInput.length(), "_");
        }
    }

    void Menu::_update() {

        update();

        tick++;

    }

    void Menu::printCenter(int y, string s, ...){
        move(y, (terminalSize.x/2)-((int)(s.length()-1)/2));
        va_list args;
        va_start(args, s);
        vwprintw(stdscr, s.c_str(), args);
        va_end(args);
    }

    void Menu::printCenterOffset(int y, int xOff, string s, ...){
        move(y, (terminalSize.x/2)-((int)(s.length()-1)/2)+xOff);
        va_list args;
        va_start(args, s);
        vwprintw(stdscr, s.c_str(), args);
        va_end(args);
    }


    bool execute(string commandRaw, Menu* currentMenu){
        
        auto firstSpacePos = commandRaw.find_first_of(' ');
        string argumentsRaw = (firstSpacePos==string::npos)?"":commandRaw.substr(firstSpacePos+1);
        string commandName = commandRaw.substr(0, firstSpacePos);

        vector<string> arguments;
        string s;
        int lastI = 0;
        for(int i=0;i<argumentsRaw.length();i++){
            if(argumentsRaw[i] == ' '){
                s = argumentsRaw.substr(lastI, i-lastI);
                if(s.length() > 0){
                	arguments.push_back(s);
                }
                lastI = i+1;
            }
        }
        s = argumentsRaw.substr(lastI);
        if(s.length() > 0){
			arguments.push_back(s);
        }

        if(!currentMenu->menuOnlyExecute(commandName, arguments, argumentsRaw)){
            for(int i=0;i<commandList.size();i++){
                if(commandList[i]->acceptableName(commandName)){
                    commandList[i]->execute(commandName, arguments, argumentsRaw, currentMenu);
                    break;
                }
            }

        }

        return false;
    }

    struct CommandHelp : Command{
        string help(){
            return "Provides information about a command.";
        }
        string usage(){
            return "help [command]";
        }
        string defaultName(){
            return "help";
        }
        bool acceptableName(string name){
            return Command::acceptableName(name) || name.compare("?") == 0;
        }
        bool execute(string name, vector<string> arguments, string argumentsRaw, Menu* currentMenu){
            
            switch(arguments.size()){
                case 0:{
                    print("Global Commands:");
                    for(int i=0;i<commandList.size();i++){
                        print("- "+commandList[i]->usage());
                    }
                    string s = currentMenu->menuOnlyCommands();
                    if(s.length() > 0){
                        print("Menu Commands:");
                        print(s);
                    }
                    return true;
                }

                case 1:{
                    for(int i=0;i<commandList.size();i++){
                        if(commandList[i]->acceptableName(arguments[0])){
                            print(commandList[i]->usage() + "  " + commandList[i]->help());
                            return true;
                        }
                    }
                    string s = currentMenu->menuOnlyCommandHelp(arguments[0]);
                    if(s.length() > 0){
                        print(s);
                    }else{
                        print("Unknown Command: "+name);
                    }
                    return false;
                }

                default:
                    return false;
            }
        }
    };

    struct CommandEcho : Command{
        string help(){
            return "Echos what you input.";
        }
        string usage(){
            return "echo [words]";
        }
        string defaultName(){
            return "echo";
        }
        bool execute(string name, vector<string> arguments, string argumentsRaw, Menu* currentMenu){
            if(argumentsRaw.length() > 0){
                print(argumentsRaw);
                return true;
            }
            return false;
        }
    };

    struct CommandDebug : Command{
        string help(){
            return "Toggles debug messages.";
        }
        string usage(){
            return "debug";
        }
        string defaultName(){
            return "debug";
        }
        bool execute(string name, vector<string> arguments, string argumentsRaw, Menu* currentMenu){
            if(argumentsRaw.length() == 0){
                printDebugEnabled = !printDebugEnabled;
                print("Debug "+string(printDebugEnabled?"Enabled":"Disabled")+".");
                return true;
            }
            return false;
        }
    };

    void initCommandList(){
        commandList.push_back(new CommandHelp());
        commandList.push_back(new CommandEcho());
        commandList.push_back(new CommandDebug());
    }
}
