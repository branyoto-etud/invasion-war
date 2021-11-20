#include "../include/Projectile.h"
#include "../include/Parametre.h"
#include "../include/Player.h"
#include "../include/Enemy.h"

/* Initialise toutes les tailles à 'val' */
void init_sizes(Sizes *sizes, int val) {
    sizes->player_width = sizes->player_height = val;
    
    sizes->enemy1_width = sizes->enemy1_height = val;
    sizes->enemy2_width = sizes->enemy2_height = val;
    sizes->enemy3_width = sizes->enemy3_height = val;
    sizes->enemy4_width = sizes->enemy4_height = val;
    sizes->boss_width = sizes->boss_height = val;
    
    sizes->pcharged40_width = sizes->pcharged40_height = val;
    sizes->pcharged60_width = sizes->pcharged60_height = val;
    sizes->pcharged80_width = sizes->pcharged80_height = val;
    sizes->pcharged100_width = sizes->pcharged100_height = val;
    sizes->missile_width = sizes->missile_height = val;
    sizes->explosion_width = sizes->explosion_height = val;
    sizes->eblaster_width = sizes->eblaster_height = val;
    sizes->emissile_auto_width = sizes->emissile_auto_height = val;
    sizes->bmissile_auto_width = sizes->bmissile_auto_height = val;
    sizes->pblaster_width = sizes->pblaster_height = val;
    sizes->bblaster_width = sizes->bblaster_height = val;
    sizes->bonus_width = sizes->bonus_height = val;
}

/* Stock dans 'w' et 'h' la valeur de sizes correspondante à un blaster chargé avec 'damage' puissance */
void get_charged_size(Sizes *sizes, int damage, int*w, int*h) {
    double ecart = (MAX_CHARGING - MIN_CHARGING) / 4.0;  /* On divise l'écart en 4 */
    
    if      (damage <= MIN_CHARGING + 1 * ecart) {*w = sizes->pcharged40_width; *h = sizes->pcharged40_height;}
    else if (damage <= MIN_CHARGING + 2 * ecart) {*w = sizes->pcharged60_width; *h = sizes->pcharged60_height;}
    else if (damage <= MIN_CHARGING + 3 * ecart) {*w = sizes->pcharged80_width; *h = sizes->pcharged80_height;}
    else                                         {*w = sizes->pcharged100_width; *h = sizes->pcharged100_height;}
}
/* Renvoie 1 si aucun bonus n'est activé 0 sinon */
int no_bonus(Parametre* para) {
    int i;
    for (i=0; i<NB_BONUS; i++) {
        if (para->power_up[i]) 
            return 0;
    }
    return 1;
}

/* Stock dans 'w' et 'h' la valeur de sizes correspondante à l'ennemi de type 'type' */
void get_enemy_size(Sizes *sizes, int type, int* w, int* h) {
    switch ((EnemyType)type) {
        case Special:
        case E1: *w = sizes->enemy1_width; *h = sizes->enemy1_height; break;
        case E2: *w = sizes->enemy2_width; *h = sizes->enemy2_height; break;
        case E3: *w = sizes->enemy3_width; *h = sizes->enemy3_height; break;
        case E4: *w = sizes->enemy4_width; *h = sizes->enemy4_height; break;
        case Boss1: *w = sizes->boss_width; *h = sizes->boss_height; break;
        case Boss2: *w = sizes->boss_width; *h = sizes->boss_height; break;
        case Boss3: *w = sizes->boss_width; *h = sizes->boss_height; break;
        case Boss4: *w = sizes->boss_width; *h = sizes->boss_height; break;
        case Boss5: *w = sizes->boss_width; *h = sizes->boss_height; break;
    }
}

