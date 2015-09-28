//
//  Controls.cpp
//  Underneath
//
//  Created by Braeden Atlee on 12/27/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "Controls.hpp"

namespace Key {

    //In Game
    int moveUp = KEY_UP;
    int moveDown = KEY_DOWN;
    int moveLeft = KEY_LEFT;
    int moveRight = KEY_RIGHT;
    int interact = 'g';
    int secondaryAttack = 'G';
    int inspect = 'q';
    int adjustConsole = '\\';
    int waitUntilHealed = 'h';
    int wait = 'w';
    int walk = 'W';

    //Open UI
    int inventory = 'i';
    int statsMenu = 's';



    //In Ui
    int uiUp = KEY_UP;
    int uiDown = KEY_DOWN;
    int uiLeft = KEY_LEFT;
    int uiRight = KEY_RIGHT;

    //In inventory
    int drop = 'd';
    int dropAll = 'D';
    int equip = 'e';
    int equipPrimary = 'E';
    int sortInv = 's';

    //In chest
    int take = 't';
    int takeStack = 'T';

}

vector<KeyBind> keybindings = {
    KeyBind("Move Up", &Key::moveUp, keyCatPlayer),
    KeyBind("Move Down", &Key::moveDown, keyCatPlayer),
    KeyBind("Move Left", &Key::moveLeft, keyCatPlayer),
    KeyBind("Move Right", &Key::moveRight, keyCatPlayer),
    KeyBind("Interact / Use / Attack", &Key::interact, keyCatPlayer | keyCatUi),
    KeyBind("Secondary Attack", &Key::secondaryAttack, keyCatPlayer),
    KeyBind("Examine", &Key::inspect, keyCatPlayer),
    KeyBind("Auto Walk", &Key::walk, keyCatPlayer),
    KeyBind("Wait", &Key::wait, keyCatPlayer),
    KeyBind("Wait until Healed", &Key::waitUntilHealed, keyCatPlayer),
    KeyBind("Open Inventory", &Key::inventory, keyCatPlayer | keyCatUi),
    KeyBind("Open Skills", &Key::statsMenu, keyCatPlayer),
    KeyBind("Ui Up", &Key::uiUp, keyCatUi),
    KeyBind("Ui Down", &Key::uiDown, keyCatUi),
    KeyBind("Ui Left", &Key::uiLeft, keyCatUi),
    KeyBind("Ui Right", &Key::uiRight, keyCatUi),
    KeyBind("Equip Item", &Key::equip, keyCatUi),
    KeyBind("Equip Item as Primary", &Key::equipPrimary, keyCatUi),
    KeyBind("Drop Item", &Key::drop, keyCatUi),
    KeyBind("Drop Stack", &Key::dropAll, keyCatUi),
    KeyBind("Transfer Item", &Key::take, keyCatUi),
    KeyBind("Transfer Stack", &Key::takeStack, keyCatUi),
    KeyBind("Sort Inventory", &Key::sortInv, keyCatUi),

    KeyBind("Adjust Borders", &Key::adjustConsole, keyCatPlayer)
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
        case ' ':
            return "Space";
        case KEY_BACKSPACE:
        case 127:
            return "Backspace";
        case 8:
            return "Delete";

        default:
            return string()+(char)key;
    }
}
