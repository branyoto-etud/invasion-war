#include "../include/Affichage.h"
#include "../include/Enemy.h"

#include <stdio.h>
#include <math.h>

/* Initialise les valeurs de 'enemy'
 * param 'enemy' : Pointeur de la structure à initialiser
 * param 'type' : Type de l'ennemi
 * param 'x' : Abscisse du centre de l'ennemi
 * param 'y' : Ordonnée du centre de l'ennemi
 * param 'w' : Largeur de l'ennemi
 * param 'h' : Hauteur de l'ennemi
 * param 'speed' : Vitesse de l'ennemi
 */
void init_enemy(Enemy *enemy, EnemyType type, Pattern pattern, int x, int y, int w, int h, int speed) {
    enemy->pv = enemy_pv(type);
    enemy->x = x - w / 2;
    enemy->y = y - h / 2;
    enemy->w = w;
    enemy->h = h;
    enemy->alive = 1;
    enemy->xstart = x - w / 2;
    enemy->ystart = y;
    enemy->speed = speed;
    enemy->internal_clock = 0;
    enemy->pattern = pattern;
    enemy->type = type;
    enemy->damaged = 0;
}

/* Permet de savoir quelle quantité de vie maximum est attribuée au type 'type'
 * param 'typa' : Type dont on veut connaitre le nombre de points de vie
 * retour : La quantité de vie associé à 'type'
 *          0 si il n'a pas de valeur connue
 */
int enemy_pv(EnemyType type) {
    switch (type) {
        case E1: return 300;
        case E2: return 400;
        case E3: return 500;
        case E4: return 800;
        case Boss1: return BOSS_HP * 1.0;
        case Boss2: return BOSS_HP * 1.7;
        case Boss3: return BOSS_HP * 2.4;
        case Boss4: return BOSS_HP * 3.1;
        case Boss5: return BOSS_HP * 3.8;
        case Special: return 1000;
        default: return 0;
    }
}

/* Fonction cubique servant au pattern ReverseU et NReverseU
 * param 'x' : Abscisse passé ç la fonction cubique
 * retour : L'ordonnée correspondante à 'x'
 */
double f_x(double x) {
    return 0.00004 * pow(x, 3) - 0.01 * pow(x, 2) - 0.3 * x;
}

/* Demande si le boss 'boss' doit tirer
 * param 'boss' : boss à faire tirer ou non
 * retour : 1 si l'ennemi doit tirer
 *          0 s'il ne doit pas tirer
 */
int boss_shooting(Enemy* boss) {
    int amount = 0;

    if (3 * boss->pv <= enemy_pv(boss->type) / 4) {
        amount += 1;
    }
    if (2 * boss->pv <= enemy_pv(boss->type) / 4) {
        amount += 1;
    }
    if (1 * boss->pv <= enemy_pv(boss->type) / 4) {
        amount += 1;
    }
    switch (boss->type) {
        case Boss1: amount = 60.0 * (16 - amount) / 16; break;
        case Boss2: amount = 55.0 * (16 - amount) / 16; break;
        case Boss3: amount = 50.0 * (16 - amount) / 16; break;
        case Boss4: amount = 45.0 * (16 - amount) / 16; break;
        case Boss5: amount = 40.0 * (16 - amount) / 16; break;
        default: return 0;
    }
    if (boss->internal_clock % amount == amount-1) {
        return MLV_get_random_integer(0, 5) == 1;
    }
    return 0;
}

/* Demande si l'ennemi 'enemy' doit tirer
 * param 'enemy' : ennemi à faire tirer ou non
 * retour : 1 si l'ennemi doit tirer
 *          0 s'il ne doit pas tirer
 */
int enemy_shooting(Enemy *enemy){
    int ret;
    /* Pas de nouvelle attaque si le boss dash */
    if (enemy->speed != -1 && Boss1 <= enemy->type && enemy->type <= Boss5) {
        return 0;
    }
    
    switch (enemy->type) {
        case E1:   ret = enemy->internal_clock % 100 ==  99; break;
        case E2:   ret = enemy->internal_clock % 125 == 124; break;
        case E3:   ret = enemy->internal_clock % 130 == 129; break;
        case E4:   ret = enemy->internal_clock %  75 ==  74; break;
        case Boss1:
        case Boss2:
        case Boss3:
        case Boss4:
        case Boss5:
            return boss_shooting(enemy);
        case Special: return 0;
    }
    if (ret == 1) {
        return MLV_get_random_integer(0, 5) == 1;
    }
    return 0;
}

/* Fait foncer le boss vers le bas de l'écran
 * param 'boss' : Boss à faire foncer
 */
void boss_dash_update(Enemy* boss) {
    int A, B, C;
    /* A+B+C = BOSS_DASH_VALUE */
    A = 80;
    B = 30;
    C = 48;
    /* Si le boss n'est pas entièrement sur l'écran */
    if (boss->internal_clock < 200) {
        boss->speed = -1;
        return;
    }
    
    boss->speed -= 1;
    if (boss->speed > BOSS_DASH_VALUE - A) {                    /* Recule */
        boss->y -= 2;
    } else if (boss->speed > BOSS_DASH_VALUE - A - B) {         /* Dash */
        boss->y += 26;
    } else if (boss->speed > BOSS_DASH_VALUE - A - B - C) {     /* Repositionnement */
        boss->y -= 13;
    }
}

/* Met à jour la position de l'ennemi 'e' en suivant son pattern
 * param 'e' : Ennemi à mettre à jour
 * param 'para' : Structure contenant les paramètres du jeu
 * retour : 0 si l'ennemi meurt de la bordure
 *          1 sinon
 */
int enemy_update(Enemy* e, Parametre *para) {
    int amount = e->speed;
    int tmp1, tmp2;
    
    if (!e->alive) {
        e->y--;
        e->x += (e->internal_clock) % 2 ? -5 : 5;
        return 1;
    }
    
    /* 1 frame sur 2 */
    if (e->internal_clock % 2) {
        return 1;
    }
    
    if (e->speed >= 0 && Boss1 <= e->type && e->type <= Boss5) {
        boss_dash_update(e);
        if (e->speed == -1) {
            e->xstart = e->x - e->internal_clock;
        }
        return 1;
    }
    
    switch (e->pattern) {
        case Linear:
            e->x = e->x;
            e->y += amount;
            break;
        case Sin:
            e->y = e->y + amount;
            e->x = e->xstart + 100.0*sin((double)e->y/50.0);
            break;
        case NSin:
            e->y = e->y + amount;
            e->x = e->xstart - 100.0*sin((double)e->y/50.0);
            break;
        case ReverseU:
            e->x = e->x + amount;
            e->y = f_x(e->x+e->w/2) + e->ystart - e->h/2;
            break;
        case NReverseU:
            e->x = e->x - amount;
            e->y = f_x(-e->x+para->width-e->w/2) + e->ystart - e->h/2;
            break;
        case Sidetodown:
            if (e->x + e->w/2 < para->width/2-70) e->x += amount;
            else e->y += amount;
            break;
        case NSidetodown:
            if (e->x + e->w/2 > para->width/2+70) e->x -= amount;
            else e->y += amount;
            break;
        case Custom:    /* Pattern de Boss (bouncing) */
            tmp1 = e->xstart + e->internal_clock;
            tmp2 = para->width - e->w;
            e->y = e->y + (e->y < e->h/4 ? 3 : 0);
            if ( tmp1 % (2 * tmp2) < tmp2) {
                e->x = tmp1 % tmp2;
            } else {
                e->x = tmp2 - (tmp1 % tmp2);
            }
            break;
    }
    /* Si l'ennemi sort de l'écran ==> Mort */
    if (e->y > para->height) {
        e->alive = 0;
        return 0;
    }
    
    return 1;
}

/* Echange la valeur du pointeur 'a' avec la valeur du pointeur 'b'
 * param 'a' : Premier Enemy à échanger
 * param 'b' : Second Enemy à échanger
 */
static void swap(Enemy* a, Enemy* b) {
    Enemy t = *a;
    *a = *b;
    *b = t;
}

/* Parcours le tableau 'tab' de 'n' elements et décale les ennemis morts à la fin du tableau
 * param 'tab : Tableau d'Enemy
 * param 'n' : Nombre d'ennemi contenus dans le tableau
 * retour : la taille du tableau après suppression des ennemis morts
 */
int enemy_remove(Enemy tab[], int n) {
    int i, j;
    
    /* Ne peut pas être inférieur à 0 */
    if (n <= 0)
        return 0;
    /* S'il y en a qu'un on renvoie 1 s'il est en vie et 0 s'il est mort (ou p->alive c'est la meme)*/
    if (n == 1)
        return tab[0].alive || (tab[0].internal_clock * FRAME_RATE / DEAD_FRAME) <= 20;
    
    /* Parcours le tableau */
    for (i=0,j=0; i < n && j < n; i++, j++) {
        /* Si l'élément courant est à supprimer */
        if (!tab[i].alive && (tab[i].internal_clock * FRAME_RATE / DEAD_FRAME) > 20) {
            /* Si l'élément ciblé est aussi à supprimé on augmente l'écartement entre i et j */
            if (!tab[j].alive && (tab[j].internal_clock * FRAME_RATE / DEAD_FRAME) > 20) {i--;}
            /* Sinon on échange les deux */
            else {swap(tab+i,tab+j);}
        }
    }
    
    /* On renvoie la nouvelle taille du tableau */
    return n - (j-i);
}
