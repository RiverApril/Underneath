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

    struct ItemBase{

        ItemBase(){}
        virtual ~ItemBase(){};
    };

    struct ExactItemBase : ItemBase{

        ExactItemBase(Item* i){
            this->i = i;
        }

        Item* createCopy(){
            return Item::clone(i);
        }

        Item* original(){
            return i;
        }
        
    private:
        Item* i;
    };

    struct EnchantmentBase {

        EnchantmentBase() {
        }

        EnchantmentBase(EffectId effect, int chanceMin, int chanceMax, double powerMin, double powerMax, double timeMin, double timneMax, double meta = 0) {
            this->effect = effect;
            this->chance.x = chanceMin;
            this->chance.y = chanceMax;
            this->power.x = powerMin;
            this->power.y = powerMax;
            this->time.x = timeMin;
            this->time.y = timneMax;
            this->meta = meta;
        }
        
        EffectId effect;
        Point2 chance;
        Vector2 power;
        Vector2 time;
        double meta;
    };

    struct Condition {

        Condition() {
        }

        Condition(vector<string> names, double damMin, double damMax, double udMin, double udMax, vector<WeaponType> weaponTypes) {
            this->names = names;
            this->damage.x = damMin;
            this->damage.y = damMax;
            this->useDelay.x = udMin;
            this->useDelay.y = udMax;
            this->applicableWeaponTypes = weaponTypes;
        }

        Condition* magical(double minMana, double maxMana) {
            manaCost.x = minMana;
            manaCost.y = maxMana;
            return this;
        }

        vector<string> names = {""};
        Vector2 damage = Vector2One;
        Vector2 useDelay = Vector2One;
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

    struct ArmorBase : ItemBase{
        ArmorBase(){
        }

        ArmorBase(vector<vector<string> > names, vector<EquipSlot> viableSlots, vector<DefenseRange> defences){
            this->names = names;
            this->viableSlots = viableSlots;
            this->defences = defences;
        }
        vector<vector<string>> names;
        vector<EquipSlot> viableSlots;
        vector<DefenseRange> defences;
        vector<EnchantmentBase*> enchs;
        vector<int> arts = {-1};

        ArmorBase* setArts(vector<int> artIndecies) {
            this->arts = artIndecies;
            return this;
        }
    };

    struct WeaponBase : ItemBase{

        WeaponBase() {
        }

        WeaponBase(WeaponBase* other) {
            this->names = other->names;
            this->damage = other->damage;
            this->useDelay = other->useDelay;
            this->damageType = other->damageType;
            this->weaponType = other->weaponType;
            this->arts = other->arts;
            this->range = other->range;
            this->manaCost = other->manaCost;
            this->enchs = other->enchs;
        }

        WeaponBase(vector<vector<string>> names, double damage, double useDelay, DamageType damageType, WeaponType weaponType) {
            this->names = names;
            this->damage = damage;
            this->useDelay = useDelay;
            this->damageType = damageType;
            this->weaponType = weaponType;
        }

        WeaponBase* ranged(double range) {
            this->range = range;
            return this;
        }

        WeaponBase* magical(double range, double manaCost) {
            this->range = range;
            this->manaCost = manaCost;
            return this;
        }

        WeaponBase* setArts(vector<int> artIndecies) {
            this->arts = artIndecies;
            return this;
        }

        double damage = 0;
        double useDelay = 0;
        vector<vector<string>> names = {{""}};
        vector<int> arts = {-1};
        DamageType damageType = damSharp;
        WeaponType weaponType = wepMelee;
        double range = -1;
        double manaCost = -1;
        vector<EnchantmentBase*> enchs;
    };

    struct EffIdMeta {
        EffIdMeta(EffectId id, double meta){
            this->id = id;
            this->meta = meta;
        }
        EffectId id;
        double meta;
    };

    struct PotionBase : ItemBase{

        PotionBase() {
        }

        PotionBase(vector<vector<string> > names, vector<EffIdMeta> effects, double timeMin, double timeMax, double powerMin, double powerMax, bool difficultyScales = true) {
            this->names = names;
            this->effects = effects;
            this->time = Vector2(timeMin, timeMax);
            this->power = Vector2(powerMin, powerMax);
            this->difficultyScales = difficultyScales;
        }

        vector<vector<string> > names = {{""}};
        vector<EffIdMeta> effects = {};
        Vector2 time = Vector2One;
        Vector2 power = Vector2One;
        bool difficultyScales;

    };

    struct ScrollBase : ItemBase{

        ScrollBase() {
        }

        ScrollBase(vector<vector<string> > names, SpellEffect eff) {
            this->names = names;
            this->eff = eff;
        }

        vector<vector<string> >  names = {{""}};
        SpellEffect eff = 0;
    };

    ArmorBase* atl(ArmorBase* a);
    Condition* atl(Condition* c);
    WeaponBase* atl(WeaponBase* w);
    PotionBase* atl(PotionBase* p, int of100);
    ScrollBase* atl(ScrollBase* s);


    extern vector<ArmorBase*> armorList;
    extern vector<Condition*> conditionList;
    extern vector<WeaponBase*> weaponList;
    extern vector<ScrollBase*> scrollList;
    extern vector<PotionBase*> potionList;
    extern vector<int> potionChanceList;

    extern ExactItemBase* coin;
    extern ExactItemBase* smallKey;
    extern ExactItemBase* repairHammer;

    extern PotionBase* potionHealth;
    extern PotionBase* potionMana;
    extern PotionBase* potionRegen;
    extern PotionBase* potionManaRegen;
    extern PotionBase* potionPhysicalDefense;
    extern PotionBase* potionElementalDefense;
    extern PotionBase* potionPhysicalAttack;
    extern PotionBase* potionElementalAttack;

    extern PotionBase* potionRemoveBad;

    extern ScrollBase* scrollRemoteUse;
    extern ScrollBase* scrollRelocate;
    extern ScrollBase* scrollBarrier;

    extern WeaponBase* wKnife;
    extern WeaponBase* wSword;
    extern WeaponBase* wAxe;
    extern WeaponBase* wMace;
    extern WeaponBase* wSpear;

    extern WeaponBase* wBow;
    extern WeaponBase* wCrossbow;

    extern WeaponBase* wFireItemCombatSpell;
    extern WeaponBase* wFrostItemCombatSpell;
    extern WeaponBase* wShockItemCombatSpell;

    extern WeaponBase* wHealingCombatSpell;

    extern WeaponBase* wNatural;

    extern ArmorBase* aLeatherChest;
    extern ArmorBase* aLeatherHelm;
    extern ArmorBase* aLeatherLeggings;
    extern ArmorBase* aLeatherBoots;
    extern ArmorBase* aLeatherGloves;

    extern ArmorBase* aMailChest;
    extern ArmorBase* aMailHelm;
    extern ArmorBase* aMailLeggings;
    extern ArmorBase* aMailBoots;
    extern ArmorBase* aMailGloves;
    extern ArmorBase* aGoldenRing;
    extern ArmorBase* aJewelRing;


    vector<Item*> createRandLoots(int difficulty, int goldMaxQty, int wepMaxQty, int armMaxQty, int altMaxQty, int keyMaxQty);

    Item* createRandAltLoot(int itemDifficulty);

    ItemWeapon* createRandItemWeapon(int itemDifficulty);

    ItemArmor* createRandArmor(int itemDifficulty);

    ItemWeapon* createRandItemWeapon(int itemDifficulty);

    WeaponBase* getRandWeaponBase(WeaponType w, DamageType d);
    WeaponBase* getRandWeaponBase(WeaponType w);
    WeaponBase* getRandWeaponBase();
    ArmorBase* getRandArmorBase();

    Item* createItemFromBase(ItemBase* base, int itemDifficulty);
    ItemWeapon* createItemWeaponFromBase(WeaponBase* base, int itemDifficulty);
    ItemArmor* createArmorFromBase(ArmorBase* base, int itemDifficulty);
    ItemPotion* createPotionFromBase(PotionBase* pb, int itemDifficulty);

    Enchantment createEnchantmentFromBase(EnchantmentBase* base);

    ItemWeapon* createItemWeaponFromType(WeaponType w, DamageType d, int itemDifficulty);
    ItemWeapon* createItemWeaponFromType(WeaponType w, int itemDifficulty);

    ItemWeapon* applyConditionToItemWeapon(ItemWeapon* w, Condition* c, int itemDifficulty, bool prependName = true);

    ItemWeapon* applyRandConditionToItemWeapon(ItemWeapon* w, int itemDifficulty, bool prependName = true);



    ItemUtilitySpell* createScrollFromBase(ScrollBase* sb);

    Item* makeCoins(int qty);

    int calculateItemValue(Item* item);


    void initItemTemplates();
    void cleanupItemTemplates();

}


#endif /* defined(__Underneath__ItemGenerator__) */
