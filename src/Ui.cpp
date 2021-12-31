//
//  Ui.cpp
//  Underneath
//
//  Created by Emily Atlee on 10/19/14.
//  Copyright (c) 2014 Emily Atlee. All rights reserved.
//

#include "Ui.hpp"
#include "MenuGame.hpp"
#include "Entity.hpp"
#include "EntityAi.hpp"
#include "EntityPlayer.hpp"
#include "Math.hpp"
#include "ItemPotion.hpp"
#include "ItemUtilitySpell.hpp"
#include "ItemSpecial.hpp"
#include "ItemExplosive.hpp"
#include "ItemArmor.hpp"
#include "Settings.hpp"
#include "ItemAreaOfEffectWeapon.hpp"

vector<string> consoleBuffer;

void debug(string s) {
    if (Settings::debugMode) {
        console("DEBUG: " + s);
    }
    if(Settings::logFile){
        ofstream log_file(string(UnderneathDir)+"debug.log", ios_base::out | ios_base::app );
        log_file << s << endl;
    }
}

void console(string s) {
    /*size_t lastI = 0;
    for (size_t i = 0; i < s.length(); i++) {
        if (s[i] == '\n') {
            consoleBuffer.push_back(s.substr(lastI + 1, i - lastI));
            lastI = i;
        } else if (i - lastI > Ui::terminalSize.x - 4) {
            consoleBuffer.push_back(s.substr(lastI, i - lastI));
            lastI = i;
        }
    }
    consoleBuffer.push_back(s.substr(lastI));*/
    consoleBuffer.push_back(s);
    if(Settings::logFile){
        ofstream log_file(string(UnderneathDir)+"console.log", ios_base::out | ios_base::app );
        log_file << s << endl;
    }
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

    unsigned long tick = 0;

    /*double ms = 0;

    int fps = 0;
    long l = 0;
    long lastL = 0;
    long startTime = 0;
    int frames = 0;*/

    bool limitedColorMode = true;

    Point2 terminalSize;

    bool debugMode = true;

    bool consoleInputMode = false;
    string consoleInput = "";
    int consoleScroll = 0;

    void initNCurses(int limitedColorModeOverride) {

        setlocale(LC_ALL, "");

        initscr();
        start_color();
        use_default_colors();
        setTerminalSizeVar();
        keypad(stdscr, TRUE);
        noecho();
        cbreak();
        curs_set(0);
        timeout(defaultTimeout);
        ESCDELAY = 1;


        limitedColorMode = (COLORS < 16) || limitedColorModeOverride;

        /*if (limitedColorMode) {

			//swap(C_DARK_BLUE, C_DARK_RED);
			//swap(C_DARK_YELLOW, C_DARK_CYAN);

            C_LIGHT_BLACK = C_DARK_WHITE;
            C_LIGHT_RED = C_DARK_RED;
            C_LIGHT_GREEN = C_DARK_GREEN;
            C_LIGHT_YELLOW = C_DARK_YELLOW;
            C_LIGHT_BLUE = C_DARK_BLUE;
            C_LIGHT_MAGENTA = C_DARK_MAGENTA;
            C_LIGHT_CYAN = C_DARK_CYAN;
            C_LIGHT_WHITE = C_DARK_WHITE;
        }*/

        initColorPairs();

        bkgd(getColorPair(C_WHITE, C_BLACK));

        //refresh();

    }

    void initColorPairs() {

        /*if(limitedColorMode){

            short a = 0;

            for (short i = 0; i < 0x08; i++) {
                for (short j = 0; j < 0x08; j++) {
                    init_pair(a, j, i);
                    a++;
                }
            }

        }else*/{

            short a = 0;

            for (short i = 0; i < 0x10; i++) {
                for (short j = 0; j < 0x10; j++) {
                    init_pair(a, j, i);
                    a++;
                }
            }

        }

    }

    void setTerminalSizeVar() {
        terminalSize.x = getmaxx(stdscr);
        terminalSize.y = getmaxy(stdscr);
    }

    void setColor(Color fg) {
        setColor(fg, C_BLACK, 0);
    }

    void setColor(Color fg, Color bg, int attr) {
        if (limitedColorMode) {
            switch (bg) {
                case C_LIGHT_BLACK: bg = C_DARK_BLACK;break;
                case C_LIGHT_RED: bg = C_DARK_RED;break;
                case C_LIGHT_GREEN: bg = C_DARK_GREEN;break;
                case C_LIGHT_YELLOW: bg = C_DARK_YELLOW;break;
                case C_LIGHT_BLUE: bg = C_DARK_BLUE;break;
                case C_LIGHT_MAGENTA: bg = C_DARK_MAGENTA;break;
                case C_LIGHT_CYAN: bg = C_DARK_CYAN;break;
                case C_LIGHT_WHITE: bg = C_DARK_WHITE;break;
            }
            switch (fg) {
                case C_LIGHT_BLACK: fg = C_DARK_WHITE;break;
                case C_LIGHT_RED: fg = C_DARK_RED;break;
                case C_LIGHT_GREEN: fg = C_DARK_GREEN;break;
                case C_LIGHT_YELLOW: fg = C_DARK_YELLOW;break;
                case C_LIGHT_BLUE: fg = C_DARK_BLUE;break;
                case C_LIGHT_MAGENTA: fg = C_DARK_MAGENTA;break;
                case C_LIGHT_CYAN: fg = C_DARK_CYAN;break;
                case C_LIGHT_WHITE: fg = (bg == C_DARK_BLACK)?C_DARK_BLACK:C_DARK_WHITE;break;
            }
        }
        attrset(getColorPair(fg, bg) | attr);
    }

    int getColorPair(Color fg, Color bg){
        return COLOR_PAIR(fg + (bg * 0x10));
    }

    int printMultiLineString(int y, int x, string s, int maxX) {
        int a = 0;
        if (maxX == -1) {
            maxX = terminalSize.x;
        }
        int w = maxX - x;
        if(w <= 0){
            return a;
        }
        for (int i = 0; i < (int)s.length(); i += w) {
            string ss = s.substr(i, w);
            if (ss.length() > 0) {
                if (ss[0] == ' ') {
                    ss = ss.substr(1);
                }
                mvprintw(y + a, x, "%s", ss.c_str());
                a += 1;
            }
        }
        return a;
    }

    /*Color codeToColor(char c){
        switch (c) {
            case C_CODE_DARK_BLACK:
                return C_DARK_BLACK;
            case C_CODE_DARK_RED:
                return C_DARK_RED;
            case C_CODE_DARK_GREEN:
                return C_DARK_GREEN;
            case C_CODE_DARK_YELLOW:
                return C_DARK_YELLOW;
            case C_CODE_DARK_BLUE:
                return C_DARK_BLUE;
            case C_CODE_DARK_MAGENTA:
                return C_DARK_MAGENTA;
            case C_CODE_DARK_CYAN:
                return C_DARK_CYAN;
            case C_CODE_DARK_WHITE:
                return C_DARK_WHITE;
            case C_CODE_LIGHT_BLACK:
                return C_LIGHT_BLACK;
            case C_CODE_LIGHT_RED:
                return C_LIGHT_RED;
            case C_CODE_LIGHT_GREEN:
                return C_LIGHT_GREEN;
            case C_CODE_LIGHT_YELLOW:
                return C_LIGHT_YELLOW;
            case C_CODE_LIGHT_BLUE:
                return C_LIGHT_BLUE;
            case C_CODE_LIGHT_MAGENTA:
                return C_LIGHT_MAGENTA;
            case C_CODE_LIGHT_CYAN:
                return C_LIGHT_CYAN;
            case C_CODE_LIGHT_WHITE:
                return C_LIGHT_WHITE;

            default:
                return C_WHITE;
        }
    }*/

    /*string colorCode(const unsigned char fgc, const unsigned char bgc) {
		string s = "&";
		s += (char)(unsigned char)(fgc | (bgc << 4));
		//debugf("code: 0x%x, 0x%x, 0x%x", s[1], bgc, fgc);
		return s;
	}*/

    //colorCode proceded by &
    char cc(const unsigned char fgc, const unsigned char bgc){
        return (char)(fgc | (bgc << 4));
    }

    bool addChColor(char c, bool* lookingForCode) {
        if (*lookingForCode) {
            *lookingForCode = false;
            if (c == escapeColorCode) {
                addch('&');
                return true;
            } else {
                unsigned char uc = (unsigned char)c;
                setColor(uc & 0xF, (uc >> 4) & 0xF);
                //consolef("uc:0x%x, msd:0x%x, lsd:0x%x", uc, uc & 0xF, uc >> 4);
                return false;
            }
        } else if (c == '&') {
            *lookingForCode = true;
        } else {
            addch(c);
            return true;
        }
        return false;
    }

    int stringLengthWithColored(string s){
        bool lookingForCode = false;
        int l = 0;
        for (int i = 0; i < (int)s.length(); i++) {
            if(lookingForCode){
                lookingForCode = false;
                if(s[i] == escapeColorCode){
                    l++;
                }
            }else if(s[i] == '&'){
                lookingForCode = true;
            }else{
                l++;
            }
        }
        return l;
    }

    int printMultiLineColoredString(int y, int x, string s, int maxX) {
        int a = 0;
        int b = 0;
        if (maxX == -1) {
            maxX = terminalSize.x;
        }
        move(y, x);
        bool lookingForCode = false;
        for (int i = 0; i < (int)s.length(); i++) {
            if (!(s[i] == ' ' && b == 0)) {
                if (addChColor(s[i], &lookingForCode)) {
                    b++;
                }
            }
            if (b >= maxX - x) {
                b = 0;
                a++;
                move(y+a, x);
            }
        }
        setColor(C_WHITE);
        return a+(b==0?0:1);
    }

    int printEnchantments(int a, vector<Enchantment> enchantments, int columnX){
        if (enchantments.size() > 0) {
            a++;
            a += printMultiLineString(a, columnX, formatString("Enchantments:"));
            for (Enchantment e : enchantments) {
                setColor(effectColor(e.effect.eId, e.effect.meta));
                string s = "";
                switch (e.style) {
                    case eStyle_EnemyToSelf_EnemyEff:
                        s += "T->@ *T";
                        break;
                    case eStyle_EnemyToSelf_SelfEff:
                        s += "T->@ *@";
                        break;
                    case eStyle_onTick_SelfEff:
                        s += "tick *@";
                        break;
                    case eStyle_SelfToEnemy_EnemyEff:
                        s += "@->T *T";
                        break;
                    case eStyle_SelfToEnemy_SelfEff:
                        s += "@->T *@";
                        break;

                    default:
                        break;
                }
                s += ": ";
                s += effectName(e.effect);
                string ss = effectPowerString(e.effect.eId, e.effect.power);
                if(ss.size() > 0){
                    s += " "+ss;
                }
                if(e.chance != 1 || e.effect.timeLeft > 0){
                    s += " (";
                }
                if(e.chance != 1){
                    if(e.chance > 0){
                    	s += formatString("1/%d", e.chance);
                    }else{
                        s += formatString("!!!", e.chance);
                    }
                }
                if(e.chance != 1 && e.effect.timeLeft > 0){
                    s += ", ";
                }
                if(e.effect.timeLeft > 0){
                    s += formatString("%.2f%s", e.effect.timeLeft, SYMBOL_TIME);
                }
                if(e.chance != 1 || e.effect.timeLeft > 0){
                    s += ")";
                }
                a += printMultiLineString(a, columnX, s);

            }
            a++;
        }
        return a;
    }

    void drawInventory(World* currentWorld, EntityPlayer* player, int selectedY/*, int scrollOffset*/, Inventory* secondaryInv, string playerDisplayName, string invDisplayName, bool selectedLeft, bool showPrice, bool flashImportantInfo, int highlightIndex) {

        int columnX = 0;
        int columnWidth = (terminalSize.x) / (secondaryInv == nullptr ? 2 : 3);

        //ItemWeapon* activeItemWeapon = player->getActiveItemWeapon();


        move(0, 0);
        clrtobot();


        vector<Item*> inv = (selectedLeft ? player : secondaryInv)->inventory;
        if (inv.size() > 0) {
            Item* item = inv[selectedY];
            if (item) {
                setColor(C_LIGHT_GRAY, C_BLACK);
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
            int minI = max(0, midI - countI);
            int maxI = min(midI + countI, (int) inv.size());
            if (((ii == 0) != selectedLeft)) {
                setColor(C_LIGHT_GRAY);
            }
            mvprintw(0, columnX, (ii == 1 ? invDisplayName.c_str() : playerDisplayName.c_str()));

            int y = 2;

            for (int i = minI; i < maxI; i++) {
                Item* item = inv[i];

                setColor(((ii == 0) == selectedLeft) ? C_WHITE : C_LIGHT_GRAY);

                ItemEquipable* equipable = dynamic_cast<ItemEquipable*>(item);
                if(equipable){
                    /*if(equipable->durability == 0){
                        setColor(((ii == 0) == selectedLeft) ? C_LIGHT_RED : C_DARK_RED);
                    }*/
                }

                bool cantAfford = false;

                if(showPrice && ii == 1){
                    if(item->coinValue > player->getWallet()){
                        cantAfford = true;
                        setColor(C_DARK_RED, C_BLACK);
                    }
                }

                if (i == selectedY && (((ii == 0) == selectedLeft))) {
                    setColor(C_BLACK, cantAfford?C_DARK_RED:C_WHITE);
                }

                if(i == highlightIndex && selectedLeft && tick%10<5){
                    setColor(C_BLACK, cantAfford?C_LIGHT_RED:C_LIGHT_CYAN);
                }

                char pre;

                EquipSlot slot = player->getSlot(item);
                pre = ItemEquipable::equipSlotAbr(slot);

                ItemEquipable* equippable = dynamic_cast<ItemEquipable*>(item);
                
                if(!equippable && slot == slotNone){
                    pre = '-';
                }
                string displayName = formatString("%c ", pre);
                
                bool fav = false;
                for(int i=0;i<10;i++){
                    if(currentWorld->currentPlayer->getFav(i) == item){
                        if(!fav){
                            displayName += SYMBOL_HEART;
                        }
                        displayName += i==9?'0':i+'1';
                        fav = true;
                    }
                }
                if(fav){
                    displayName += " ";
                }

                if(showPrice){
                    if(item->coinValue > 0){
                        displayName += formatString("(%d%s) ", item->coinValue, SYMBOL_COIN);
                    }else if(item->coinValue == 0){
                        displayName += "(Free) ";
                    }else if(item->coinValue == -1){
                        displayName += "";
                    }
                }
                if(item->qty != 1){
                    displayName += formatString("%d ", item->qty);
                }

                displayName += formatString("%s", item->getName(item->qty != 1).c_str());

                int w = columnWidth - 4;

                if((int)displayName.size() > w+3){
                    displayName = displayName.substr(0, w)+"...";
                }

                //mvprintw(y, columnX, "%c ", pre);
                y += printMultiLineString(y, columnX, displayName, columnX + columnWidth - 1);

            }


        }
        setColor(C_WHITE, C_BLACK);

        columnX += columnWidth;

        if (inv.size() > 0) {
            Item* item = inv[selectedY];

            if(item) {
                ItemEquipable* equipable = dynamic_cast<ItemEquipable*>(item);
                ItemArmor* armor = dynamic_cast<ItemArmor*>(item);
                ItemWeapon* weapon = dynamic_cast<ItemWeapon*>(item);
                ItemRanged* ranged = dynamic_cast<ItemRanged*>(item);
                ItemCombatSpell* spell = dynamic_cast<ItemCombatSpell*>(item);
                ItemPotion* potion = dynamic_cast<ItemPotion*>(item);
                ItemUtilitySpell* utilitySpell = dynamic_cast<ItemUtilitySpell*>(item);
                ItemSpecial* itemSpecial = dynamic_cast<ItemSpecial*>(item);
                ItemExplosive* itemExplosive = dynamic_cast<ItemExplosive*>(item);
                ItemAreaOfEffectWeapon* aoe = dynamic_cast<ItemAreaOfEffectWeapon*>(item);

                int a = 2;

                a += printMultiLineString(a, columnX, formatString("%s %s", item->qty!=1?formatString("%d", item->qty).c_str():"", item->getName(item->qty != 1).c_str()));

                setColor(C_LIGHT_GRAY, C_BLACK);

                if(equipable){
                    /*if(equipable->durability == 0){
                        setColor(C_LIGHT_RED);
                        a += printMultiLineString(a, columnX, "ITEM BROKEN Effectiveness halved");
                    }
                    if(equipable->durability >= 0){
                    	a += printMultiLineString(a, columnX, formatString("Durability: %d", equipable->durability));
                    	setColor(C_LIGHT_GRAY, C_BLACK);
                    }*/
                }

                if(item->coinValue > 0){
                    a += printMultiLineString(a, columnX, formatString("Value: %d%s", item->coinValue, SYMBOL_COIN));
                }

                setColor(C_LIGHT_GRAY, C_BLACK);

                if(equipable){
                    //a+= printMultiLineString(a, columnX, "EQUIPABLE");

                    vector<string> eqv;
                    string eqs = "";
                    string eqp = "";

                    vector<EquipSlot> vs = equipable->getViableSlots();

                    for(EquipSlot slot : vs){
                        string c = ItemEquipable::equipSlotNameGeneric(slot);
                        if(!Utility::vectorContains(eqv, c)){
                            eqv.push_back(c);
                            if(eqs.length() > 0){
                                eqs += ", ";
                            }else{
                                eqp = ItemEquipable::equipSlotPreposition(slot);
                            }
                            eqs += ItemEquipable::equipSlotNameGeneric(slot);
                        }
                    }

                    setColor(C_LIGHT_MAGENTA, C_BLACK);

                    if(eqs.length() > 0){
                        a += printMultiLineString(a, columnX, formatString("Equipable %s %s", eqp.c_str(), eqs.c_str()));
                    }

                    setColor(C_LIGHT_GRAY, C_BLACK);

                    EquipSlot slot = player->getSlot(equipable);
                    if(slot != slotNone){
                        mvprintw(terminalSize.y - 1, columnX, "-Equipped %s %s", ItemEquipable::equipSlotPreposition(slot).c_str(), ItemEquipable::equipSlotName(slot).c_str());
                    }
                }

                //mvprintw(a++, columnX, "Qty: %d", item->qty);
                //a += printMultiLineString(a, columnX, formatString("Weight: %.2f", item->weight));
                if (potion) {
                    a += printMultiLineString(a, columnX, "Effects:");
                    if (potion->effects.size() == 0) {
                        a += printMultiLineString(a, columnX, "   None");
                    } else {
                        for (Effect e : potion->effects) {
                            if (e.timeLeft > 0) {
                                if(e.eId == effMultRecivedDamage || e.eId == effMultAttack){
                                    a += printMultiLineString(a, columnX, formatString("   %s %d%%, %.2f%s", effectName(e).c_str(), (int)(e.power*100), e.timeLeft, SYMBOL_TIME));
                                }else{
                                    if(e.power == 0){
                                        a += printMultiLineString(a, columnX, formatString("   %s, %.2f%s", effectName(e).c_str(), e.timeLeft, SYMBOL_TIME));
                                    }else{
                                        a += printMultiLineString(a, columnX, formatString("   %s %.2f, %.2f%s", effectName(e).c_str(), e.power, e.timeLeft, SYMBOL_TIME));
                                    }
                                }
                            } else {
                                if(e.power == 0){
                                    a += printMultiLineString(a, columnX, formatString("   %s", effectName(e).c_str()));
                                }else{
                                    a += printMultiLineString(a, columnX, formatString("   %s %.2f", effectName(e).c_str(), e.power));
                                }
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
                            a += printMultiLineString(a, columnX, "Transport to selected location or random location if used on yourself.");
                            break;

                        case spellBarrier:
                            a += printMultiLineString(a, columnX, "Place a 3x3 square of destrutable walls at a selected location.");
                            break;

                        case spellSwap:
                            a += printMultiLineString(a, columnX, "Swap places with a selected entity.");
                            break;

                        default:
                            break;
                    }
                    if (utilitySpell->manaCost == -1) {
                        setColor(C_LIGHT_RED);
                        a += printMultiLineString(a, columnX, "One Use");
                        setColor(C_LIGHT_GRAY, C_BLACK);
                    } else {

                        if(player->getMaxMp() < utilitySpell->manaCost || (player->getMp() < utilitySpell->manaCost && flashImportantInfo && (tick % 4) >= 2)){
                            setColor(C_BLACK, C_LIGHT_RED);
                        }else if(player->getMp() < utilitySpell->manaCost){
                            setColor(C_BLACK, C_LIGHT_YELLOW);
                        }
                        a += printMultiLineString(a, columnX, formatString("Mana Cost: %.2f", utilitySpell->manaCost));
                        setColor(C_LIGHT_GRAY, C_BLACK);
                    }
                } else if(armor){
                    for(Defense def : armor->defenses){
                        a += printMultiLineString(a, columnX, formatString("%d%c resistance to %s", (int)(def.amount*100.0), '%', damageTypeName(def.damageType).c_str()));
                    }

                    a = printEnchantments(a, armor->enchantments, columnX);

                } else if (weapon) {
                    double x = player->calcDamageMultiplier(weapon);
                    double delay = player->useDelay(weapon);
                    if(weapon->baseDamage != 0){
                        if (x == 1.0) {
                            a += printMultiLineString(a, columnX, formatString("Damage: %.2f", weapon->baseDamage));
                        } else {
                            a += printMultiLineString(a, columnX, formatString("Damage: %.2f (%.2f)", weapon->baseDamage, weapon->baseDamage * x));
                        }
                        a += printMultiLineString(a, columnX, formatString("Damage Type: %s", damageTypeName(weapon->damageType).c_str()));
                    }
                    if(delay == weapon->useDelay){
                    	a += printMultiLineString(a, columnX, formatString("Speed: %d%%", (int) (100 / weapon->useDelay)));
                    }else{
                        a += printMultiLineString(a, columnX, formatString("Speed: %d%% (%d%%)", (int) (100 / weapon->useDelay), (int) (100 / delay)));
                    }
                    if (ranged) {
                        a += printMultiLineString(a, columnX, formatString("Range: %.2f", ranged->range));
                        switch (ranged->rangedType) {
                            case rangedOneUse:
                                setColor(C_LIGHT_RED);
                                a += printMultiLineString(a, columnX, "One Use");
                                setColor(C_LIGHT_GRAY, C_BLACK);
                                break;
                            case rangedOneUseRecoverable:
                                setColor(C_LIGHT_YELLOW);
                                a += printMultiLineString(a, columnX, "One Use, Recoverable");
                                setColor(C_LIGHT_GRAY, C_BLACK);
                                break;
                                
                            default:
                                break;
                        }
                        if (spell) {
                            if(spell->manaCost > 0){
                                a += printMultiLineString(a, columnX, formatString("Mana Cost: %.2f", spell->manaCost));
                            }
                            if(aoe){
                                a += printMultiLineString(a, columnX, formatString("Radius: %d", aoe->radius));
                            }
                        }
                    }
                    //if (!spell) {
                    a = printEnchantments(a, weapon->enchantments, columnX);
                    //}
                    if(weapon->baseDamage != 0){
                        setColor(C_LIGHT_CYAN);
                        a += printMultiLineString(a, columnX, formatString("d/t: %.2f", ((weapon->baseDamage * x) / delay)));
                        if (spell) {
                            a += printMultiLineString(a, columnX, formatString("d/m: %.2f", ((spell->baseDamage * x) / (double) spell->manaCost)));
                        }
                    }
                    setColor(C_LIGHT_GRAY, C_BLACK);
                }else if(itemSpecial){
                    switch (itemSpecial->specialty) {
                        case specialtyCompass:{

							printMultiLineString(a++, columnX, "Points towards the staircase down.");

                            int frame = 0;

                            Point2 diff = player->pos - currentWorld->currentLevel->stairDownPos;

                            double angle = (atan2((double)diff.y, (double)-diff.x) * 180 / PI);

                            if(angle < 0){
                                angle = angle + 360;
                            }

                            string d = "";

                            if((angle >= 0 && angle < 22.5) || (angle >= 337.5 && angle <= 360)){ //E
                                frame = 1;
                            }else if(angle >= 22.5 && angle < 67.5){//NE
                                d = "NE";
                                frame = 2;
                            }else if(angle >= 67.5 && angle < 112.5){//N
                                d = "N";
                                frame = 3;
                            }else if(angle >= 112.5 && angle < 157.5){//NW
                                d = "NW";
                                frame = 4;
                            }else if(angle >= 157.5 && angle < 202.5){//E
                                d = "E";
                                frame = 5;
                            }else if(angle >= 202.5 && angle < 247.5){//SW
                                d = "SW";
                                frame = 6;
                            }else if(angle >= 247.5 && angle < 292.5){//S
                                d = "S";
                                frame = 7;
                            }else if(angle >= 292.5 && angle < 337.5){//SE
                                d = "SE";
                                frame = 8;
                            }else{
                                frame = (tick%8)+1;
                            }

                        	//debugf("Size:%d  Frame:%d  Angle:%f", Arts::compassInList.size(), frame, angle);


                            Arts::getArt(Arts::compassOut)->printAt(Point2(columnX, a));
							if ((int)Arts::compassInList.size() > frame) {
								Arts::getArt(Arts::compassInList[frame])->printAt(Point2(columnX, a) + Arts::compassInOffset);
                            }else{
                                a += printMultiLineString(a, columnX, d.c_str());
                            }
                            break;
                        }
                        case specialtyRepairer:{
                            printMultiLineString(a++, columnX, "Use on a weapon to increase durability.");
                            printMultiLineString(a++, columnX, formatString("+%d durability on use.", player->repairToolPower()));
                            break;
                        }
                        default:
                            break;
                    }
                } else if(itemExplosive){
                    switch (itemExplosive->explosiveType) {
                        case timeActivatedBomb:
                            a += printMultiLineString(a, columnX, formatString("Fuse: %.2f", itemExplosive->time));
                            //no break intentional
                        case pressureBomb:
                        case throwableBomb:
                            a += printMultiLineString(a, columnX, formatString("Damage: %.2f", itemExplosive->power));
                            a += printMultiLineString(a, columnX, formatString("Radius: %.2f", itemExplosive->radius));
                            break;
                        default:
                            break;
                    }
                    switch (itemExplosive->explosiveType) {
                        case pressureBomb:
                        case throwableBomb:
                        case timeActivatedBomb:
                            if(itemExplosive->destroysTiles){
                                setColor(C_LIGHT_RED);
                                a += printMultiLineString(a, columnX, "Will destroy tiles.");
                            }else{
                                a += printMultiLineString(a, columnX, "Will not destroy tiles.");
                            }
                            break;
                        default:
                            break;
                    }
                    setColor(C_WHITE);

                }

                if(equipable){
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
                        if(flashImportantInfo && (tick % 4) >= 2){
                            setColor(C_BLACK, C_LIGHT_RED);
                        }

                        printMultiLineString(b, columnX, "Requires:");
                        setColor(C_LIGHT_GRAY);
                        
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

        const int hp = roundToInt(player->getHp());
        const int maxHp = roundToInt(player->getMaxHp());
        const int mp = roundToInt(player->getMp());
        const int maxMp = roundToInt(player->getMaxMp());

        move(0, terminalSize.x - 15);
        clrtoeol();

        int mpStrL = 1;
        if(maxMp > 0 && !player->specials[specialPoolHpMp]){
            setColor(C_LIGHT_BLUE);
            string mpStr = formatString("%d/%d", mp, maxMp);
            mpStrL += (int)mpStr.size();
            mvprintw(0, terminalSize.x - mpStrL, "%s", mpStr.c_str());
        }
        mpStrL++;
        setColor(C_LIGHT_GREEN);
        string hpStr = formatString("%d/%d", hp, maxHp);
        mvprintw(0, terminalSize.x - mpStrL - (int)hpStr.size(), "%s", hpStr.c_str());



        /*
         setColor(C_WHITE);
         int minI = max(0, scrollOffset);
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
         ItemWeapon* weapon;
         ItemRanged* ranged;
         ItemCombatSpell* spell;
         int y = 3;
         for(int i=minI;i<maxI;i++){
         item = inv->inventory[i];
         weapon = dynamic_cast<ItemWeapon*>(item);
         ranged = dynamic_cast<ItemRanged*>(item);
         spell = dynamic_cast<ItemCombatSpell*>(item);
         if(i == selected){
         setColor(C_BLACK, C_WHITE);
         }
         char pre = activeItemWeapon!=nullptr?(item == activeItemWeapon?'E':' '):' ';
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
         weapon = dynamic_cast<ItemWeapon*>(item);
         ranged = dynamic_cast<ItemRanged*>(item);
         spell = dynamic_cast<ItemCombatSpell*>(item);

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
         if(item == activeItemWeapon){
         mvprintw(terminalSize.y-1, columnInfo, "-Equipped");
         }else{
         mvprintw(terminalSize.y-1, columnInfo, "");
         }

         setColor(C_LIGHT_GRAY, C_BLACK);

         mvprintw(a++, columnInfo, "Qty: %d", item->qty);
         mvprintw(a++, columnInfo, "Weight: %.2f", item->weight);
         if(weapon){
         mvprintw(a++, columnInfo, "Damage: %.2f", weapon->baseDamage);
         mvprintw(a++, columnInfo, "Damage Type: %s", ItemWeapon::damageTypeName(weapon->damageType).c_str());
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
         mvprintw(a++, columnInfo, "   %s x%d (1/%d for %.2f)", ItemWeapon::enchantmentName(e).c_str(), e.power, e.chance, e.time);
         }
         a++;
         }
         }

         }
         }*/
    }
}
