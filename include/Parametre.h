#include "../include/Bonus.h"

#ifndef __PARAMETRE__
    #define __PARAMETRE__
    
    #define NB_MONDE (5)
    #define NB_KEY (8)
    
    typedef enum {
        Up_key,
        Left_key,
        Down_key,
        Right_key,
        Blaster_key,
        Missile_key,
        Bonus_key,
        Charged_key
    } Keys;
    
    /* Structure contenant les tailles de tous les elements du terrain */
    typedef struct {
        int player_width, player_height;
        
        int enemy1_width, enemy1_height;
        int enemy2_width, enemy2_height;
        int enemy3_width, enemy3_height;
        int enemy4_width, enemy4_height;
        int boss_width,   boss_height;
        
        int pcharged40_width, pcharged40_height;
        int pcharged60_width, pcharged60_height;
        int pcharged80_width, pcharged80_height;
        int pcharged100_width, pcharged100_height;
        int missile_width, missile_height;
        int explosion_width, explosion_height;
        int pblaster_width, pblaster_height;
        int eblaster_width, eblaster_height;
        int bblaster_width, bblaster_height;
        int emissile_auto_width, emissile_auto_height;
        int bmissile_auto_width, bmissile_auto_height;
        int bonus_width, bonus_height;
        int background_height;
    } Sizes;
    
    typedef struct {
        int width, height;
        /* Volume between 0% and 100% */
        int sound;
        int music;
        /* Booleen 1 actif, 0 inactif */
        int background;
        int power_up[NB_BONUS];
        /* Tableau des touches du joueur (Modifiable dans les options) */
        int prim_key[NB_KEY];
        int sec_key[NB_KEY];             /* Et un deuxieme pour avoir les fleches et zqsd par default */
        Sizes sizes;
    } Parametre;
    
    int no_bonus(Parametre* para);
    void init_sizes(Sizes *sizes, int val);
    
    void get_charged_size(Sizes *sizes, int damage, int*w, int*h);
    void get_enemy_size(Sizes *sizes, int type, int* w, int* h);
#endif
