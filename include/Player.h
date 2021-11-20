#include "../include/Parametre.h"
#include "../include/Bonus.h"

#ifndef __PLAYER__
    #define __PLAYER__
    
    #define MAX_SPEED (20)              /* Vitesse limite du joueur */
    #define MAX_HEALTH (100)            /* Pv max du joueur au debut d'une nouvelle partie */
    #define MAX_HEALTH_BOOST (300)      /* Nombre max de point de vie maximum que le player peut avoir */
    #define MAX_MISSILES (3)            /* A: Nombre max de missiles que le joueur peut stocker */
    #define MAX_CHARGING (150)          /* Degats max du blaster chargé */
    #define MIN_CHARGING (100)          /* Temps minimum de charge du blaster */
    #define B_COOLDOWN (5)              /* Temps de rechargement entre deux balles */
    #define M_COOLDOWN (300)            /* Temps de rechargement entre deux missiles */
    #define RECOVERY_TIME (20)          /* Temps d'invincibilité entre deux coups */
    
    typedef struct {
        int pv;                 /* Points de vie */
        int x, y;               /* Angle Haut Gauche */
        int w, h;               /* Largeur et Hauteur du sprite */
        int pv_max;             /* Points de vie maximum du joueur */
        int money;              /* Argent nescessaire au shop */
        int vx, vy;             /* Vitesse */
        int ax, ay;             /* Acceleration */
        int shield;             /* Points de bouclier restant */
        int charging;           /* Time blaster key is pressed */
        int invincible;         /* Boolean to stop the death */
        int recovery_time;
        int b_cooldown;         /* Cooldown between two shot */
        int m_cooldown;         /* Cooldown between two missiles */
        int nb_missiles;        /* Nombres de missiles restants */
        int internal_clock;     /* NE JAMAIS REMETTRE A ZERO SAUF A LA MORT */
        Power perma_bonus;      /* Bonus permanent pris que le joueur possède */
        Power stock_bonus;      /* Bonus acheté par le joueur dans le shop */
    } Player;
    
    void init_player(Player *player, int x, int y, int w ,int h);
    void player_update(Player* player, Parametre* para);
    void move_player(Player *p, int key_hist[], Parametre *para);
    void apply_bonus(Player *player, Power power);
    int damage_player(Player* player, int amount);
    
    int player_dead(Player p);
#endif
