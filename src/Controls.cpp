//
//  Controls.cpp
//  Underneath
//
//  Created by Braeden Atlee on 12/27/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Controls.hpp"
#include "Ui.hpp"

namespace Key {

    //In Game
    int moveUp = KEY_UP;
    int moveDown = KEY_DOWN;
    int moveLeft = KEY_LEFT;
    int moveRight = KEY_RIGHT;
    int interact = 'g';
    int secondaryAttack = 'f';
    int inspect = 'q';
    int adjustConsole = '\\';
    int waitUntilHealed = 'h';
    int wait = 'w';
    int walk = 'e';
    int explore = 't';
    int instantInteract = 'j';

    //Open UI
    int inventory = 'i';
    int statsMenu = 's';



    //In Ui
    int uiUp = KEY_UP;
    int uiDown = KEY_DOWN;
    int uiLeft = KEY_LEFT;
    int uiRight = KEY_RIGHT;
    int console = '`';

    //In inventory
    int drop = 'd';
    //int dropAll = 'D';
    int equip = 'e';
    int sortInv = 's';

    //In chest
    int take = 't';
    int takeStack = 'T';

    int fav1 = '1';
    int fav2 = '2';
    int fav3 = '3';
    int fav4 = '4';
    int fav5 = '5';
    int fav6 = '6';
    int fav7 = '7';
    int fav8 = '8';
    int fav9 = '9';
    int fav0 = '0';

}

vector<KeyBind*> keybindings = {
    new KeyBind("Move Up", &Key::moveUp, keyCatPlayer),
    new KeyBind("Move Down", &Key::moveDown, keyCatPlayer),
    new KeyBind("Move Left", &Key::moveLeft, keyCatPlayer),
    new KeyBind("Move Right", &Key::moveRight, keyCatPlayer),
    new KeyBind("Interact / Use / Attack", &Key::interact, keyCatPlayer | keyCatUi),
    new KeyBind("Secondary Attack", &Key::secondaryAttack, keyCatPlayer),
    new KeyBind("Examine", &Key::inspect, keyCatPlayer),
    new KeyBind("Auto Walk", &Key::walk, keyCatPlayer),
    new KeyBind("Wait", &Key::wait, keyCatPlayer),
    new KeyBind("Auto Explore", &Key::explore, keyCatPlayer),
    new KeyBind("Instant Interact", &Key::instantInteract, keyCatPlayer),
    new KeyBind("Wait until Healed", &Key::waitUntilHealed, keyCatPlayer),
    new KeyBind("Open Inventory", &Key::inventory, keyCatPlayer | keyCatUi),
    new KeyBind("Open Skills", &Key::statsMenu, keyCatPlayer),
    new KeyBind("Ui Up", &Key::uiUp, keyCatUi),
    new KeyBind("Ui Down", &Key::uiDown, keyCatUi),
    new KeyBind("Ui Left", &Key::uiLeft, keyCatUi),
    new KeyBind("Ui Right", &Key::uiRight, keyCatUi),
    new KeyBind("Equip Item", &Key::equip, keyCatUi),
    new KeyBind("Drop Item", &Key::drop, keyCatUi),
    //KeyBind("Drop Stack", &Key::dropAll, keyCatUi),
    new KeyBind("Transfer Item", &Key::take, keyCatUi),
    new KeyBind("Transfer Stack", &Key::takeStack, keyCatUi),
    new KeyBind("Sort Inventory", &Key::sortInv, keyCatUi),

    new KeyBind("Open Console", &Key::adjustConsole, keyCatPlayer | keyCatUi),
    new KeyBind("Adjust Borders", &Key::adjustConsole, keyCatPlayer),

    new KeyBind("Favorite 1", &Key::fav1, keyCatPlayer | keyCatUi),
    new KeyBind("Favorite 2", &Key::fav2, keyCatPlayer | keyCatUi),
    new KeyBind("Favorite 3", &Key::fav3, keyCatPlayer | keyCatUi),
    new KeyBind("Favorite 4", &Key::fav4, keyCatPlayer | keyCatUi),
    new KeyBind("Favorite 5", &Key::fav5, keyCatPlayer | keyCatUi),
    new KeyBind("Favorite 6", &Key::fav6, keyCatPlayer | keyCatUi),
    new KeyBind("Favorite 7", &Key::fav7, keyCatPlayer | keyCatUi),
    new KeyBind("Favorite 8", &Key::fav8, keyCatPlayer | keyCatUi),
    new KeyBind("Favorite 9", &Key::fav9, keyCatPlayer | keyCatUi),
    new KeyBind("Favorite 0", &Key::fav0, keyCatPlayer | keyCatUi),
};

vector<int> reservedKeys = {
    KEY_ESCAPE, '\n', KEY_RESIZE, KEY_BACKSPACE, 127
};

string keyDisplayName(int key){
    switch (key) {
        case KEY_UP:
            return "Up";
        case KEY_DOWN:
            return "Down";
        case KEY_LEFT:
            return "Left";
        case KEY_RIGHT:
            return "Right";
        case KEY_ESCAPE:
            return "Escape";
        case '\n':
            return "Enter";
        case '\t':
            return "Tab";
        case ' ':
            return "Space";
        case KEY_BACKSPACE:
        case 127:
            return "Backspace";
        case 8:
        case KEY_DC:
            return "Delete";
        case KEY_F(1):
            return "F1";
        case KEY_F(2):
            return "F2";
        case KEY_F(3):
            return "F3";
        case KEY_F(4):
            return "F4";
        case KEY_F(5):
            return "F5";
        case KEY_F(6):
            return "F6";
        case KEY_F(7):
            return "F7";
        case KEY_F(8):
            return "F8";
        case KEY_F(9):
            return "F9";
        case KEY_F(10):
            return "F10";
        case KEY_F(11):
            return "F11";
        case KEY_F(12):
            return "F12";

        default:
            if(key >= 32 && key <= 126){
                return formatString("%c", (char)key);
            }else{
                return formatString("code(%d)", key);
            }
    }
}
