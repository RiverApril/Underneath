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
#include "UtiitySpell.h"

vector<string> consoleBuffer;

void debug(string s){
    if(Ui::printDebugEnabled){
    	console("DEBUG: "+s);
    }
}

void console(string s){
    size_t lastI = 0;
    for(size_t i=0;i<s.length();i++){
        if(s[i] == '\n'){
            consoleBuffer.push_back(s.substr(lastI+1, i-lastI));
            lastI = i;
        }else if(i-lastI > Ui::terminalSize.x - 4){
            consoleBuffer.push_back(s.substr(lastI, i-lastI));
            lastI = i;
        }
    }
    consoleBuffer.push_back(s.substr(lastI));
}


void debugf(string format, ...){
    char buff[256];
    va_list args;
    va_start(args, format);
    vsprintf(buff, format.c_str(), args);
    debug(buff);
    va_end(args);
}


void consolef(string format, ...){
	char buff[256];
    va_list args;
    va_start(args, format);
    vsprintf(buff, format.c_str(), args);
    console(buff);
    va_end(args);
}

string formatString(string format, ...){
    string s = "";
    
    char buff[256];
    va_list args;
    va_start(args, format);
    vsprintf(buff, format.c_str(), args);
    s+=buff;
    va_end(args);

    return s;
}

string plural(string n){
    return n+"s";
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


        limitedColorMode = COLORS<256;

		if (limitedColorMode){
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

        for(short i = 0; i<0x10; i++) {
            for(short j = 0; j<0x10; j++) {
            	init_pair(a, j, i);
                a++;
            }
        }

    }

    void setTerminalSizeVar(){
        terminalSize.x = getmaxx(stdscr);
        terminalSize.y = getmaxy(stdscr);
    }

    void setColor(Color fg, Color bg, int attr) {
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

    void drawInventory(Player* player, int selectedY, int scrollOffset, Inventory* secondaryInv, string invDisplayName, bool selectedLeft){

        int columnX = 0;
        int columnWidth = (terminalSize.x-30)/(player==nullptr?1:2);

        Weapon* activeWeapon = player->getActiveWeapon();

        move(0, 0);
        clrtobot();

        setColor(C_WHITE);
        mvhline(1, 0, '-', terminalSize.x);

        for(int ii = 0; ii < (secondaryInv==nullptr?1:2); ii++){
            vector<Item*> inv = (ii==1?secondaryInv:player)->inventory;

            setColor(C_WHITE);

            columnX = (ii * columnWidth);
            mvaddch(1, columnX+columnWidth-1, '+');
            mvvline(2, columnX+columnWidth-1, '|', terminalSize.y-2);

            int minI = Math::max(0, scrollOffset);
            int maxI = Math::min(minI+terminalSize.y-3, (int)inv.size());
            if(((ii==0)!=selectedLeft)){
                setColor(C_LIGHT_GRAY);
            }
            mvprintw(0, columnX, (ii==1?invDisplayName.c_str():"Player"));

            int y = 1;

            for(int i = minI; i < maxI; i++){
                y++;
                Item* item = inv[i];

                setColor(((ii==0)==selectedLeft)?C_WHITE:C_LIGHT_GRAY);
                if(i == selectedY && (((ii==0) == selectedLeft))){
                    setColor(C_BLACK, C_WHITE);
                }

                char pre = activeWeapon!=nullptr?(item == activeWeapon?'E':' '):' ';
                string displayName;
                if(item->qty == 1){
                    displayName = formatString("%c  %s", pre, item->name.c_str());
                }else{
                    displayName = formatString("%c  %d %s", pre, item->qty, plural(item->name).c_str());
                }
                mvprintw(y, columnX, "%*-s", columnWidth-1, displayName.c_str());

            }


        }

        columnX += columnWidth;

        vector<Item*> inv = (selectedLeft?player:secondaryInv)->inventory;

        if(inv.size() > 0){
            Item* item = inv[selectedY];

            if(item){
                Equipable* equipable = dynamic_cast<Equipable*>(item);
                Weapon* weapon = dynamic_cast<Weapon*>(item);
                Ranged* ranged = dynamic_cast<Ranged*>(item);
                CombatSpell* spell = dynamic_cast<CombatSpell*>(item);
                Potion* potion = dynamic_cast<Potion*>(item);
                UtilitySpell* utilitySpell = dynamic_cast<UtilitySpell*>(item);

                int a = 2;

                setColor(C_DARK_GRAY, C_BLACK);
                if(item->artIndex > -1){
                    Art* art = Arts::getArt(item->artIndex);
                    Point2 artSize = art->getSize();
                    art->printAt(terminalSize - artSize - Point2One);
                }
                setColor(C_WHITE, C_BLACK);

                mvprintw(a++, columnX, "Name: %s%s", item->name.c_str(), item->qty==1?"":(formatString(" x %d", item->qty).c_str()));
                if(item == activeWeapon){
                    mvprintw(terminalSize.y-1, columnX, "-Equipped");
                }else{
                    mvprintw(terminalSize.y-1, columnX, "");
                }

                setColor(C_LIGHT_GRAY, C_BLACK);

                //mvprintw(a++, columnX, "Qty: %d", item->qty);
                mvprintw(a++, columnX, "Weight: %.2f", item->weight);
                if(potion){
                    mvprintw(a++, columnX, "Effects:");
                    if(potion->effects.size() == 0){
                        mvprintw(a++, columnX, "   None");
                    }else{
                        for(Effect e : potion->effects){
                            if(e.timeLeft > 0){
                            	mvprintw(a++, columnX, "   %s %.2f for %.2f", Weapon::effectName(e.eId, e.meta).c_str(), e.power, e.timeLeft);
                            }else{
                                mvprintw(a++, columnX, "   %s %.2f", Weapon::effectName(e.eId, e.meta).c_str(), e.power);
                            }
                        }
                    }
                }else if(utilitySpell){
                    switch (utilitySpell->spellEffect) {
                        case spellRemoteUse:
                            mvprintw(a++, columnX, "Interact with tile or entity at a distance.");
                            break;

                        case spellRelocate:
                            mvprintw(a++, columnX, "Transport to selected location");
                            mvprintw(a++, columnX, "or random location if already there.");
                            break;
                            
                        default:
                            break;
                    }
                    if(utilitySpell->manaCost == -1){
                        setColor(C_LIGHT_RED);
                        mvprintw(a++, columnX, "One Use");
                        setColor(C_LIGHT_GRAY, C_BLACK);
                    }else{
                        mvprintw(a++, columnX, "Mana Cost: %d", utilitySpell->manaCost);
                    }
                }else if(weapon){
                    mvprintw(a++, columnX, "Damage: %.2f", weapon->baseDamage);
                    mvprintw(a++, columnX, "Damage Type: %s", Weapon::damageTypeName(weapon->damageType).c_str());
                    mvprintw(a++, columnX, "Speed: %d%%", (int)(100/weapon->useDelay));
                    if(ranged){
                        mvprintw(a++, columnX, "Range: %.2f", ranged->range);
                        if(spell){
                            mvprintw(a++, columnX, "Mana Cost: %d", spell->manaCost);
                        }
                    }
                    if(!spell && weapon->enchantments.size() > 0){
                        a++;
                        mvprintw(a++, columnX, "Enchantments:");
                        for(Enchantment e : weapon->enchantments){
                            mvprintw(a++, columnX, "   %s x%d (1/%d for %.2f)", Weapon::enchantmentName(e).c_str(), e.power, e.chance, e.time);
                        }
                        a++;
                    }

                    mvprintw(a++, columnX, "d/t: %.2f", (weapon->baseDamage/weapon->useDelay));
                    if(spell){
                        mvprintw(a++, columnX, "d/m: %.2f", (spell->baseDamage/(double)spell->manaCost));
                    }

                    int b = a;
                    a++;
                    bool c = false;

                    for(int i=0;i<abilityCount;i++){
                        if(equipable->minimumAbilities[i] > 0){
                            mvprintw(a++, columnX, "- %s", abilityAbr[i].c_str());
                            if(equipable->minimumAbilities[i] > player->abilities[i]){
                                setColor(C_LIGHT_RED);
                            }
                            printw(" %d", equipable->minimumAbilities[i]);
                            setColor(C_LIGHT_GRAY);
                            c = true;
                        }
                    }
                    if(c){
                    	mvprintw(b, columnX, "Requires: ");
                    }
                }

            }
        }

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
