//
//  ItemGenerator.h
//  Underneath
//
//  Created by Braeden Atlee on 1/25/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__ItemGenerator__
#define __Underneath__ItemGenerator__


#include "CombatSpell.hpp"
#include "Potion.hpp"
#include "UtilitySpell.hpp"
#include "Armor.hpp"


namespace ItemGenerator {

    struct Condition {

        Condition() {
        }

        Condition(vector<string> names, double damMin, double damMax, double udMin, double udMax, double wMin, double wMax, vector<WeaponType> weaponTypes) {
            this->names = names;
            this->damage.x = damMin;
            this->damage.y = damMax;
            this->useDelay.x = udMin;
            this->useDelay.y = udMax;
            this->weight.x = wMin;
            this->weight.y = wMax;
            this->applicableWeaponTypes = weaponTypes;
        }

        Condition magical(double minMana, double maxMana) {
            manaCost.x = minMana;
            manaCost.y = maxMana;
            return *this;
        }

        vector<string> names = {""};
        Vector2 damage = Vector2One;
        Vector2 useDelay = Vector2One;
        Vector2 weight = Vector2One;
        Vector2 manaCost = Vector2One;
        vector<WeaponType> applicableWeaponTypes;
    };

    struct DefenseRange{
        DefenseRange(DamageType damageType, double amountMin, double amountMax, int chance = 1){
            this->damageType = damageType;
            this->amountMin = amountMin;
            this->amountMax = amountMax;
            this->chance = chance;
        }
        DamageType damageType;
        double amountMin;
        double amountMax;
        int chance;
    };

    struct ArmorBase{
        ArmorBase(){
        }

        ArmorBase(vector<string> names, vector<EquipSlot> viableSlots, vector<DefenseRange> defences, double weight){
            this->names = names;
            this->viableSlots = viableSlots;
            this->defences = defences;
            this->weight = weight;
        }
        vector<string> names;
        vector<EquipSlot> viableSlots;
        vector<DefenseRange> defences;
        double weight;
    };

    struct WeaponBase {

        WeaponBase() {
        }

        WeaponBase(vector<string> names, double damage, double useDelay, DamageType damageType, WeaponType weaponType) {
            this->names = names;
            this->damage = damage;
            this->useDelay = useDelay;
            this->damageType = damageType;
            this->weaponType = weaponType;
        }

        WeaponBase ranged(double range) {
            this->range = range;
            return *this;
        }

        WeaponBase magical(double range, double manaCost) {
            this->range = range;
            this->manaCost = manaCost;
            return *this;
        }

        WeaponBase setWeight(double weight) {
            this->weight = weight;
            return *this;
        }

        WeaponBase setArts(vector<int> artIndecies) {
            this->arts = artIndecies;
            return *this;
        }

        double damage = 0;
        double useDelay = 0;
        double weight = 1;
        vector<string> names = {""};
        vector<int> arts = {-1};
        DamageType damageType = damSharp;
        WeaponType weaponType = wepMelee;
        double range = -1;
        double manaCost = -1;
        vector<Enchantment> enchs;
    };

    struct PotionBase {

        PotionBase() {
        }

        PotionBase(vector<string> names, vector<EffectId> effIds, double timeMin, double timeMax, double powerMin, double powerMax, double meta) {
            this->names = names;
            this->effIds = effIds;
            this->time = Vector2(timeMin, timeMax);
            this->power = Vector2(powerMin, powerMax);
            this->meta = meta;
        }

        vector<string> names = {""};
        vector<EffectId> effIds = {0};
        Vector2 time = Vector2One;
        Vector2 power = Vector2One;
        double meta = 0;

    };

    struct ScrollBase {

        ScrollBase() {
        }

        ScrollBase(vector<string> names, SpellEffect eff) {
            this->names = names;
            this->eff = eff;
        }

        vector<string> names = {""};
        SpellEffect eff = 0;
    };

    ArmorBase atl(ArmorBase a);
    Condition atl(Condition c);
    WeaponBase atl(WeaponBase w);
    PotionBase atl(PotionBase p);
    ScrollBase atl(ScrollBase s);


    extern vector<ArmorBase> armorList;
    extern vector<Condition> conditionList;
    extern vector<WeaponBase> weaponList;
    extern vector<PotionBase> potionList;
    extern vector<ScrollBase> scrollList;

    extern Item* iCoin;

    extern WeaponBase wKnife;
    extern WeaponBase wSword;
    extern WeaponBase wAxe;
    extern WeaponBase wMace;
    extern WeaponBase wSpear;

    extern WeaponBase wBow;
    extern WeaponBase wCrossbow;

    extern WeaponBase wFireCombatSpell;
    extern WeaponBase wFrostCombatSpell;
    extern WeaponBase wShockCombatSpell;

    extern WeaponBase wNatural;


    vector<Item*> createRandLoots(int difficulty, int goldMaxQty, int wepMaxQty, int armMaxQty, int altMaxQty);

    Item* createRandAltLoot(int itemDifficulty);

    Weapon* createRandWeapon(int itemDifficulty);

    Armor* createRandArmor(int itemDifficulty);

    WeaponBase getRandWeaponBase(DamageType d);
    WeaponBase getRandWeaponBase();
    ArmorBase getRandArmorBase();

    Weapon* createWeaponFromBase(WeaponBase base, int itemDifficulty);

    Armor* createArmorFromBase(ArmorBase base, int itemDifficulty);

    Weapon* createWeaponFromType(WeaponType w, int itemDifficulty);

    Weapon* applyConditionToWeapon(Weapon* w, Condition c, int itemDifficulty, bool prependName = true);

    Weapon* applyRandConditionToWeapon(Weapon * w, int itemDifficulty, bool prependName = true);

    Weapon* createRandWeapon(int itemDifficulty);

    Potion* createPotionFromBase(PotionBase pb, int itemDifficulty);

    UtilitySpell* createScrollFromBase(ScrollBase sb);

    Item* makeCoins(int qty);


    void initItemTemplates();
    void cleanupItemTemplates();


}


#endif /* defined(__Underneath__ItemGenerator__) */
