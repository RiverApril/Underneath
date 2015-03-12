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
    void drawInventory(Inventory* inv, int selected, int scrollOffset, string display, Weapon* activeWeapon){
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

        mvprintw(1, columnName, "Item");mvvline(1, 0, '|', terminalSize.y);
        mvprintw(1, columnQty-3, "Qty");//mvvline(1, columnQty-1, '|', terminalSize.y);
        //mvprintw(1, columnWeight, "Wgt");mvvline(1, columnWeight-1, '|', terminalSize.y);
        //mvprintw(1, columnHitPoints, "Hp");mvvline(1, columnHitPoints-1, '|', terminalSize.y);
        mvprintw(1, columnInfo, "Information");mvvline(1, columnInfo-1, '|', terminalSize.y);

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
            mvprintw(y, columnName, "%s%s", activeWeapon!=nullptr?(item == activeWeapon?"E ":"  "):"", item->name.c_str());
            mvprintw(y, columnQty-4, "%4s", (to_string(item->qty)).c_str());
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
        }
    }
}

