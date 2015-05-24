//
//  MenuInv.cpp
//  Underneath
//
//  Created by Braeden Atlee on 11/12/14.
//  Copyright (c) 2014 Braeden Atlee. All rights reserved.
//

#include "MenuInv.hpp"
#include "Math.hpp"
#include "ItemEntity.hpp"
#include "Controls.hpp"
#include "Utility.hpp"
#include "Art.hpp"

namespace Ui {

    MenuInv::MenuInv(Player* player, World* w, int* useItem) : Menu() {
        this->player = player;
        this->currentWorld = w;
        this->useItem = useItem;
        *useItem = -1;
    }

    void MenuInv::handleInput(int in) {
        if (in == Key::uiUp) {
            selected--;
        } else if (in == Key::uiDown) {
            selected++;
        } else if (in == Key::drop) {
            if (player->inventory.size() > 0 && selected < player->inventory.size()) {
                Item* drop;
                if (player->inventory[selected]->qty == 1) {
                    drop = player->inventory[selected];
                    player->removeItem(player->inventory[selected], false);
                } else {
                    player->inventory[selected]->qty -= 1;
                    drop = Item::clone(player->inventory[selected]);
                    drop->qty = 1;
                }
                currentWorld->currentLevel->newEntity(new ItemEntity(drop, player->pos));
            }
        } else if (in == Key::dropAll) {
            if (player->inventory.size() > 0 && selected < player->inventory.size()) {
                Item* drop;
                drop = player->inventory[selected];
                player->removeItem(player->inventory[selected], false);
                currentWorld->currentLevel->newEntity(new ItemEntity(drop, player->pos));
            }
        } else if (in == Key::equip) {
            Equipable* equipable = dynamic_cast<Equipable*> (player->inventory[selected]);
            if (equipable) {
                EquipSlot slot = player->getSlot(equipable);
                if(slot){
                    player->equipItem(nullptr, slot);
                }else{
                    player->equipItem(equipable);
                }
            }else{
                *useItem = selected;
                closeThisMenu();
            }
        } else if (in == Key::interact) {
            *useItem = selected;
            closeThisMenu();
            return;
        } else if (in == KEY_ESCAPE || in == Key::inventory) {
            closeThisMenu();
            return;
        }
    }

    void MenuInv::update() {
        if (selected < 0) {
            selected = (int) player->inventory.size() - 1;
        }
        if (selected >= player->inventory.size()) {
            selected = 0;
        }

		Ui::drawInventory(currentWorld, player, selected/*, scrollOffset*/);


        /*setColor(C_WHITE);
        int minI = Math::max(0, scrollOffset);
        int maxI = player->inventory.size() - scrollOffset;
        move(0, 0);
        clrtobot();
        int totalWeight = 0;
        forVector(player->inventory, i){
            totalWeight += player->inventory[i]->weight * player->inventory[i]->qty;
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
        CombatSpell* spell;
        int y = 3;
        for(int i=minI;i<maxI;i++){
            item = player->inventory[i];
            weapon = dynamic_cast<Weapon*>(item);
            ranged = dynamic_cast<Ranged*>(item);
            spell = dynamic_cast<CombatSpell*>(item);
            if(i == selected){
                setColor(C_BLACK, C_WHITE);
            }
            mvprintw(y, columnPrefixChar, "%c", item == player->getActiveWeapon()?'E':' ');
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

        item = player->inventory[selected];

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

            mvprintw(a++, columnInfo, "Name: %s", item->getExtendedName().c_str());
            if(item == player->getActiveWeapon()){
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
