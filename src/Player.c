#include "../include/Player.h"

#include <stdio.h>

/* Initialise le joueur 'player' avec les coordonnées ('x', 'y') et les dimentions 'w' et 'h' */
void init_player(Player *player, int x, int y, int w, int h) {
    player->x = x;
    player->y = y;
    player->w = w;
    player->h = h;
    player->money = 0;
    player->shield = 0;
    player->charging = 0;
    player->b_cooldown = 0;
    player->m_cooldown = 0;
    player->invincible = 0;
    player->pv = MAX_HEALTH;
    player->recovery_time = 0;
    player->nb_missiles = MAX_MISSILES;
    player->internal_clock = 0;
    player->pv_max = MAX_HEALTH;
    player->perma_bonus = Single_shot;
    player->stock_bonus = Aucun;
    player->vx = player->vy = 0;
    player->ax = player->ay = 0;
}

/* Met à jours la position du joueur en fonction de sa vitesse et de
 * son acceleration (garde l'inertie) */
void player_update(Player *player, Parametre *para) {
     
    player->ax /= 2.0;
    player->ay /= 2.0;
    
    player->vx += player->ax;
    player->vy += player->ay;
    
    /* Frictions */
    player->vx *= .99;
    player->vy *= .99;
    
    /* Vitesse limit */
    if (player->vx > MAX_SPEED) {
        player->vx = MAX_SPEED;
    } else if (player->vx < -MAX_SPEED) {
        player->vx = -MAX_SPEED;
    }
    if (player->vy > MAX_SPEED) {
        player->vy = MAX_SPEED;
    } else if (player->vy < -MAX_SPEED) {
        player->vy = -MAX_SPEED;
    }
    
    player->x += player->vx;
    player->y += player->vy;
    
    /* Out of Bound => Repositionnement */
    if (para->width < player->x + player->w) {
        player->x = para->width - player->w;
    } else if (player->x < 0) {
        player->x = 0;
    }
    if (para->height < player->y + player->h) {
        player->y = para->height - player->h;
    } else if (player->y < 0) {
        player->y = 0;
    }
    
    /* Gestion du cooldown */
    if (player->b_cooldown > 0) {
        player->b_cooldown--;
    }
    if (player->m_cooldown > 0 ) {
        player->m_cooldown--;
    }
    if (player->recovery_time > 0) {
        player->recovery_time--;
    }
}

void move_player(Player *p, int key_hist[], Parametre *para) {
    /* Deplace le joueur en fonction des touches appuyées */
    int amount = 4, n = 0;
    
    /* Pas de else sinon pas de mvt en diagonal */
    /* 'n' permet de ne pas prioriser une direction. Si gauche et droite sont appuyés on n'accelere pas. */
    if (key_hist[para->prim_key[Up_key]] || key_hist[para->sec_key[Up_key]]) {
        n -= amount;
    }
    if (key_hist[para->prim_key[Down_key]] || key_hist[para->sec_key[Down_key]]) {
        n += amount;
    }
    p->ay = n;
    n = 0;
    if (key_hist[para->prim_key[Left_key]] || key_hist[para->sec_key[Left_key]]) {
        n -= amount;
    }
    if (key_hist[para->prim_key[Right_key]] || key_hist[para->sec_key[Right_key]]) {
        n += amount;
    }
    p->ax = n;
}

/* Donne l'effet Power au joueur */
void apply_bonus(Player *player, Power power) {
    switch (power) {
        case Missile :
            player->nb_missiles ++;
            if (player->nb_missiles > MAX_MISSILES) {
                player->nb_missiles = MAX_MISSILES;
                player->m_cooldown = 0;
            }
            break;
        case Heal :
            player->pv += HEAL_AMOUNT;
            if (player->pv > player->pv_max) {
                player->pv = player->pv_max;
            }
            break;
        case Health_boost :
            player->pv_max += HEALTH_BOOST;
            if (player->pv_max > MAX_HEALTH_BOOST) {
                player->pv_max = MAX_HEALTH_BOOST;
            }
            break;
        case Shield :
            player->shield = SHIELD_AMOUNT; break;
        case Single_shot :     
            player->perma_bonus = Single_shot; break;
        case Dual_shot :       
            player->perma_bonus = Dual_shot;   break;
        case Triple_shot :     
            player->perma_bonus = Triple_shot; break;
        case Quadra_shot :     
            player->perma_bonus = Quadra_shot; break;
        default:
            break;
    }
}

/* Applique 'amount' point de dégats au joueur 
 * Renvoie 1 si le joueur est mort
 *         0 sinon */
int damage_player(Player* player, int amount) {
    if (player->invincible)
        return 0;
    
    if (player->shield != 0) {
        player->recovery_time = 0;
        player->shield -= amount;
        player->shield = player->shield <= 0 ? 0 : player->shield;
    } else {
        player->recovery_time = RECOVERY_TIME;
        player->pv -= amount;
        player->pv = player->pv < 0 ? 0 : player->pv;
    }
    return player->pv == 0;
}

/* Renvoie 1 si le joueur est mort 0 s'il est en vie */
int player_dead(Player p) {return (p.pv <= 0) && !p.invincible;}

