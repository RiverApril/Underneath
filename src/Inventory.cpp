//
//  Inventory.cpp
//  Underneath
//
//  Created by Braeden Atlee on 2/26/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#include "Inventory.h"
#include "Ui.h"
#include "CombatSpell.h"
#include "Math.h"


namespace Ui{
    void drawInventory(Inventory* inventory, string invDisplayName, int selectedY, int scrollOffset, Weapon* activeWeapon, Inventory* playerInv, bool selectedLeft){

        int columnX = 0;
        int columnWidth = (terminalSize.x-30)/(playerInv==nullptr?1:2);

        move(0, 0);
        clrtobot();

        setColor(C_WHITE);
        mvhline(1, 0, '-', terminalSize.x);

        for(int ii = 0; ii < (playerInv==nullptr?1:2); ii++){
            vector<Item*> inv = (ii==0?inventory:playerInv)->inventory;

            setColor(C_WHITE);

            columnX = (ii * columnWidth);
            mvaddch(1, columnX+columnWidth-1, '+');
            mvvline(2, columnX+columnWidth-1, '|', terminalSize.y-2);

            int minI = Math::max(0, scrollOffset);
            int maxI = Math::min(minI+terminalSize.y-3, (int)inv.size());
            if(((ii==0)!=selectedLeft)){
                setColor(C_LIGHT_GRAY);
            }
            mvprintw(0, columnX, (ii==0?invDisplayName.c_str():"Player"));
			
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
                if(item->qty > 1){
                    displayName = formatString("%c  %d x %s", pre, item->qty, item->name.c_str());
                }else{
                    displayName = formatString("%c  %s", pre, item->name.c_str());
                }
                mvprintw(y, columnX, "%*-s", columnWidth-1, displayName.c_str());

            }


        }

        columnX += columnWidth;

        vector<Item*> inv = (selectedLeft?inventory:playerInv)->inventory;

        if(inv.size() > 0){
            Item* item = inv[selectedY];

            if(item){
                Weapon* weapon = dynamic_cast<Weapon*>(item);
                Ranged* ranged = dynamic_cast<Ranged*>(item);
                CombatSpell* spell = dynamic_cast<CombatSpell*>(item);

                int a = 2;

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

                mvprintw(a++, columnX, "Name: %s%s", item->name.c_str(), item->qty==1?"":(formatString(" x %d", item->qty).c_str()));
                if(item == activeWeapon){
                    mvprintw(terminalSize.y-1, columnX, "-Equipped");
                }else{
                    mvprintw(terminalSize.y-1, columnX, "");
                }

                setColor(C_LIGHT_GRAY, C_BLACK);

                //mvprintw(a++, columnX, "Qty: %d", item->qty);
                mvprintw(a++, columnX, "Weight: %.2f", item->weight);
                if(weapon){
                    mvprintw(a++, columnX, "Damage: %.2f", weapon->baseDamage);
                    mvprintw(a++, columnX, "Damage Type: %s", Weapon::damageTypeName(weapon->damageType).c_str());
                    mvprintw(a++, columnX, "Use Delay: %.2f", weapon->useDelay);
                    if(ranged){
                        mvprintw(a++, columnX, "Range: %.2f", ranged->range);
                        if(spell){
                            mvprintw(a++, columnX, "Mana Cost: %d", spell->manaCost);
                        }
                    }
                    if(!spell && weapon->enchantments.size() > 0){
                        a++;
                        mvprintw(a++, columnX, "Enchantments: ");
                        for(Enchantment e : weapon->enchantments){
                            mvprintw(a++, columnX, "   %s x%d (1/%d for %.2f)", Weapon::enchantmentName(e).c_str(), e.power, e.chance, e.time);
                        }
                        a++;
                    }

                    mvprintw(a++, columnX, "d/t: %.2f", (weapon->baseDamage/weapon->useDelay));
                    if(spell){
                        mvprintw(a++, columnX, "d/m: %.2f", (spell->baseDamage/(double)spell->manaCost));
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

