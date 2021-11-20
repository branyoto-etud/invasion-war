#include "../include/Projectile.h"
#include <math.h>

/* Temporaire */
#include <stdio.h>

/* Initialise le projectile 'p' avec le type 'type' les coordonnées 'x' et 'y', les dimentions 'w' et 'h' et les dégats 'damage' */
void init_projectile(Projectile* p, ProjectileType type, int x, int y, int w, int h, int damage, int dx, int dy, int speed) {
    p->speed = speed;
    p->alive = 1;
    p->x = x;
    p->y = y;
    p->w = w;
    p->h = h;
    p->dx = type==EBlaster_sin ? x : dx;
    p->dy = type==EBlaster_sin ? y : dy;
    p->damage = damage / (type == PBlaster_Charged ? CHARGING_RATIO : 1);
    p->type = type;
    p->internal_clock = 0;
}

/* Met à jours la direction du projectile 'p' pour qu'il cible le joueur 'p' avec une vitesse de 'speed' */
void update_projectile_direction(Projectile* p, int px, int py, int speed) {
    int dx, dy;
    double h, ratio;
    
    /* Calcul de l'hypoténuse */
    dx = p->x - px;
    dy = p->y - py;
    h = sqrt(dx*dx + dy*dy);
    
    /* Anti division par zero */
    if (h == 0) 
        ratio = 0;
    else
        ratio = speed/h;
    
    /* Met à jour la direction */
    p->dx = -1.0 * dx * ratio;
    p->dy = -1.0 * dy * ratio;
}

/* Se déplace de p->speed pixel en direction du joueur (px,py) étant le centre du joueur */
void move_to_player(Projectile *p, int px, int py) {
    int dx, dy;
    double h, ratio;
    
    /* Calcul de l'hypoténuse */
    dx = p->x - px;
    dy = p->y - py;
    h = sqrt(dx*dx + dy*dy);
    
    /* Anti division par zero (ne devrai jamais etre possible vu que la colision à lieux avant """normalement""" */
    if (h == 0) 
        ratio = 0;
    else
        ratio = p->speed/(h * 2);
    
    /* Déplace le projectile */
    p->x -= dx * ratio;
    p->y -= dy * ratio;
    
}

/* Fonction nescessaire à projectile_remove() échange les valeurs des projectiles a et b */
static void swap(Projectile* a, Projectile* b) {
    Projectile t = *a;
    *a = *b;
    *b = t;
}

/* Parcours le tableau 'tab' de taille n et retire les projectiles "mort"
 * Renvoie la nouvelle taille de tab */
int projectile_remove(Projectile tab[], int n) {
    int i, j;
    
    /* Ne peut pas être inférieur à 0 */
    if (n <= 0)
        return 0;
    /* S'il y en a qu'un on renvoie 1 s'il est en vie et 0 s'il est mort (ou p->alive c'est la meme)*/
    if (n == 1)
        return tab[0].alive || tab[0].internal_clock <= AFTERLIFE_TIME;
    
    /* Parcours le tableau */
    for (i=0,j=0; i < n && j < n; i++, j++) {
        /* Si l'élément courant est à supprimer */
        if (!tab[i].alive && tab[i].internal_clock > AFTERLIFE_TIME) {
            /* Si l'élément ciblé est aussi à supprimé on augmente l'écartement entre i et j */
            if (!tab[j].alive && tab[j].internal_clock > AFTERLIFE_TIME) {i--;}
            /* Sinon on échange les deux */
            else {swap(tab+i,tab+j);}
        }
    }
    /* On renvoie la nouvelle taille du tableau */
    return n - (j-i);
}

/* Met à jour la position du projectile en fonction de son type
 * Renvoie 1 si il est en vie 0 s'il est mort */
int projectile_update(Projectile *p, Parametre* para, int px, int py) {
    
    if (!p->alive) {
        return 0;
    }
    
    switch (p->type) {
        /* Pour les trois suivant on les fait aller vers le haut de l'écran */
        case PBlaster:
        case PMissile:
        case PBlaster_Charged:
            p->x = p->x;
            p->y = p->y - p->speed;
            break;
        /* On le fait se déplacer dans sa direction */
        case BBlaster:
        case EBlaster:
            p->x += p->dx;
            p->y += p->dy;
            break;
        /* On le fait aller vers le bas de l'écran en oscillant */
        case EBlaster_sin:
            p->y = p->y + p->speed;
            p->x = p->dx + 50.0*sin((double)p->y/30.0);
            break;
        /* On le fait se déplacer vers le joueur */
        case BMissile_auto:
        case EMissile_auto:
            move_to_player(p, px, py);
            break;
        default:
            break;
    }
    /* On les tuent quand ils se sont déplacé de plus de la moitié de l'écran */
    /* (Pas grave si on les gardent hors de l'écran pendant un petit moment) */
    if ((p->type == BMissile_auto || p->type == EMissile_auto) && p->internal_clock > para->height/4) {
        p->alive = 0;
        p->internal_clock = 0;
        return 0;
    }
    if (p->internal_clock > para->height/2) {
        p->alive = 0;
        p->internal_clock = 0;
        return 0;
    }
    
    return 1;
}
