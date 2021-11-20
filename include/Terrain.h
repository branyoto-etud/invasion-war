#include <stdlib.h>
#include <stdio.h>

#include <math.h>
#include "../include/Parametre.h"
#include "../include/Player.h"
#include "../include/Enemy.h"
#include "../include/Projectile.h"
#include "../include/Bonus.h"

#ifndef STRING_LEN___
#define STRING_LEN___ 128
#endif

#ifndef __TERRAIN__
    #define __TERRAIN__
    
    /* Nombre max d'element du meme type chargés */
    #define MAX_BONUS (100)                 /* 100 Bonus en meme temps sur l'écran c'est énorme mais je préfère prendre large */
    #define MAX_ENEMY (200)                 /* Pareil 200 ennemis c'est énorme mais au cas ou */
    #define MAX_PROJECTILE (500)            /* Bis repetita */
    #define BULLET_TIME_FACTOR (3)          /* Puissance du ralentissement */
    
    /* La hauteur et la largeur sont dans les Parametres */
    typedef struct {
        int score;
        int scroll;             /* Valeur actuel du scorlling permet de dessiner le background et de charger le ennemis */
        
        int lvlnum;             /* Numéro du monde */
        int nb_wave;            /* Nombre de vagues avant le boss */
        FILE* level;            /* Fichier texte contenant le déroulement du niveau */
        int lvlstate;           /* Etat du niveau: 1 niveau en cours, 0 niveau terminé */
        Player *player;         /* Joueur (jte jure) */

        /* Temps de vie d'un bonus */
        int bullet_time;        /* Ralentit l'environnement autour du joueur (update toutes les 2 frames) */
        int score_mult;         /* Multiplie les point obtenus par le joueurs par 2 */
        /* Taille courante des tableaux suivants */
        int nb_bonus;
        int nb_enemies;
        int nb_projectiles;
        /* Tableau fixe d'objets */
        Bonus bonus[MAX_BONUS];
        Enemy enemies[MAX_ENEMY];
        Projectile projectiles[MAX_PROJECTILE];
    } Terrain;
    
    int init_terrain(Terrain *terrain, Parametre* para, Player* player, int lvl_num, int fpos, int score);
    void add_boss(Terrain *terrain, Parametre* para);
    void add_enemy(Terrain *terrain, EnemyType type, Pattern pattern, int x, int y, int w, int h, int speed);
    void add_projectile(Terrain *terrain, ProjectileType type, int x, int y, int w, int h, int damage, int dx, int dy, int speed);
    void blaster(Terrain *terrain, Parametre *para);
    int shoot(Terrain *terrain, int key_hist[], Parametre *para);
    
    int use_key(Terrain *terrain, int key_hist[], Parametre *para);
    void inc_clocks(Terrain *terrain);
    void enemy_death(Terrain* terrain, Enemy* enemy, Parametre* para);
    int collision_projectile_player(Projectile pr, Player p);
    int collision_projectile_enemy(Projectile p, Enemy e);
    int collision_player_enemy(Player p, Enemy e);
    int collision_player_bonus(Player p, Bonus b);
    int terrain_collisions(Terrain* terrain, Parametre* para);
    
    void boss_dash(Terrain* terrain, Parametre *para, Enemy* enemy);
    void boss_seven_shoot(Terrain* terrain, Parametre* para, Enemy* enemy);
    void boss_summon_enemies(Terrain* terrain, Parametre* para, Enemy* enemy);
    void boss_auto_shoot(Terrain* terrain, Parametre* para, Enemy* enemy);
    void boss_grid_shoot(Terrain* terrain, Parametre* para, Enemy* enemy);
    void boss_select_action(Terrain *terrain, Parametre* para, Enemy* enemy, int action_number);
    void boss_shoot(Terrain* terrain, Parametre* para, Enemy* enemy);
    void enemy_shoot(Terrain* terrain, Parametre* para, Enemy* enemy);
    
    int terrain_update(Terrain* terrain, Parametre *para, int* shop, char* fname, int last_world);
    
#endif
