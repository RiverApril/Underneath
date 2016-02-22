//
//  MenuDialog.cpp
//  Underneath
//
//  Created by Braeden Atlee on 12/30/15.
//  Copyright © 2015 Braeden Atlee. All rights reserved.
//

#include "MenuDialog.hpp"
#include "Controls.hpp"

namespace Ui {

    MenuDialog::MenuDialog(vector<string> messageList, vector<string> dialogOptions, function<void(MenuDialog*, int)> afterClose, bool escapable, int defaultSelect) : Menu() {
        this->messageList = messageList;
        this->dialogOptions = dialogOptions;
        this->afterClose = afterClose;
        this->selected = defaultSelect;
        scrollTick = 0;
    }

    bool MenuDialog::openUi(){
        timeout(fastTimeout);
        return true;
    }

    void MenuDialog::closeUi(){
        timeout(defaultTimeout);
    }

    void MenuDialog::handleInput(int in) {
        if (in == Key::uiUp) {
            selected--;
        } else if (in == Key::uiDown) {
            selected++;
        } else if (in == Key::interact || in == '\n') {
            closeThisMenu();
            afterClose(this, selected);
        } else if (escapable && in == KEY_ESCAPE) {
            closeThisMenu();
            afterClose(this, -1);
        }
        
    }

    void MenuDialog::update() {
        if (selected < 0) {
            selected = (int) dialogOptions.size() - 1;
        }
        if (selected >= (int) dialogOptions.size()) {
            selected = 0;
        }

        setColor(C_WHITE);
        move(0, 0);
        clrtobot();
        int y = (int)(terminalSize.y - (messageList.size()+2+dialogOptions.size())) / 2;
        int t = 0;
        for(string line : messageList){
            if(t < scrollTick){
                printCenter(y++, line.substr(0, scrollTick-t));
                t += line.size();
            }
        }
        y+=2;
        int i = 0;
        for(string line : dialogOptions){
            if(t < scrollTick){
                printCenter(y++, formatString("%s %s %s", selected == i?"-":" ", line.substr(0, scrollTick-t).c_str(), selected == i?"-":" "));
                t += line.size();
            }
            i++;
        }

        scrollTick++;



    }
    
}
