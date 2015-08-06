//
//  ItemGenerator.h
//  Underneath
//
//  Created by Braeden Atlee on 1/25/15.
//  Copyright (c) 2015 Braeden Atlee. All rights reserved.
//

#ifndef __Underneath__ItemGenerator__
#define __Underneath__ItemGenerator__


#include "ItemCombatSpell.hpp"
#include "ItemPotion.hpp"
#include "ItemUtilitySpell.hpp"
#include "ItemArmor.hpp"


namespace ItemGenerator {

    struct Condition {

        Condition() {
        }

        Condition(vector<string> names, double damMin, double damMax, double udMin, double udMax, double wMin, double wMax, vector<ItemWeaponType> weaponTypes) {
            this->names = names;
            this->damage.x = damMin;
            this->damage.y = damMax;
            this->useDelay.x = udMin;
            this->useDelay.y = udMax;
            this->weight.x = wMin;
            this->weight.y = wMax;
            this->applicableItemWeaponTypes = weaponTypes;
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
        vector<ItemWeaponType> applicableItemWeaponTypes;
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

    struct ItemArmorBase{
        ItemArmorBase(){
        }

        ItemArmorBase(vector<string> names, vector<EquipSlot> viableSlots, vector<DefenseRange> defences, double weight){
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

    struct ItemWeaponBase {

        ItemWeaponBase() {
        }

        ItemWeaponBase(vector<string> names, double damage, double useDelay, DamageType damageType, ItemWeaponType weaponType) {
            this->names = names;
            this->damage = damage;
            this->useDelay = useDelay;
            this->damageType = damageType;
            this->weaponType = weaponType;
        }

        ItemWeaponBase ranged(double range) {
            this->range = range;
            return *this;
        }

        ItemWeaponBase magical(double range, double manaCost) {
            this->range = range;
            this->manaCost = manaCost;
            return *this;
        }

        ItemWeaponBase setWeight(double weight) {
            this->weight = weight;
            return *this;
        }

        ItemWeaponBase setArts(vector<int> artIndecies) {
            this->arts = artIndecies;
            return *this;
        }

        double damage = 0;
        double useDelay = 0;
        double weight = 1;
        vector<string> names = {""};
        vector<int> arts = {-1};
        DamageType damageType = damSharp;
        ItemWeaponType weaponType = wepMelee;
        double range = -1;
        double manaCost = -1;
        vector<Enchantment> enchs;
    };

    struct EffIdMeta {
        EffIdMeta(EffectId id, double meta){
            this->id = id;
            this->meta = meta;
        }
        EffectId id;
        double meta;
    };

    struct PotionBase {

        PotionBase() {
        }

        PotionBase(vector<string> names, vector<EffIdMeta> effects, double timeMin, double timeMax, double powerMin, double powerMax, bool difficultyScales = true) {
            this->names = names;
            this->effects = effects;
            this->time = Vector2(timeMin, timeMax);
            this->power = Vector2(powerMin, powerMax);
            this->difficultyScales = difficultyScales;
        }

        vector<string> names = {""};
        vector<EffIdMeta> effects = {};
        Vector2 time = Vector2One;
        Vector2 power = Vector2One;
        bool difficultyScales;

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

    ItemArmorBase atl(ItemArmorBase a);
    Condition atl(Condition c);
    ItemWeaponBase atl(ItemWeaponBase w);
    PotionBase atl(PotionBase p);
    ScrollBase atl(ScrollBase s);


    extern vector<ItemArmorBase> armorList;
    extern vector<Condition> conditionList;
    extern vector<ItemWeaponBase> weaponList;
    extern vector<PotionBase> potionList;
    extern vector<ScrollBase> scrollList;

    extern Item* iCoin;
    extern Item* iSmallKey;

    extern ItemWeaponBase wKnife;
    extern ItemWeaponBase wSword;
    extern ItemWeaponBase wAxe;
    extern ItemWeaponBase wMace;
    extern ItemWeaponBase wSpear;

    extern ItemWeaponBase wBow;
    extern ItemWeaponBase wCrossbow;

    extern ItemWeaponBase wFireItemCombatSpell;
    extern ItemWeaponBase wFrostItemCombatSpell;
    extern ItemWeaponBase wShockItemCombatSpell;

    extern ItemWeaponBase wNatural;


    vector<Item*> createRandLoots(int difficulty, int goldMaxQty, int wepMaxQty, int armMaxQty, int altMaxQty, int keyMaxQty);

    Item* createRandAltLoot(int itemDifficulty);

    ItemWeapon* createRandItemWeapon(int itemDifficulty);

    ItemArmor* createRandItemArmor(int itemDifficulty);

    ItemWeaponBase getRandItemWeaponBase(DamageType d);
    ItemWeaponBase getRandItemWeaponBase();
    ItemArmorBase getRandItemArmorBase();

    ItemWeapon* createItemWeaponFromBase(ItemWeaponBase base, int itemDifficulty);

    ItemArmor* createItemArmorFromBase(ItemArmorBase base, int itemDifficulty);

    ItemWeapon* createItemWeaponFromType(ItemWeaponType w, int itemDifficulty);

    ItemWeapon* applyConditionToItemWeapon(ItemWeapon* w, Condition c, int itemDifficulty, bool prependName = true);

    ItemWeapon* applyRandConditionToItemWeapon(ItemWeapon * w, int itemDifficulty, bool prependName = true);

    ItemWeapon* createRandItemWeapon(int itemDifficulty);

    ItemPotion* createPotionFromBase(PotionBase pb, int itemDifficulty);

    ItemUtilitySpell* createScrollFromBase(ScrollBase sb);

    Item* makeCoins(int qty);

    int calculateItemValue(Item* item);


    void initItemTemplates();
    void cleanupItemTemplates();

}


#endif /* defined(__Underneath__ItemGenerator__) */
