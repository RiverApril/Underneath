//
//  MenuInv.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/12/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuInv.h"
#include "Math.h"
#include "ItemEntity.h"
#include "Controls.h"
#include "Utility.h"
#include "Art.h"

namespace Ui {

    MenuInv::MenuInv(Alive* alive, World* w, int* useItem) : Menu(){
        this->alive = alive;
        this->currentWorld = w;
        this->useItem = useItem;
        *useItem = -1;
    }

    void MenuInv::handleInput(int in){
        switch (in) {
            case KEY_UP:
                selected--;
                break;

            case KEY_DOWN:
                selected++;
                break;

			case Key::drop:{
                if(alive->inventory.size() > 0 && selected < alive->inventory.size()){
                    Item* drop;
                    if(alive->inventory[selected]->qty == 1){
                        drop = alive->inventory[selected];
                    	alive->removeItem(alive->inventory[selected], false);
                    }else{
                        alive->inventory[selected]->qty -= 1;
                        drop = Item::clone(alive->inventory[selected]);
                        drop->qty = 1;
                    }
                    currentWorld->currentLevel->newEntity(new ItemEntity(drop, alive->pos));
                }
                break;
            }

			case Key::dropAll:{
                if(alive->inventory.size() > 0 && selected < alive->inventory.size()){
                    Item* drop;
                    drop = alive->inventory[selected];
                    alive->removeItem(alive->inventory[selected], false);
                    currentWorld->currentLevel->newEntity(new ItemEntity(drop, alive->pos));
                }
                break;
            }

			case Key::equip:{
                Weapon* weapon = dynamic_cast<Weapon*>(alive->inventory[selected]);
                if(weapon){
                    if(alive->getActiveWeapon() == weapon){
                        alive->setActiveWeapon(nullptr);
                    }else{
                        alive->setActiveWeapon(weapon);
                    }
                }
                break;
            }

			case Key::use:{
                *useItem = selected;
                closeThisMenu();
				return;
            }

            case KEY_ESCAPE:
            case Key::inventory:
                closeThisMenu();
                return;
        }
        if(selected<0){
            selected = 0;
        }
		if (selected >= alive->inventory.size()){
			selected = (int)alive->inventory.size() - 1;
		}
    }


    void MenuInv::update() {

		Ui::drawInventory(alive, selected, scrollOffset, "Inventory", alive->getActiveWeapon());


        /*setColor(C_WHITE);
        int minI = Math::max(0, scrollOffset);
        int maxI = alive->inventory.size() - scrollOffset;
        move(0, 0);
        clrtobot();
        int totalWeight = 0;
        forVector(alive->inventory, i){
            totalWeight += alive->inventory[i]->weight * alive->inventory[i]->qty;
        }
        mvprintw(0, 0, "Inventory   Total Weight: %-3d", totalWeight);

        setColor(C_BLACK, C_WHITE);
		mvhline(selected+3, 0, ' ', terminalSize.x);
        setColor(C_WHITE, C_BLACK);

        mvprintw(1, columnPrefixChar, "");mvvline(1, columnPrefixChar-1, '|', terminalSize.y);
        mvprintw(1, columnName, "Item");mvvline(1, columnName-1, '|', terminalSize.y);
        mvprintw(1, columnQty-3, "Qty");//mvvline(1, columnQty-1, '|', terminalSize.y);
        //mvprintw(1, columnWeight, "Wgt");mvvline(1, columnWeight-1, '|', terminalSize.y);
        //mvprintw(1, columnHitPoints, "Hp");mvvline(1, columnHitPoints-1, '|', terminalSize.y);
        mvprintw(1, columnInfo, "Information");mvvline(1, columnInfo-1, '|', terminalSize.y);

        mvhline(2, 0, '-', terminalSize.x);
        Item* item;
        Weapon* weapon;
        Ranged* ranged;
        Spell* spell;
        int y = 3;
        for(int i=minI;i<maxI;i++){
            item = alive->inventory[i];
            weapon = dynamic_cast<Weapon*>(item);
            ranged = dynamic_cast<Ranged*>(item);
            spell = dynamic_cast<Spell*>(item);
            if(i == selected){
                setColor(C_BLACK, C_WHITE);
            }
            mvprintw(y, columnPrefixChar, "%c", item == alive->getActiveWeapon()?'E':' ');
            mvprintw(y, columnName, item->getExtendedName().c_str());
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

        item = alive->inventory[selected];

        if(item){
            weapon = dynamic_cast<Weapon*>(item);
            ranged = dynamic_cast<Ranged*>(item);
            spell = dynamic_cast<Spell*>(item);

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

            mvprintw(a++, columnInfo, "Name: %s", item->getExtendedName().c_str());
            if(item == alive->getActiveWeapon()){
                mvprintw(terminalSize.y-1, columnInfo, "-Equipped");
            }else{
                mvprintw(terminalSize.y-1, columnInfo, "");
            }

            setColor(C_LIGHT_GRAY, C_BLACK);

            mvprintw(a++, columnInfo, "Qty: %d", item->qty);
            mvprintw(a++, columnInfo, "Weight: %.2f", item->weight);
            if(weapon){
                mvprintw(a++, columnInfo, "Damage: %.2f", weapon->baseDamage);
                mvprintw(a++, columnInfo, "Type: %s", Weapon::damageTypeToString(weapon->damageType).c_str());
                mvprintw(a++, columnInfo, "Use Delay: %.2f (d/t: %.2f)", weapon->useDelay, (weapon->baseDamage/weapon->useDelay));
                if(ranged){
                    mvprintw(a++, columnInfo, "Range: %.2f", ranged->range);
                    if(spell){
                        mvprintw(a++, columnInfo, "Mana Cost: %d", spell->manaCost);
                    }
                }
                if(weapon->enchantments.size() > 0){
                    a++;
                	mvprintw(a++, columnInfo, "Enchantments: ");
                    for(Enchantment e : weapon->enchantments){
                        mvprintw(a++, columnInfo, "   %s x%d (%d)", Weapon::enchantmentIdToString(e.eId).c_str(), e.power, e.chance);
                    }
                    a++;
                }
            }

        }*/

    }
}

