#include "../include/Parametre.h"


#ifndef __PROJECTILE__
    #define __PROJECTILE__
    
    #define AFTERLIFE_TIME (20)         /* Possible constante de temps de vie */
    #define CHARGING_RATIO (10.0)       /* Divise le temps de chargement pour donner le nombre de dégat du projectile */
    
    typedef enum {
        /* Player Projectile */
        PBlaster = 0,               /* Tir classique ligne droite */
        PBlaster_Charged,           /* Tir en ligne droite faisant plus de dégats */
        PMissile,                   /* Tir en ligne droite puissant et causant des dégats de zone */
        /* Enemy Projectile */
        EBlaster = 10,              /* Tir classique possédant une direction */
        EBlaster_sin,               /* Tir classique qui suit une courbe sinusoidale */
        EMissile_auto,              /* Tir puissant mais lent qui vise toujours le joueur */
        BBlaster,                   /* Tir puissant tiré par le boss */
        BMissile_auto
    } ProjectileType;

    typedef struct {
        int alive;                  /* Booleen. 1 est en vie, 0 est mort. */ 
        int speed;                  /* Nombres de pixels déplacé par frames */
        int x, y;                   /* centre */
        int dx,dy;                  /* directions du projectile (utile seulement si le projectile est de type EBlaster) */
        int damage;                 /* Dégats causé lors de la collision avec ce projectile */    
        int internal_clock;         /* NE JAMAIS REMETTRE A ZERO SAUF A LA MORT */
        ProjectileType type;        /* Type du projectile (voir plus haut) */
        int w, h;                   /* Dimention du projectile */
    } Projectile;
    
    void init_projectile(Projectile* p, ProjectileType type, int x, int y, int w, int h, int damage, int dx, int dy, int speed);
    void update_projectile_direction(Projectile* p, int px, int py, int speed);
    void move_to_player(Projectile *p, int px, int py);
        
    int projectile_update(Projectile *p, Parametre* para, int px, int py);
    int projectile_remove(Projectile tab[], int n);
    
#endif
