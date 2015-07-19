//
//  MenuControls.cpp
//  Underneath
//
//  Created by Braeden Atlee on 4/14/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "MenuControls.hpp"
#include "MenuMessage.hpp"
#include "MenuYesNo.hpp"
#include "Controls.hpp"
#include "Settings.hpp"

namespace Ui {

    MenuControls::MenuControls() : Menu() {

    }


    void MenuControls::handleInput(int in){
        if(in == KEY_ESCAPE){
            Setting::saveSettings(UnderneathDir + "settings.txt");
            closeThisMenu();
            return;
        }else if(in == Key::uiUp){
            selected--;
            if (selected < 0) {
                selected = (int)keybindings.size()-1;
            }
        }else if(in == Key::uiDown){
            selected++;
            if (selected >= keybindings.size()) {
                selected = 0;
            }

        }else if(in == '\n'){
            move(0, 0);
            clrtobot();
            printCenter(terminalSize.y/2 - 2, "Press any key to asign (case sensitive)");
            printCenter(terminalSize.y/2 - 1, "\"%s\"", keybindings[selected].name.c_str());
            printCenter(terminalSize.y/2 + 1, "Currently asigned as [%s]", keyDisplayName(*(keybindings[selected].key)).c_str());
            int newKey = getch();
            bool set = true;
            for(int r : reservedKeys){
                if(r == newKey){
                    set = false;
                }
            }
            if(set){
                *(keybindings[selected].key) = newKey;
            }else{
                openMenu(new MenuMessage("Sorry, that key is reserved."));
            }
        }
    }

    void MenuControls::update() {
        move(0, 0);
        clrtobot();
        setColor(C_WHITE);

        mvprintw(0, 0, "Controls");

        int y = 2;

        for(int i=0;i<keybindings.size();i++){
            if(i == selected){
                setColor(C_BLACK, C_WHITE);
            }
            bool conflict = false;
            for(KeyBind b : keybindings){
                if((b.catagory & keybindings[i].catagory) && b.key != keybindings[i].key && *b.key == *keybindings[i].key){
                    conflict = true;
                }
            }
            if(conflict){
                setColor(C_WHITE, C_LIGHT_RED);
            }
            mvhline(y, 0, ' ', terminalSize.x);

            if(i == selected){
                mvaddch(y, 0, '-');
            }

            mvprintw(y, 2, "[ %s ]", keyDisplayName(*keybindings[i].key).c_str());
            mvprintw(y, 22, keybindings[i].name.c_str());
            mvprintw(y++, terminalSize.x-8, "%s %s", (keybindings[i].catagory & keyCatUi)?"UI":"  ", (keybindings[i].catagory & keyCatEntityPlayer)?"GAME":"    ");

            setColor(C_WHITE, C_BLACK);
        }
        
    }

    string MenuControls::keyDisplayName(int key){
        switch (key) {
            case KEY_UP:
                return "Arrow Up";
            case KEY_DOWN:
                return "Arrow Down";
            case KEY_LEFT:
                return "Arrow Left";
            case KEY_RIGHT:
                return "Arrow Right";
            case KEY_ESCAPE:
                return "Escape";
            case '\n':
                return "Enter";
            case ' ':
                return "Space";
            case KEY_BACKSPACE:
                return "Backspace";
                
            default:
                return string()+(char)key;
        }
    }
    
}
