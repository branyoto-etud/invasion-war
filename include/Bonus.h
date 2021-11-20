
#ifndef __BONUS__
    #define __BONUS__
    
    #define HEALTH_BOOST (20)
    #define SHIELD_AMOUNT (30)
    #define HEAL_AMOUNT (20)
    #define BULLET_TIME (250)
    #define SCORE_TIME (500)
    #define NB_BONUS (10)
    
    typedef enum {
        Missile,            /* Ajoute 1 missile au joueur */
        Single_shot,        /* Tir un seul coup de blaster à la fois */
        Dual_shot,          /* Tir deux coups de blaster à la fois */
        Triple_shot,        /* Tir trois coups de blaster à la fois */
        Quadra_shot,        /* Tir quatre coups de blaster à la fois */
        Health_boost,       /* Ajoute HEALTH_BOOST pv au joueur (sans se limiter au nb de pv max) */
        Shield,             /* Ajoute SHIELD_AMOUNT pv de shield au joueur (ou) SHIELD_AMOUNT coups pouvants etre pris par le shield */
        Heal,               /* Ajoute REPAIR_AMOUNT pv au joueur (limité à pv_max) */
        Score_multiplier,   /* A: TU L'AVAIS OUBLIE */
        Bullet_time,        /* Ralentit tous les ennemis et tous les projectiles BULLET_TIME frames */
        Aucun = 1000
    } Power;

    typedef struct {
        int x, y, r,        /* A: position du bail + son diametre (A CHANGER PARCE QUE ADAM EST TROP MIGNON) */
            alive,          /* A: obligé de l'implémenter, sinon il se supp pas */
            internal_clock; /* A: allez je te laisse deviner */
        Power power;        /* A: type du bonus */
    } Bonus;
    
    void init_bonus(Bonus* bonus, Power power, int x, int y, int rayon);
    int bonus_update(Bonus* b, int window_height);
    int bonus_remove(Bonus tab[], int n);
    int get_bonus_price(Power power);
    const char* get_bonus_description(Power power);

#endif
