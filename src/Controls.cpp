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
    int secondaryAttack = 'f';
    int inspect = 'q';
    int adjustConsole = '\\';
    int waitUntilHealed = 'h';

    //Open UI
    int inventory = 'i';
    int statsMenu = 's';
    int debugMenu = 'd';



    //In Ui
    int uiUp = KEY_UP;
    int uiDown = KEY_DOWN;
    int uiLeft = KEY_LEFT;
    int uiRight = KEY_RIGHT;

    //In inventory
    int drop = 'd';
    int dropAll = 'D';
    int equip = 'e';

    //In chest
    int take = 't';
    int takeStack = 'T';
    int takeAll = 'R';

}

vector<KeyBind> keybindings = {
    KeyBind("Move Up", &Key::moveUp, keyCatPlayer),
    KeyBind("Move Down", &Key::moveDown, keyCatPlayer),
    KeyBind("Move Left", &Key::moveLeft, keyCatPlayer),
    KeyBind("Move Right", &Key::moveRight, keyCatPlayer),
    KeyBind("Interact / Use / Attack", &Key::interact, keyCatPlayer | keyCatUi),
    KeyBind("Secondary Attack", &Key::secondaryAttack, keyCatPlayer),
    KeyBind("Examine", &Key::inspect, keyCatPlayer),
    KeyBind("Heal", &Key::waitUntilHealed, keyCatPlayer),
    KeyBind("Open Inventory", &Key::inventory, keyCatPlayer | keyCatUi),
    KeyBind("Open Skills", &Key::statsMenu, keyCatPlayer),
    KeyBind("Ui Up", &Key::uiUp, keyCatUi),
    KeyBind("Ui Down", &Key::uiDown, keyCatUi),
    KeyBind("Ui Left", &Key::uiLeft, keyCatUi),
    KeyBind("Ui Right", &Key::uiRight, keyCatUi),
    KeyBind("Equip Item", &Key::equip, keyCatUi),
    KeyBind("Drop Item", &Key::drop, keyCatUi),
    KeyBind("Drop Stack", &Key::dropAll, keyCatUi),
    KeyBind("Transfer Item", &Key::take, keyCatUi),
    KeyBind("Transfer Stack", &Key::takeStack, keyCatUi),
    KeyBind("Transfer Inventory", &Key::takeAll, keyCatUi),

    KeyBind("Adjust Borders", &Key::adjustConsole, keyCatPlayer),
    KeyBind("Open Debug Menu", &Key::debugMenu, keyCatPlayer)
};

vector<int> reservedKeys = {
    KEY_ESCAPE, '\n'
};
