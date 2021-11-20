#include "../include/Menu.h"
#include "../include/Terrain.h"
#include "../include/Fichier.h"
#include <stdlib.h>
#include <time.h>

/* Initialise le terrain en ouvrant le fichier lvl_num 
 * Return 1 si le fichier est correctement chargé, 0 sinon */
/* Cette fonction sera appelé seulement pour ouvrir un nouveau niveau pas pour charger un niveau commencé */
int init_terrain(Terrain* terrain, Parametre* para, Player *player, int lvl_num, int fpos, int score) {
    char path[STRING_LEN___];
    /* Ouverture du fichier + gestion d'erreur */
    sprintf(path, "data/level/level%d.lvl", lvl_num);
    if ((terrain->level = fopen(path, "r")) == NULL) {
        printf("Error : file data/level/level_%d.lvl don't exist\n", lvl_num);
        return 0;
    }
    if (player == NULL) return 0;
    
    fseek(terrain->level, fpos, SEEK_SET);
    terrain->scroll = 0;
    
    terrain->lvlnum = lvl_num;
    terrain->score = score;
    terrain->lvlstate = 1;
    terrain->nb_bonus = 0;
    terrain->nb_wave = 1;
    terrain->nb_enemies = 0;
    terrain->score_mult = 0;
    terrain->bullet_time = 0;
    terrain->nb_projectiles = 0;
    
    terrain->player = player;
    player->x = para->width/2 - player->w/2;
    player->y = 3*para->height/4;
    return 1;
}

/* Ajoute un projectile à la fin du tableau 'terrain->projectiles' */
void add_projectile(Terrain *terrain, ProjectileType type, int x, int y, int w, int h, int damage, int dx, int dy, int speed) {
    if (terrain->nb_projectiles >= MAX_PROJECTILE) {
        terrain->nb_projectiles--;
    }
    init_projectile(&(terrain->projectiles[terrain->nb_projectiles++]), type, x, y, w, h, damage, dx, dy, speed);
}

/* Fait apparaitre un Boss en fonction du monde courant */
void add_boss(Terrain *terrain, Parametre* para) {
    switch (terrain->lvlnum) {
        case 0: add_enemy(terrain, Boss1, Custom, para->width / 2, -para->sizes.boss_width/2, para->sizes.boss_width, para->sizes.boss_height, -1); break;
        case 1: add_enemy(terrain, Boss2, Custom, para->width / 2, -para->sizes.boss_width/2, para->sizes.boss_width, para->sizes.boss_height, -1); break;
        case 2: add_enemy(terrain, Boss3, Custom, para->width / 2, -para->sizes.boss_width/2, para->sizes.boss_width, para->sizes.boss_height, -1); break;
        case 3: add_enemy(terrain, Boss4, Custom, para->width / 2, -para->sizes.boss_width/2, para->sizes.boss_width, para->sizes.boss_height, -1); break;
        case 4: add_enemy(terrain, Boss5, Custom, para->width / 2, -para->sizes.boss_width/2, para->sizes.boss_width, para->sizes.boss_height, -1); break;
        default: break;
    }
}

/* Ajoute un projectile à la fin du tableau 'terrain->enemies' */
void add_enemy(Terrain *terrain, EnemyType type, Pattern pattern, int x, int y, int w, int h, int speed) {
    if (terrain->nb_enemies >= MAX_ENEMY) {
        terrain->nb_enemies--;
    }
    init_enemy(&(terrain->enemies[terrain->nb_enemies++]), type, pattern, x, y, w, h, speed);
}


/* Tir une fois fait 40 points de dégats */
static void single_shot(Terrain* terrain, Parametre* para) {
    add_projectile(terrain, PBlaster, terrain->player->x + terrain->player->w/2, terrain->player->y, para->sizes.pblaster_width, para->sizes.pblaster_height, 40, 0 ,-1, 10);
}
/* Tir deux fois fait 42 points de dégats au total */
static void dual_shot(Terrain* terrain, Parametre* para) {
    add_projectile(terrain, PBlaster, terrain->player->x + terrain->player->w*1/3, terrain->player->y, para->sizes.pblaster_width, para->sizes.pblaster_height, 21, 0 ,-1, 10);
    add_projectile(terrain, PBlaster, terrain->player->x + terrain->player->w*2/3, terrain->player->y, para->sizes.pblaster_width, para->sizes.pblaster_height, 21, 0 ,-1, 10);
}
/* Tir trois fois fait 45 points de dégats au total */
static void triple_shot(Terrain* terrain, Parametre* para) {
    add_projectile(terrain, PBlaster, terrain->player->x + terrain->player->w*1/4, terrain->player->y, para->sizes.pblaster_width, para->sizes.pblaster_height, 15, 0 ,-1, 10);
    add_projectile(terrain, PBlaster, terrain->player->x + terrain->player->w*2/4, terrain->player->y, para->sizes.pblaster_width, para->sizes.pblaster_height, 15, 0 ,-1, 10);
    add_projectile(terrain, PBlaster, terrain->player->x + terrain->player->w*3/4, terrain->player->y, para->sizes.pblaster_width, para->sizes.pblaster_height, 15, 0 ,-1, 10);
}
/* Tir quatre fois fait 48 points de dégats au total */
static void quadra_shot(Terrain* terrain, Parametre* para) {
    add_projectile(terrain, PBlaster, terrain->player->x + terrain->player->w*1/5, terrain->player->y, para->sizes.pblaster_width, para->sizes.pblaster_height, 12, 0 ,-1, 10);
    add_projectile(terrain, PBlaster, terrain->player->x + terrain->player->w*2/5, terrain->player->y, para->sizes.pblaster_width, para->sizes.pblaster_height, 12, 0 ,-1, 10);
    add_projectile(terrain, PBlaster, terrain->player->x + terrain->player->w*3/5, terrain->player->y, para->sizes.pblaster_width, para->sizes.pblaster_height, 12, 0 ,-1, 10);
    add_projectile(terrain, PBlaster, terrain->player->x + terrain->player->w*4/5, terrain->player->y, para->sizes.pblaster_width, para->sizes.pblaster_height, 12, 0 ,-1, 10);
}

/* Fait tirer le joueur en fonction du bonus qu'il possède */
void blaster(Terrain *terrain, Parametre *para) {
    switch (terrain->player->perma_bonus) {
        case Single_shot: single_shot(terrain, para); break;
        case Dual_shot: dual_shot(terrain, para); break;
        case Triple_shot: triple_shot(terrain, para); break;
        case Quadra_shot: quadra_shot(terrain, para); break;
        default: printf("Error cannot have the bonus number %d as a permanant bonus\n", terrain->player->perma_bonus);
    }
}

/* Gère les tirs du joueur en fonctions des touches appuyées 
 * Renvoie 1 si il y a eu tir et 0 sinon */
int shoot(Terrain *terrain, int key_hist[], Parametre *para) {
    Player *p = terrain->player;
    
    /* Blaster */
    if (p->b_cooldown == 0) {
        /* Chargement du blaster */
        if (key_hist[para->prim_key[Charged_key]] || key_hist[para->sec_key[Charged_key]] ) {
            p->charging = p->charging+1 > MAX_CHARGING ? MAX_CHARGING : p->charging+1;
        } else if (p->charging >= MIN_CHARGING) {      /* Tir chargé */
            int tmp_w, tmp_h;
            get_charged_size(&(para->sizes), p->charging, &tmp_w, &tmp_h);
            add_projectile(terrain, PBlaster_Charged, p->x + p->w/2, p->y, tmp_w, tmp_h, p->charging*5, 0, -1, 10);
            p->charging = 0;
            return 1;
        } else {
            p->charging = 0;
        }
        if ((key_hist[para->prim_key[Blaster_key]] || key_hist[para->sec_key[Blaster_key]] ) ) {
            blaster(terrain, para);
            p->b_cooldown = B_COOLDOWN;
            return 1;
        }
    }
    /* Missile */
    if (p->nb_missiles && p->m_cooldown == 0 && (key_hist[para->prim_key[Missile_key]] || key_hist[para->sec_key[Missile_key]] )) {
        add_projectile(terrain, PMissile, p->x + p->w/2, p->y, para->sizes.missile_width, para->sizes.missile_height, enemy_pv(Boss1 + terrain->lvlnum)/3, 0 ,-1, 15);
        p->nb_missiles --;
        p->m_cooldown = M_COOLDOWN;
        return 1;
    }
    return 0;
}

/* Utilise les touches appuyées */
int use_key(Terrain *terrain, int key_hist[], Parametre *para) {
    /* Pause */
    if (key_hist[MLV_KEYBOARD_ESCAPE]) {
        key_hist[MLV_KEYBOARD_ESCAPE] = 0;
        return menu_pause(para);
    }
    /* Deplacement */
    move_player( terrain->player, key_hist, para);
    
    /* Shoot */
    shoot(terrain, key_hist, para);
    
    if (key_hist[270] && key_hist[105]) { /* i + */
        terrain->player->invincible = 1;
    }
    if (key_hist[269] && key_hist[105]) { /* i - */
        terrain->player->invincible = 0;
    }
    if (key_hist[270] && key_hist[269] && terrain->level != NULL) {  /* - + */
        fseek(terrain->level, -1, SEEK_END);
        read_file(terrain, para, NULL, 0);
        terrain->nb_enemies = 0;
    }
    
    /* Bonus Use */
    if (key_hist[para->prim_key[Bonus_key]] || key_hist[para->sec_key[Bonus_key]]) {
        if (terrain->player->stock_bonus == Bullet_time) {
            terrain->bullet_time = 1;
        } else if (terrain->player->stock_bonus == Score_multiplier) {
            terrain->score_mult = 1;
        } else {
            apply_bonus(terrain->player, terrain->player->stock_bonus);
        }
        terrain->player->stock_bonus = Aucun;
    }
    
    return 1;
}

void inc_clocks(Terrain *terrain) {
    /* Incrémente toutes les internal_clock */
    /* Fonction indépendante de terrain_update() pour qu'elles soient toutes incrémentés en meme temps */
    int i;

    terrain->player->internal_clock++;

    if (terrain->bullet_time > 0) {
        terrain->bullet_time++;
        if (terrain->bullet_time > BULLET_TIME) {
            terrain->bullet_time = 0;
        }
    }
    
    if (terrain->score_mult > 0) {
        terrain->score_mult++;
        if (terrain->score_mult > SCORE_TIME) {
            terrain->score_mult = 0;
        }
    }
    
    if ((terrain->bullet_time % BULLET_TIME_FACTOR) != 0)
        return;

    terrain->scroll++;
    for (i=0; i<terrain->nb_enemies; i++) {
        terrain->enemies[i].internal_clock++;
    }
    for (i=0; i<terrain->nb_projectiles; i++) {
        terrain->projectiles[i].internal_clock++;
    }
    for (i=0; i<terrain->nb_bonus; i++) {
        terrain->bonus[i].internal_clock++;
    }
}

/* Gère la mort d'un ennemi (donne des points, fait apparaitre des bonus...) */
void enemy_death(Terrain* terrain, Enemy* enemy, Parametre* para) {
    int w,h;
    enemy->internal_clock = 0;
    enemy->alive = 0;
    enemy->pv = 0;
    
    if (Boss1 <= enemy->type && enemy->type <= Boss5) {
        terrain->score += 75000*((terrain->score_mult!=0) + 1);
        terrain->player->money += 75000*((terrain->score_mult!=0) + 1);
        terrain->player->pv = terrain->player->pv < MAX_HEALTH ? MAX_HEALTH : terrain->player->pv;
    } else {
        terrain->score += (100 * (enemy->type + 1))*((terrain->score_mult!=0) + 1);
        terrain->player->money += (100 * (enemy->type + 1))*((terrain->score_mult!=0) + 1);
    }
    
    if (!(rand() % 20) || enemy->type == Special) {
        int bonus_number;
        if (terrain->nb_bonus >= MAX_BONUS) {
            terrain->nb_bonus--;
        }
        if (!no_bonus(para)) {
            do {bonus_number = rand() % NB_BONUS;} while (para->power_up[bonus_number] != 1);
            init_bonus(&(terrain->bonus[terrain->nb_bonus++]), bonus_number, enemy->x+enemy->w/2, enemy->y+enemy->h/2, para->sizes.bonus_width);
        }
    }

    if (enemy->type == E4) { /* Split on death */
        get_enemy_size(&(para->sizes), E1, &w, &h);
        if (enemy->x > 0)
            add_enemy(terrain, E1, Linear, enemy->x, enemy->y - enemy->h/2, w, h, enemy->speed);
        if (enemy->x + enemy->w < para->width)
            add_enemy(terrain, E1, Linear, enemy->x + enemy->w, enemy->y - enemy->h/2, w, h, enemy->speed);
    }
}

/* ------------GESTION DES COLLISIONS------------ */

static int dist_carre(int x1, int y1, int x2, int y2) {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

int collision_projectile_player(Projectile pr, Player p) {
    int r = pr.w/2 + p.w/2 - 10;
    return dist_carre((pr.x), (pr.y), (p.x + p.w/2), (p.y + p.h/2)) <= r * r;
}

int collision_projectile_enemy(Projectile p, Enemy e) {
    int r = e.w/2 + p.w/2;
    return dist_carre((e.x + e.w/2), (e.y + e.h/2), (p.x), (p.y)) <= r * r;
}

int collision_player_enemy(Player p, Enemy e) {
    int r = e.w/2 + p.w/2;
    return dist_carre((e.x + e.w/2), (e.y + e.h/2), (p.x + p.w/2), (p.y + p.h/2)) <= r * r;
}

int collision_player_bonus(Player p, Bonus b) {
    int r = b.r/2 + p.w/2;
    return dist_carre((b.x + b.r/2), (b.y + b.r/2), (p.x + p.w/2), (p.y + p.h/2)) <= r * r;
}

/* Check les collisions entre tous les elements du terrain */
int terrain_collisions(Terrain* terrain, Parametre* para) {
    Player* player = terrain->player;
    int i, j;
    
    /* Collision player/projectile */
    for (i=0; i<terrain->nb_projectiles; i++) {
        /* Si c'est un tir allié ou que le tir est mort */
        if (terrain->projectiles[i].type < EBlaster || terrain->projectiles[i].alive == 0) {
            continue;
        }
        if (collision_projectile_player(terrain->projectiles[i], *player)) {
            damage_player(player, terrain->projectiles[i].damage);
            terrain->projectiles[i].internal_clock = 0;
            terrain->projectiles[i].alive = 0;
        }
    }
    /* Collision enemy/projectile */
    for (i=0; i<terrain->nb_projectiles; i++) {
        Projectile* prjct = &(terrain->projectiles[i]);
        /* Si c'est un tir enemy ou que le tir est mort */
        if (prjct->type >= EBlaster || prjct->alive == 0)
            continue;
        for (j=0; j<terrain->nb_enemies; j++) {
            if (terrain->enemies[j].alive == 0)
                continue;
            if (Boss1 <= terrain->enemies[j].type && terrain->enemies[j].type <= Boss5 && terrain->enemies[j].speed != -1)
                continue;
            if (collision_projectile_enemy(*prjct, terrain->enemies[j])) {
                if (prjct->type != PMissile) {
                    terrain->enemies[j].damaged = 5;
                    terrain->enemies[j].pv -= prjct->damage;
                }
                if (prjct->type != PBlaster_Charged) {
                    prjct->internal_clock = 0;
                    prjct->alive = 0;
                }
            }
            if (terrain->enemies[j].pv <= 0) {
                enemy_death(terrain, &(terrain->enemies[j]), para);
            }
        }

        /* Si le Missile est mort --> Zone d'explosion (collision mais avec un plus gros rayon) */
        if (prjct->type == PMissile && prjct->alive == 0) {
            prjct->h = para->sizes.explosion_height;
            prjct->w = para->sizes.explosion_width;
            prjct->internal_clock = 0;
            for (j=0; j<terrain->nb_enemies; j++) {
                if (collision_projectile_enemy(*prjct, terrain->enemies[j])) {
                    terrain->enemies[j].pv -= prjct->damage;
                    terrain->enemies[j].damaged = 5;
                    if (terrain->enemies[j].pv <= 0) {
                        enemy_death(terrain, &(terrain->enemies[j]), para);
                    }
                }
            }
        }
    }
    /* Collision enemy/player */
    for (i=0; i<terrain->nb_enemies; i++) {
        if (!terrain->enemies[i].alive) {
            continue;
        }
        if (collision_player_enemy(*player, terrain->enemies[i])) {
            if (Boss1 <= terrain->enemies[i].type && terrain->enemies[i].type <= Boss5) {
                damage_player(player, 150); /* Ne tue pas instant (volontaire) */
                if (terrain->enemies[i].speed == -1)
                    terrain->enemies[i].pv -= 500;
                if (terrain->enemies[i].pv <= 0)
                    enemy_death(terrain, &(terrain->enemies[i]), para);
            } else {
                damage_player(player, terrain->enemies[i].pv / 30);
                enemy_death(terrain, &(terrain->enemies[i]), para);
            }
        }
    }
    /* Collision bonus/player */
    for (i=0; i<terrain->nb_bonus; i++) {
        if (!terrain->bonus[i].alive) {
            continue;
        }
        if (collision_player_bonus(*player, terrain->bonus[i])) {
            terrain->bonus[i].alive = 0;
            /*Applique l'effet du bonus sur le joueur  */
            if (terrain->bonus[i].power == Bullet_time) {
                terrain->bullet_time = 1;
            } else if (terrain->bonus[i].power == Score_multiplier) {
                terrain->score_mult = 1;
            } else {
                apply_bonus(player, terrain->bonus[i].power);              
            }
        }
    }
    return 0;
}

/* ACTIONS DU BOSS */

/* Prépare le boss à foncer vers le bas de l'écran */
void boss_dash(Terrain* terrain, Parametre *para, Enemy* enemy) {
    enemy->speed = BOSS_DASH_VALUE;
}

/* Fait tirer 7 BBlaster à partir du centtre du boss */
void boss_seven_shoot(Terrain* terrain, Parametre* para, Enemy* enemy) {
    add_projectile(terrain, BBlaster, enemy->x + enemy->w/2, enemy->y + enemy->h, para->sizes.bblaster_width, para->sizes.bblaster_height, 10, 0, 0, 10);
    update_projectile_direction(&(terrain->projectiles[terrain->nb_projectiles-1]), enemy->x + enemy->w/2 - 10, enemy->y + enemy->h + 10, 8);
    add_projectile(terrain, BBlaster, enemy->x + enemy->w/2, enemy->y + enemy->h, para->sizes.bblaster_width, para->sizes.bblaster_height, 10, 0, 0, 10);
    update_projectile_direction(&(terrain->projectiles[terrain->nb_projectiles-1]), enemy->x + enemy->w/2 - 11, enemy->y + enemy->h + 21, 9);
    add_projectile(terrain, BBlaster, enemy->x + enemy->w/2, enemy->y + enemy->h, para->sizes.bblaster_width, para->sizes.bblaster_height, 10, 0, 0, 10);
    update_projectile_direction(&(terrain->projectiles[terrain->nb_projectiles-1]), enemy->x + enemy->w/2 -  6, enemy->y + enemy->h + 23, 9);
    add_projectile(terrain, BBlaster, enemy->x + enemy->w/2, enemy->y + enemy->h, para->sizes.bblaster_width, para->sizes.bblaster_height, 10, 0, 0, 10);
    update_projectile_direction(&(terrain->projectiles[terrain->nb_projectiles-1]), enemy->x + enemy->w/2 -  0, enemy->y + enemy->h + 10, 8);
    add_projectile(terrain, BBlaster, enemy->x + enemy->w/2, enemy->y + enemy->h, para->sizes.bblaster_width, para->sizes.bblaster_height, 10, 0, 0, 10);
    update_projectile_direction(&(terrain->projectiles[terrain->nb_projectiles-1]), enemy->x + enemy->w/2 +  6, enemy->y + enemy->h + 23, 9);
    add_projectile(terrain, BBlaster, enemy->x + enemy->w/2, enemy->y + enemy->h, para->sizes.bblaster_width, para->sizes.bblaster_height, 10, 0, 0, 10);
    update_projectile_direction(&(terrain->projectiles[terrain->nb_projectiles-1]), enemy->x + enemy->w/2 + 11, enemy->y + enemy->h + 21, 9);
    add_projectile(terrain, BBlaster, enemy->x + enemy->w/2, enemy->y + enemy->h, para->sizes.bblaster_width, para->sizes.bblaster_height, 10, 0, 0, 10);
    update_projectile_direction(&(terrain->projectiles[terrain->nb_projectiles-1]), enemy->x + enemy->w/2 + 10, enemy->y + enemy->h + 10, 8);
}

/* Fait apparaitre une ligne de 4 ennemis */
void boss_summon_enemies(Terrain* terrain, Parametre* para, Enemy* enemy) {
    add_enemy(terrain, E1, Linear, 1*para->width/5, enemy->y + enemy->h, para->sizes.enemy1_width, para->sizes.enemy1_height, 2);
    add_enemy(terrain, E1, Linear, 2*para->width/5, enemy->y + enemy->h, para->sizes.enemy1_width, para->sizes.enemy1_height, 2);
    add_enemy(terrain, E1, Linear, 3*para->width/5, enemy->y + enemy->h, para->sizes.enemy1_width, para->sizes.enemy1_height, 2);
    add_enemy(terrain, E1, Linear, 4*para->width/5, enemy->y + enemy->h, para->sizes.enemy1_width, para->sizes.enemy1_height, 2);
}

/* Fait tirer 2 tir à tête chercheuse au boss */
void boss_auto_shoot(Terrain* terrain, Parametre* para, Enemy* enemy) {
    add_projectile(terrain, BMissile_auto, enemy->x, enemy->y + enemy->h, para->sizes.bmissile_auto_width, para->sizes.bmissile_auto_height, 25, 0, 0, 10);
    add_projectile(terrain, BMissile_auto, enemy->x + enemy->w, enemy->y + enemy->h, para->sizes.bmissile_auto_width, para->sizes.bmissile_auto_height, 25, 0, 0, 10);
}

/* Fait tirer des BBlaster venant des cotés de l'écran et le traversant */
void boss_grid_shoot(Terrain* terrain, Parametre* para, Enemy* enemy) {
    int i, ecart = para->sizes.player_width * 2 + 10;
    
    for (i=0; para->width - 10 - ecart * i > enemy->y + enemy->h; ++i) {
        add_projectile(terrain, BBlaster, 5, para->height - 10 - ecart * i, para->sizes.bblaster_width, para->sizes.bblaster_height, 10, 3, 0, 10);
        add_projectile(terrain, BBlaster, para->width - 5, para->height - 10 - ecart * i, para->sizes.bblaster_width, para->sizes.bblaster_height, 10, -3, 0, 10);
    }
}

/* Fait faire une action au boss en fonction de 'action_number' */
void boss_select_action(Terrain *terrain, Parametre* para, Enemy* enemy, int action_number) {
    switch (action_number) {
        case 0: boss_dash(terrain, para, enemy); break;
        case 1: boss_seven_shoot(terrain, para, enemy); break;
        case 2: boss_summon_enemies(terrain, para, enemy); break;
        case 3: boss_auto_shoot(terrain, para, enemy); break;
        case 4: boss_grid_shoot(terrain, para, enemy); break;
        default:
            fprintf(stderr, "Action impossible %d\n", action_number);
    }
}

/* Appel boss_select_action avec le numéro d'action correspondant au boss du niveau courant
 * Par exemple le Boss1 fera l'action 0
 *             le Boss2 fera l'action 0 ou 1
 *             ... */
void boss_shoot(Terrain* terrain, Parametre* para, Enemy* enemy) {
    int num = rand() % 100;
    
    if (enemy->type == Boss1) {         /* 100, 0, 0, 0, 0 pourcents */
        boss_select_action(terrain, para, enemy, 0);
    } else if (enemy->type == Boss2) {  /* 25, 75, 0, 0, 0 pourcents */
        boss_select_action(terrain, para, enemy, num >= 25);
    } else if (enemy->type == Boss3) {  /* 15, 25, 60, 0, 0 pourcents */
        if (num < 15) { boss_select_action(terrain, para, enemy, 0); }
        else if (num < 40) { boss_select_action(terrain, para, enemy, 1); }
        else { boss_select_action(terrain, para, enemy, 2); }
    } else if (enemy->type == Boss4) {  /* 5, 15, 30, 50, 0 pourcents */
        if (num < 5) { boss_select_action(terrain, para, enemy, 0); }
        else if (num < 20) { boss_select_action(terrain, para, enemy, 1); }
        else if (num < 50) { boss_select_action(terrain, para, enemy, 2); }
        else { boss_select_action(terrain, para, enemy, 3); }
    } else if (enemy->type == Boss5) {  /* 2, 8, 20, 25, 45 pourcents */
        if (num < 2) { boss_select_action(terrain, para, enemy, 0); }
        else if (num < 10) { boss_select_action(terrain, para, enemy, 1); }
        else if (num < 30) { boss_select_action(terrain, para, enemy, 2); }
        else if (num < 55) { boss_select_action(terrain, para, enemy, 3); }
        else { boss_select_action(terrain, para, enemy, 4); }
    }
}

/* Fait tirer l'ennemi 'enemy' en fonction de son type */
void enemy_shoot(Terrain* terrain, Parametre* para, Enemy* enemy) {
    ProjectileType type;
    int damage = 0, speed = enemy->speed;
    switch (enemy->type) {
        case E1:      type = EBlaster;  damage = 5; speed += 1; break;
        case E2:  type = EBlaster_sin; damage =  7; speed += 4; break;
        case E3: type = EMissile_auto; damage = 25; speed += 8; break;
        case E4:      type = EBlaster;  damage = 7; speed += 7; break;
        default: break;
    }
    if ((Boss1 <= enemy->type && enemy->type <= Boss5)) {
        boss_shoot(terrain, para, enemy); return;
    }
    add_projectile(terrain, type, enemy->x + enemy->w/2, enemy->y + enemy->h, para->sizes.eblaster_width, para->sizes.eblaster_height, damage, 0, speed, speed);
    if (enemy->type == E4) update_projectile_direction(&(terrain->projectiles[terrain->nb_projectiles-1]), terrain->player->x, terrain->player->y, speed);
}

/* Met tous les éléments du terrain à jour
 * Renvoie 1 si tout se passe bien et 0 si erreur de lecture du fichier*/
int terrain_update(Terrain* terrain, Parametre *para, int* shop, char* fname, int last_world) {
    Player *p = terrain->player;
    int i;
    
    /* Lecture du fichier */
    if (terrain->lvlstate != 0) {
        if (read_file(terrain, para, fname, last_world) == 0) {
            return 0;
        }
        if (shop != NULL && terrain->lvlstate == 2) {
            *shop = 1;
        }
    }
    
    /* Mise à jour du joueur */
    player_update(p, para);

    if ((terrain->bullet_time % BULLET_TIME_FACTOR) != 0)
        return 1;
    
    /* Mise à jour des projectiles */
    for (i = terrain->nb_projectiles-1; i >= 0; i--) {
        projectile_update(&(terrain->projectiles[i]), para, p->x + p->w / 2, p->y + p->h / 2);
    }
    /* Mise à jour des bonus */
    for (i = terrain->nb_bonus-1; i >= 0; i--) {
        bonus_update(&(terrain->bonus[i]), para->height);
    }
    /* Mise à jour des ennemis */
    for (i = terrain->nb_enemies-1; i >= 0; i--) {
        if (enemy_update(&(terrain->enemies[i]), para) == 0)
            damage_player(terrain->player, 8);
        if (enemy_shooting(&(terrain->enemies[i])) ) {
            enemy_shoot(terrain, para, &(terrain->enemies[i]));
        }
    }
    return 1;
}
