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
#include "Potion.h"
#include "UtilitySpell.h"
#include "ItemSpecial.h"

vector<string> consoleBuffer;

void debug(string s) {
    if (Ui::printDebugEnabled) {
        console("DEBUG: " + s);
    }
}

void console(string s) {
    size_t lastI = 0;
    for (size_t i = 0; i < s.length(); i++) {
        if (s[i] == '\n') {
            consoleBuffer.push_back(s.substr(lastI + 1, i - lastI));
            lastI = i;
        } else if (i - lastI > Ui::terminalSize.x - 4) {
            consoleBuffer.push_back(s.substr(lastI, i - lastI));
            lastI = i;
        }
    }
    consoleBuffer.push_back(s.substr(lastI));
}

void debugf(string format, ...) {
    char buff[256];
    va_list args;
    va_start(args, format);
    vsprintf(buff, format.c_str(), args);
    debug(buff);
    va_end(args);
}

void consolef(string format, ...) {
    char buff[256];
    va_list args;
    va_start(args, format);
    vsprintf(buff, format.c_str(), args);
    console(buff);
    va_end(args);
}

string formatString(string format, ...) {
    string s = "";

    char buff[256];
    va_list args;
    va_start(args, format);
    vsprintf(buff, format.c_str(), args);
    s += buff;
    va_end(args);

    return s;
}

string plural(string n) {
    return n + "s";
}

namespace Ui {

    Color C_DARK_BLACK = 0x0; // white when background is black
    Color C_DARK_RED = 0x1;
    Color C_DARK_GREEN = 0x2;
    Color C_DARK_YELLOW = 0x3;
    Color C_DARK_BLUE = 0x4;
    Color C_DARK_MAGENTA = 0x5;
    Color C_DARK_CYAN = 0x6;
    Color C_DARK_WHITE = 0x7;

    Color C_LIGHT_BLACK = 0x8;
    Color C_LIGHT_RED = 0x9;
    Color C_LIGHT_GREEN = 0xA;
    Color C_LIGHT_YELLOW = 0xB;
    Color C_LIGHT_BLUE = 0xC;
    Color C_LIGHT_MAGENTA = 0xD;
    Color C_LIGHT_CYAN = 0xE;
    Color C_LIGHT_WHITE = 0xF;

    Color C_LIGHT_GRAY = C_DARK_WHITE;
    Color C_DARK_GRAY = C_LIGHT_BLACK;

    Color C_WHITE = C_LIGHT_WHITE;
    Color C_BLACK = C_DARK_BLACK;

    unsigned long tick = 0;

    /*double ms = 0;

    int fps = 0;
    long l = 0;
    long lastL = 0;
    long startTime = 0;
    int frames = 0;*/

    bool limitedColorMode = true;

    Point2 terminalSize;

    bool printDebugEnabled = true;

    bool consoleInputMode = false;
    string consoleInput = "";
    int consoleScroll = 0;

    void initNCurses() {

        setlocale(LC_ALL, "");

        initscr();
        start_color();
        use_default_colors();
        setTerminalSizeVar();
        keypad(stdscr, TRUE);
        noecho();
        cbreak();
        curs_set(0);
        timeout(-1);
        ESCDELAY = 1;


        limitedColorMode = COLORS < 256;

        if (limitedColorMode) {
            swap(C_DARK_RED, C_DARK_BLUE);
            swap(C_LIGHT_RED, C_LIGHT_BLUE);
            swap(C_DARK_CYAN, C_DARK_YELLOW);
            swap(C_LIGHT_CYAN, C_LIGHT_YELLOW);
        }

        initColorPairs();

        refresh();

    }

    void initColorPairs() {

        short a = 0;

        for (short i = 0; i < 0x10; i++) {
            for (short j = 0; j < 0x10; j++) {
                init_pair(a, j, i);
                a++;
            }
        }

    }

    void setTerminalSizeVar() {
        terminalSize.x = getmaxx(stdscr);
        terminalSize.y = getmaxy(stdscr);
    }

    void setColor(Color fg, Color bg, int attr) {
        if (limitedColorMode) {
            if (fg == 0x8) {
                fg = 0x7;
            } else if (fg >= 0x8) {
                fg -= 0x8;
            }
            if (bg == 0x8) {
                bg = 0x7;
            } else if (bg >= 0x8) {
                bg -= 0x8;
            }
        }
        attrset(COLOR_PAIR(fg + (bg * 0x10)) | attr);
    }

    int printMultiLineString(int y, int x, string s, int maxX) {
        int a = 0;
        if (maxX == -1) {
            maxX = terminalSize.x;
        }
        int w = maxX - x;
        for (int i = 0; i < s.length(); i += w) {
            string ss = s.substr(i, w);
            if (ss.length() > 0) {
                if (ss[0] == ' ') {
                    ss = ss.substr(1);
                }
                mvprintw(y + a, x, ss.c_str());
                a += 1;
            }
        }
        return a;
    }

    bool addChColor(char c, bool* lookingForCode) {
        if (*lookingForCode) {
            *lookingForCode = false;
            if (c == '&') {
                addch('&');
                return true;
            } else {
                Color color = 0;
                switch (c) {
                    case C_CODE_DARK_BLACK:
                        color = C_DARK_BLACK;
                        break;
                    case C_CODE_DARK_RED:
                        color = C_DARK_RED;
                        break;
                    case C_CODE_DARK_GREEN:
                        color = C_DARK_GREEN;
                        break;
                    case C_CODE_DARK_YELLOW:
                        color = C_DARK_YELLOW;
                        break;
                    case C_CODE_DARK_BLUE:
                        color = C_DARK_BLUE;
                        break;
                    case C_CODE_DARK_MAGENTA:
                        color = C_DARK_MAGENTA;
                        break;
                    case C_CODE_DARK_CYAN:
                        color = C_DARK_CYAN;
                        break;
                    case C_CODE_DARK_WHITE:
                        color = C_DARK_WHITE;
                        break;
                    case C_CODE_LIGHT_BLACK:
                        color = C_LIGHT_BLACK;
                        break;
                    case C_CODE_LIGHT_RED:
                        color = C_LIGHT_RED;
                        break;
                    case C_CODE_LIGHT_GREEN:
                        color = C_LIGHT_GREEN;
                        break;
                    case C_CODE_LIGHT_YELLOW:
                        color = C_LIGHT_YELLOW;
                        break;
                    case C_CODE_LIGHT_BLUE:
                        color = C_LIGHT_BLUE;
                        break;
                    case C_CODE_LIGHT_MAGENTA:
                        color = C_LIGHT_MAGENTA;
                        break;
                    case C_CODE_LIGHT_CYAN:
                        color = C_LIGHT_CYAN;
                        break;
                    case C_CODE_LIGHT_WHITE:
                        color = C_LIGHT_WHITE;
                        break;

                    default:
                        color = C_WHITE;
                        break;
                }
                setColor(color);
            }
        } else if (c == '&') {
            *lookingForCode = true;
        } else {
            addch(c);
            return true;
        }
        return false;
    }

    int printMultiLineColoredString(int y, int x, string s, int maxX) {
        int a = 0;
        int b = 0;
        if (maxX == -1) {
            maxX = terminalSize.x;
        }
        move(y, x);
        bool lookingForCode = false;
        for (int i = 0; i < s.length(); i += 1) {
            if (!(s[i] == ' ' && b == 0)) {
                if (addChColor(s[i], &lookingForCode)) {
                    b++;
                }
            }
            if (b > maxX - x) {
                b = 0;
                a++;
            }
        }
        setColor(C_WHITE);
        return a;
    }

<<<<<<< Updated upstream
    void drawInventory(Player* player, int selectedY/*, int scrollOffset*/, Inventory* secondaryInv, string invDisplayName, bool selectedLeft) {
=======
    void drawInventory(World* currentWorld, Player* player, int selectedY/*, int scrollOffset*/, Inventory* secondaryInv, string invDisplayName, bool selectedLeft){
>>>>>>> Stashed changes

        int columnX = 0;
        int columnWidth = (terminalSize.x) / (secondaryInv == nullptr ? 2 : 3);

        Weapon* activeWeapon = player->getActiveWeapon();

        move(0, 0);
        clrtobot();


        vector<Item*> inv = (selectedLeft ? player : secondaryInv)->inventory;
        if (inv.size() > 0) {
            Item* item = inv[selectedY];
            if (item) {
                setColor(C_DARK_GRAY, C_BLACK);
                if (item->artIndex > -1) {
                    Art* art = Arts::getArt(item->artIndex);
                    Point2 artSize = art->getSize();
                    art->printAt(terminalSize - artSize - Point2One);
                }
                setColor(C_WHITE, C_BLACK);
            }
        }

        for (int ii = 0; ii < (secondaryInv == nullptr ? 1 : 2); ii++) {
            vector<Item*> inv = (ii == 1 ? secondaryInv : player)->inventory;

            setColor(C_WHITE);

            columnX = (ii * columnWidth);
            //mvaddch(1, columnX+columnWidth-1, '+');
            //mvvline(2, columnX+columnWidth-1, '|', terminalSize.y-2);

            int countI = terminalSize.y - 3;


            int midI = (countI / 2) + selectedY;
            int minI = Math::max(0, midI - countI);
            int maxI = Math::min(midI + countI, (int) inv.size());
            if (((ii == 0) != selectedLeft)) {
                setColor(C_LIGHT_GRAY);
            }
            mvprintw(0, columnX, (ii == 1 ? invDisplayName.c_str() : "Player"));

            int y = 2;

            for (int i = minI; i < maxI; i++) {
                Item* item = inv[i];

                setColor(((ii == 0) == selectedLeft) ? C_WHITE : C_LIGHT_GRAY);
                if (i == selectedY && (((ii == 0) == selectedLeft))) {
                    setColor(C_BLACK, C_WHITE);
                }

                char pre = activeWeapon != nullptr ? (item == activeWeapon ? 'E' : '-') : '-';
                string displayName;
                if (item->qty == 1) {
                    displayName = formatString("%s", item->name.c_str());
                } else {
                    displayName = formatString("%d %s", item->qty, plural(item->name).c_str());
                }
                mvprintw(y, columnX, "%c ", pre);
                y += printMultiLineString(y, columnX + 2, displayName, columnX + columnWidth - 1);

            }


        }
        setColor(C_WHITE, C_BLACK);

        columnX += columnWidth;

        if (inv.size() > 0) {
            Item* item = inv[selectedY];

<<<<<<< Updated upstream
            if (item) {
                Equipable* equipable = dynamic_cast<Equipable*> (item);
                Weapon* weapon = dynamic_cast<Weapon*> (item);
                Ranged* ranged = dynamic_cast<Ranged*> (item);
                CombatSpell* spell = dynamic_cast<CombatSpell*> (item);
                Potion* potion = dynamic_cast<Potion*> (item);
                UtilitySpell* utilitySpell = dynamic_cast<UtilitySpell*> (item);
=======
            if(item){
                Equipable* equipable = dynamic_cast<Equipable*>(item);
                Weapon* weapon = dynamic_cast<Weapon*>(item);
                Ranged* ranged = dynamic_cast<Ranged*>(item);
                CombatSpell* spell = dynamic_cast<CombatSpell*>(item);
                Potion* potion = dynamic_cast<Potion*>(item);
                UtilitySpell* utilitySpell = dynamic_cast<UtilitySpell*>(item);
                ItemSpecial* itemSpecial = dynamic_cast<ItemSpecial*>(item);
>>>>>>> Stashed changes

                int a = 2;

                a += printMultiLineString(a, columnX, formatString("Name: %s%s", item->name.c_str(), item->qty == 1 ? "" : (formatString(" x %d", item->qty).c_str())));
                if (item == activeWeapon) {
                    mvprintw(terminalSize.y - 1, columnX, "-Equipped");
                } else {
                    mvprintw(terminalSize.y - 1, columnX, "");
                }

                setColor(C_LIGHT_GRAY, C_BLACK);

                //mvprintw(a++, columnX, "Qty: %d", item->qty);
                a += printMultiLineString(a, columnX, formatString("Weight: %.2f", item->weight));
                if (potion) {
                    a += printMultiLineString(a, columnX, "Effects:");
                    if (potion->effects.size() == 0) {
                        a += printMultiLineString(a, columnX, "   None");
                    } else {
                        for (Effect e : potion->effects) {
                            if (e.timeLeft > 0) {
                                a += printMultiLineString(a, columnX, formatString("   %s %.2f for %.2f", Weapon::effectName(e.eId, e.meta).c_str(), e.power, e.timeLeft));
                            } else {
                                a += printMultiLineString(a, columnX, formatString("   %s %.2f", Weapon::effectName(e.eId, e.meta).c_str(), e.power));
                            }
                        }
                    }
                    setColor(C_LIGHT_RED);
                    a += printMultiLineString(a, columnX, "One Use");
                    setColor(C_LIGHT_GRAY, C_BLACK);
                } else if (utilitySpell) {
                    switch (utilitySpell->spellEffect) {
                        case spellRemoteUse:
                            a += printMultiLineString(a, columnX, "Interact with tile or entity at a distance.");
                            break;

                        case spellRelocate:
                            a += printMultiLineString(a, columnX, "Transport to selected location or random location if already there.");
                            break;

                        default:
                            break;
                    }
                    if (utilitySpell->manaCost == -1) {
                        setColor(C_LIGHT_RED);
                        a += printMultiLineString(a, columnX, "One Use");
                        setColor(C_LIGHT_GRAY, C_BLACK);
                    } else {
                        a += printMultiLineString(a, columnX, formatString("Mana Cost: %d", utilitySpell->manaCost));
                    }
                } else if (weapon) {
                    double x = player->calcDamageMultiplier(weapon);
                    if (x == 1.0) {
                        a += printMultiLineString(a, columnX, formatString("Damage: %.2f", weapon->baseDamage));
                    } else {
                        a += printMultiLineString(a, columnX, formatString("Damage: %.2f (%.2f)", weapon->baseDamage, weapon->baseDamage * x));
                    }
                    a += printMultiLineString(a, columnX, formatString("Damage Type: %s", Weapon::damageTypeName(weapon->damageType).c_str()));
                    a += printMultiLineString(a, columnX, formatString("Speed: %d%%", (int) (100 / weapon->useDelay)));
                    if (ranged) {
                        a += printMultiLineString(a, columnX, formatString("Range: %.2f", ranged->range));
                        if (spell) {
                            a += printMultiLineString(a, columnX, formatString("Mana Cost: %d", spell->manaCost));
                        }
                    }
                    if (!spell && weapon->enchantments.size() > 0) {
                        a++;
                        a += printMultiLineString(a, columnX, formatString("Enchantments:"));
                        for (Enchantment e : weapon->enchantments) {
                            a += printMultiLineString(a, columnX, formatString("   %s x%d (1/%d for %.2f)", Weapon::enchantmentName(e).c_str(), e.power, e.chance, e.time));
                        }
                        a++;
                    }
                    setColor(C_LIGHT_CYAN);
                    a += printMultiLineString(a, columnX, formatString("d/t: %.2f", ((weapon->baseDamage * x) / weapon->useDelay)));
                    if (spell) {
                        a += printMultiLineString(a, columnX, formatString("d/m: %.2f", ((spell->baseDamage * x) / (double) spell->manaCost)));
                    }
                    setColor(C_LIGHT_GRAY, C_BLACK);

                    int b = a;
                    a++;
                    bool c = false;

                    for (int i = 0; i < abilityCount; i++) {
                        if (equipable->minimumAbilities[i] > 0) {
                            a += printMultiLineString(a, columnX, formatString("- %s", abilityAbr[i].c_str()), terminalSize.x - 4);
                            if (equipable->minimumAbilities[i] > player->abilities[i]) {
                                setColor(C_LIGHT_RED);
                            }
                            printw(" %d", equipable->minimumAbilities[i]);
                            setColor(C_LIGHT_GRAY);
                            c = true;
                        }
                    }
                    if (c) {
                        mvprintw(b, columnX, "Requires: ");
                    }
                }else if(itemSpecial){
                    switch (itemSpecial->specialty) {
                        case specialtyCompass:{
                            int frame = 0;

                            Point2 diff = player->pos - currentWorld->currentLevel->stairDownPos;

                            double angle = 360 - ((atan2((double)diff.y, (double)-diff.x) * 180 / Math::pi) + 180);

                            if((angle >= 0 && angle < 22.5) || (angle >= 337.5 && angle <= 360)){ //W
                                frame = 1;
                            }else if(angle >= 22.5 && angle < 67.5){//NW
                                frame = 2;
                            }else if(angle >= 67.5 && angle < 112.5){//N
                                frame = 3;
                            }else if(angle >= 112.5 && angle < 157.5){//NE
                                frame = 4;
                            }else if(angle >= 157.5 && angle < 202.5){//E
                                frame = 5;
                            }else if(angle >= 202.5 && angle < 247.5){//SE
                                frame = 6;
                            }else if(angle >= 247.5 && angle < 292.5){//S
                                frame = 7;
                            }else if(angle >= 292.5 && angle < 337.5){//SW
                                frame = 8;
                            }else{
                                frame = 0;
                            }
                            
                            consolef("%f", angle);
                            
                            
                            Arts::getArt(Arts::compassInList[frame])->printAt(Point2(columnX, a));
                            break;
                        }
                    }
                }

            }
        }

        setColor(C_WHITE);
        mvhline(1, 0, '-', terminalSize.x);



        for (int ii = 0; ii < (secondaryInv == nullptr ? 1 : 2); ii++) {
            vector<Item*> inv = (ii == 1 ? secondaryInv : player)->inventory;

            setColor(C_WHITE);

            columnX = (ii * columnWidth);
            mvaddch(1, columnX + columnWidth - 1, '+');
            mvvline(2, columnX + columnWidth - 1, '|', terminalSize.y - 2);
        }

        const int hp = Math::roundToInt(player->getHp());
        const int maxHp = Math::roundToInt(player->getMaxHp());
        const int mp = Math::roundToInt(player->getMp());
        const int maxMp = Math::roundToInt(player->getMaxMp());

        move(0, terminalSize.x - 15);
        clrtoeol();

        setColor(C_LIGHT_GREEN);
        mvprintw(0, terminalSize.x - 15, "%3d/%3d", hp, maxHp);
        setColor(C_LIGHT_BLUE);
        mvprintw(0, terminalSize.x - 7, "%3d/%3d", mp, maxMp);



        /*
         setColor(C_WHITE);
         int minI = Math::max(0, scrollOffset);
         int maxI = (int)inv->inventory.size() - scrollOffset;
         if(inv->inventory.size() == 0){
         minI = 0;
         maxI = -1;
         }
         move(0, 0);
         clrtobot();
         int totalWeight = 0;
         forVector(inv->inventory, i){
         totalWeight += inv->inventory[i]->weight * inv->inventory[i]->qty;
         }
         mvprintw(0, 0, "%s   Total Weight: %-3.1d", display.c_str(), totalWeight);

         setColor(C_BLACK, C_WHITE);
         mvhline(selected+3, 0, ' ', terminalSize.x);
         setColor(C_WHITE, C_BLACK);

         mvprintw(1, 0, "Item");
         mvprintw(1, columnWidth, "Information");mvvline(1, columnInfo-1, '|', terminalSize.y);

         mvhline(2, 0, '-', terminalSize.x);
         Item* item;
         Weapon* weapon;
         Ranged* ranged;
         CombatSpell* spell;
         int y = 3;
         for(int i=minI;i<maxI;i++){
         item = inv->inventory[i];
         weapon = dynamic_cast<Weapon*>(item);
         ranged = dynamic_cast<Ranged*>(item);
         spell = dynamic_cast<CombatSpell*>(item);
         if(i == selected){
         setColor(C_BLACK, C_WHITE);
         }
         char pre = activeWeapon!=nullptr?(item == activeWeapon?'E':' '):' ';
         string displayName;
         if(item->qty > 1){
         displayName = formatString("%c  %d x %s", pre, item->qty, item->name.c_str());
         }else{
         displayName = formatString("%c  %s", pre, item->name.c_str());
         }
         mvprintw(y, 0, "%*-s", columnWidth, displayName.c_str());

         //mvprintw(y, columnWeight+1, "%2.1f", item->weight);

         //if(weapon){
         //mvprintw(y, columnHitPoints+1, "%3.1f", weapon->baseDamage);
         //}
         if(i == selected){
         setColor(C_WHITE, C_BLACK);
         }

         mvhline(y, columnInfo, ' ', terminalSize.x-columnInfo);

         y++;
         }

         if(inv->inventory.size() > 0){
         item = inv->inventory[selected];

         if(item){
         weapon = dynamic_cast<Weapon*>(item);
         ranged = dynamic_cast<Ranged*>(item);
         spell = dynamic_cast<CombatSpell*>(item);

         int a = 3;

         setColor(C_DARK_GRAY, C_BLACK);
         if(item->artIndex > -1){
         debug("draw art");
         Art* art = Arts::getArt(item->artIndex);
         Point2 artSize = art->getSize();
         art->printAt(terminalSize - artSize - Point2One);
         }else{
         debug("did not draw art");
         }
         setColor(C_WHITE, C_BLACK);

         mvprintw(a++, columnInfo, "Name: %s", item->name.c_str());
         if(item == activeWeapon){
         mvprintw(terminalSize.y-1, columnInfo, "-Equipped");
         }else{
         mvprintw(terminalSize.y-1, columnInfo, "");
         }

         setColor(C_LIGHT_GRAY, C_BLACK);

         mvprintw(a++, columnInfo, "Qty: %d", item->qty);
         mvprintw(a++, columnInfo, "Weight: %.2f", item->weight);
         if(weapon){
         mvprintw(a++, columnInfo, "Damage: %.2f", weapon->baseDamage);
         mvprintw(a++, columnInfo, "Damage Type: %s", Weapon::damageTypeName(weapon->damageType).c_str());
         mvprintw(a++, columnInfo, "Use Delay: %.2f (d/t: %.2f)", weapon->useDelay, (weapon->baseDamage/weapon->useDelay));
         if(ranged){
         mvprintw(a++, columnInfo, "Range: %.2f", ranged->range);
         if(spell){
         mvprintw(a++, columnInfo, "Mana Cost: %d (d/m: %.2f)", spell->manaCost, (spell->baseDamage/(double)spell->manaCost));
         }
         }
         if(!spell && weapon->enchantments.size() > 0){
         a++;
         mvprintw(a++, columnInfo, "Enchantments: ");
         for(Enchantment e : weapon->enchantments){
         mvprintw(a++, columnInfo, "   %s x%d (1/%d for %.2f)", Weapon::enchantmentName(e).c_str(), e.power, e.chance, e.time);
         }
         a++;
         }
         }

         }
         }*/
    }
}
