#include "../include/Parametre.h"

#ifndef __ENEMY__
    #define __ENEMY__
    
    #define BOSS_HP (15000)
    #define BOSS_COOLDOWN (100)
    #define BOSS_DASH_VALUE (158)
    
    /* Types des ennemis */
    typedef enum {
        E1,
        E2,
        E3,
        E4,
        Boss1,
        Boss2,
        Boss3,
        Boss4,
        Boss5,
        Special
    } EnemyType;
    
    /* Mouvements décrits par l'ennemi */
    typedef enum {
        Linear,
        Sin,
        NSin,
        ReverseU,
        NReverseU,
        Sidetodown,
        NSidetodown,
        Custom
    } Pattern;


    typedef struct {
        int damaged;
        int pv;                     /* Point de vie */
        int x, y;                   /* Position Angle haut gauche */
        int w, h;                   /* Dimention */
        int speed;                  /* Vitesse en nombre de pixel par mouvement */
        int alive;                  /* Est en vie ?? 1 oui, 0 non*/    
        int xstart;                 /* Point de départ en x (utile pour les fonction f_x(),g_x()) */
        int ystart;                 /* Point de départ en y (utile pour les fonction f_x(),g_x()) */
        int internal_clock;         /* NE JAMAIS REMETTRE A ZERO SAUF A LA MORT */
        EnemyType type;             /* Type de l'ennemi */ 
        Pattern pattern;            /* Pattern de l'ennemi */
    } Enemy;
    
    
    void init_enemy(Enemy *enemy, EnemyType type, Pattern pattern, int x, int y, int w, int h, int speed);
    int enemy_pv(EnemyType type);
    double f_x(double x);
    int enemy_shooting(Enemy *enemy);
    void boss_dash_update(Enemy* boss);
    int enemy_update(Enemy* e, Parametre *para);
    int enemy_remove(Enemy tab[], int n);
#endif
