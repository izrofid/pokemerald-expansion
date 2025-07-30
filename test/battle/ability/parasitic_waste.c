#include "global.h"
#include "test/battle.h"

ASSUMPTIONS
{
    ASSUME(MoveHasAdditionalEffect(MOVE_SLUDGE_BOMB, MOVE_EFFECT_POISON) == TRUE);
}


SINGLE_BATTLE_TEST("Parasitic Waste does not affect primary poison effect")
{
    GIVEN
    {
        ASSUME(gMovesInfo[MOVE_POISON_GAS].effect == EFFECT_POISON);
        PLAYER(SPECIES_GARBODOR_MEGA) { Ability(ABILITY_PARASITIC_WASTE); }
        OPPONENT(SPECIES_WOBBUFFET);
    }
    WHEN
    {
        TURN { MOVE(player, MOVE_POISON_GAS); }
        TURN {}
    }
    SCENE
    {
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, opponent);
        STATUS_ICON(opponent, poison : TRUE);
    }
}

SINGLE_BATTLE_TEST("Parasitic Waste does not affect primary toxic effect")
{
    GIVEN
    {
        ASSUME(gMovesInfo[MOVE_TOXIC].effect == EFFECT_TOXIC);
        PLAYER(SPECIES_GARBODOR_MEGA) { Ability(ABILITY_PARASITIC_WASTE); }
        OPPONENT(SPECIES_WOBBUFFET);
    }
    WHEN
    {
        TURN { MOVE(player, MOVE_TOXIC); }
        TURN {}
    }
    SCENE
    {
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, opponent);
        STATUS_ICON(opponent, badPoison : TRUE);
    }
}



SINGLE_BATTLE_TEST("Parasitic Waste prevents moves from poisoning")
{
    GIVEN
    {
        PLAYER(SPECIES_GARBODOR_MEGA) { Ability(ABILITY_PARASITIC_WASTE); }
        OPPONENT(SPECIES_SNORLAX) { Ability(ABILITY_THICK_FAT); }
    }
    WHEN
    {
        TURN { MOVE(player, MOVE_SLUDGE_BOMB); }
        TURN {}
    }
    SCENE
    {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SLUDGE_BOMB, player);
        HP_BAR(opponent);

        NONE_OF
        {
            ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, opponent);
            STATUS_ICON(opponent, poison : TRUE);
        }
    }
}


SINGLE_BATTLE_TEST("Parasitic Waste will not prevent moves from poisoning if the user does not have it")
{
    GIVEN
    {
        PLAYER(SPECIES_GARBODOR_MEGA) { Ability(ABILITY_RUN_AWAY); }
        OPPONENT(SPECIES_SNORLAX) { Ability(ABILITY_THICK_FAT); }
    }
    WHEN
    {
        TURN { MOVE(player, MOVE_SLUDGE_BOMB); }
        TURN {}
    }
    SCENE
    {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SLUDGE_BOMB, player);
        HP_BAR(opponent);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, opponent);
        STATUS_ICON(opponent, poison : TRUE);
    }
}

SINGLE_BATTLE_TEST("Parasitic Waste will prevent moves from badly poisoning if the user does not have it.")
{
    GIVEN
    {
        PLAYER(SPECIES_GARBODOR_MEGA) { Ability(ABILITY_PARASITIC_WASTE); }
        OPPONENT(SPECIES_WOBBUFFET);
    }
    WHEN
    {
        TURN { MOVE(player, MOVE_MALIGNANT_CHAIN); }
        TURN {}
    }
    SCENE
    {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MALIGNANT_CHAIN, player);
        HP_BAR(opponent);
        NONE_OF
        {
            ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, opponent);
            STATUS_ICON(opponent, badPoison : TRUE);
        }
    }
}

SINGLE_BATTLE_TEST("Parasitic Waste will not prevent moves from badly poisoning if the user does not have it.")
{
    GIVEN
    {
        PLAYER(SPECIES_GARBODOR_MEGA) { Ability(ABILITY_RUN_AWAY); }
        OPPONENT(SPECIES_WOBBUFFET);
    }
    WHEN
    {
        TURN { MOVE(player, MOVE_MALIGNANT_CHAIN); }
        TURN {}
    }
    SCENE
    {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_MALIGNANT_CHAIN, player);
        HP_BAR(opponent);
        ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, opponent);
        STATUS_ICON(opponent, badPoison: TRUE);
    }
}

SINGLE_BATTLE_TEST("Parasitic Waste will not go through bulletproof")
{
    GIVEN
    {
        PLAYER(SPECIES_GARBODOR_MEGA) { Ability(ABILITY_RUN_AWAY); }
        OPPONENT(SPECIES_CHESPIN) { Ability(ABILITY_BULLETPROOF); }
    }
    WHEN
    {
        TURN { MOVE(player, MOVE_SLUDGE_BOMB); }
    }
    SCENE
    {
        ABILITY_POPUP(opponent, ABILITY_BULLETPROOF);

        NONE_OF
        {
            HP_BAR(opponent);
            ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, opponent);
            STATUS_ICON(opponent, badPoison : TRUE);
        }
    }
}

SINGLE_BATTLE_TEST("Parasitic Waste will not cause moves to affect steel types")
{
    GIVEN 
    {
        ASSUME(gSpeciesInfo[SPECIES_MEOWTH_GALAR].types[0] == TYPE_STEEL);
        PLAYER(SPECIES_GARBODOR_MEGA) {Ability(ABILITY_PARASITIC_WASTE);}
        OPPONENT(SPECIES_MEOWTH_GALAR);
    }

    WHEN
    {
        TURN { MOVE(player, MOVE_SLUDGE_BOMB); }
    }

    SCENE
    {
        NONE_OF 
        {
            ANIMATION(ANIM_TYPE_MOVE, MOVE_SLUDGE_BOMB, player);
            HP_BAR(opponent);
            ANIMATION(ANIM_TYPE_STATUS, B_ANIM_STATUS_PSN, opponent);
            STATUS_ICON(opponent, poison : TRUE);
        }

    }
}

DOUBLE_BATTLE_TEST("Parasitic Waste Sludge Wave absorb from all three targets in double battles")
{
    s16 damageFoeLeft;
    s16 damageFoeRight;
    s16 damagePlayerRight;
    s16 healedFoeLeft;
    s16 healedFoeRight;
    s16 healedPlayerRight;

    GIVEN {
        ASSUME(MoveHasAdditionalEffect(MOVE_SLUDGE_WAVE, MOVE_EFFECT_POISON)== TRUE);
        PLAYER(SPECIES_GARBODOR) { HP(1); Ability(ABILITY_PARASITIC_WASTE); }
        PLAYER(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
        OPPONENT(SPECIES_WOBBUFFET);
    } WHEN {
        TURN { MOVE(playerLeft, MOVE_SLUDGE_WAVE); }
    } SCENE {
        ANIMATION(ANIM_TYPE_MOVE, MOVE_SLUDGE_WAVE, playerLeft);
        HP_BAR(opponentLeft, captureDamage: &damageFoeLeft);
        HP_BAR(playerLeft, captureDamage: &healedFoeLeft);

        HP_BAR(playerRight, captureDamage: &damagePlayerRight);
        HP_BAR(playerLeft, captureDamage: &healedPlayerRight);

        HP_BAR(opponentRight, captureDamage: &damageFoeRight);
        HP_BAR(playerLeft, captureDamage: &healedFoeRight);

    } THEN {
        EXPECT_MUL_EQ(damageFoeLeft, Q_4_12(-0.5), healedFoeLeft);
        EXPECT_MUL_EQ(damageFoeRight, Q_4_12(-0.5), healedFoeRight);
        EXPECT_MUL_EQ(damagePlayerRight, Q_4_12(-0.5), healedPlayerRight);
    }
}