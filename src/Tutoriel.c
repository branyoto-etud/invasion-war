#define _POSIX_C_SOURCE 199309L
#include <time.h>
#include <stdio.h>
#include <MLV/MLV_all.h>

#include "../include/Affichage.h"
#include "../include/Tutoriel.h"
#include "../include/Terrain.h"
#include "../include/Player.h"
#include "../include/Partie.h"
#include "../include/Enemy.h"
#include "../include/Menu.h"

#define NB_FONTS (40)
#define CENTER MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER

/* Fait tirer dans le tuto */
static void tuto_blaster(Terrain *terrain, Parametre *para, int key_hist[]) {
    if (terrain->player->b_cooldown == 0) {
        if ((key_hist[para->prim_key[Blaster_key]] || key_hist[para->sec_key[Blaster_key]] ) ) {
            blaster(terrain, para);
            terrain->player->b_cooldown = B_COOLDOWN;
        }
    }
}
/* Fait tirer un missile dans le tuto */
static void tuto_missile(Terrain *terrain, Parametre *para, int key_hist[]) {
    Player* p = terrain->player;
    if (terrain->player->m_cooldown == 0) {
        if ((key_hist[para->prim_key[Missile_key]] || key_hist[para->sec_key[Missile_key]] ) ) {
            add_projectile(terrain, PMissile, p->x + p->w/2, p->y, para->sizes.missile_width, para->sizes.missile_height, 10000, 0 ,-1, 5);
            p->nb_missiles --;
            terrain->player->m_cooldown = M_COOLDOWN;
        }
    }
}
/* Fait charger et tirer dans le tuto */
static void tuto_charged(Terrain *terrain, Parametre *para, int key_hist[]) {
    if (key_hist[para->prim_key[Charged_key]] || key_hist[para->sec_key[Charged_key]] ) {
        terrain->player->charging = terrain->player->charging+1 > MAX_CHARGING ? MAX_CHARGING : terrain->player->charging+1;
    } else if (terrain->player->charging >= MIN_CHARGING) {
        int tmp_w, tmp_h;
        get_charged_size(&(para->sizes), terrain->player->charging, &tmp_w, &tmp_h);
        add_projectile(terrain, PBlaster_Charged, terrain->player->x + terrain->player->w/2, terrain->player->y, tmp_w, tmp_h, 1000, 0, -1, 10);
        terrain->player->charging = 0;
    } else {
        terrain->player->charging = 0;
    }
}
/* Fait apparaitre la liste des différents ennemis */
static void summon_list(Terrain *terrain, Parametre* para) {
    int w, h;
    w = para->sizes.enemy1_width; h = para->sizes.enemy1_height;
    add_enemy(terrain, E1, 0, 1*para->width / 5, para->height/2, w, h, 0);
    w = para->sizes.enemy2_width; h = para->sizes.enemy2_height;
    add_enemy(terrain, E2, 0, 2*para->width / 5, para->height/2, w, h, 0);
    w = para->sizes.enemy3_width; h = para->sizes.enemy3_height;
    add_enemy(terrain, E3, 0, 3*para->width / 5, para->height/2, w, h, 0);
    w = para->sizes.enemy4_width; h = para->sizes.enemy4_height;
    add_enemy(terrain, E4, 0, 4*para->width / 5, para->height/2, w, h, 0);
}
/* Fait apparaitre une ligne verticale d'ennemis 1 */
static void summon_line(Terrain *terrain, Parametre* para) {
    int w, h;
    w = para->sizes.enemy1_width; h = para->sizes.enemy1_height;
    add_enemy(terrain, E1, 0, para->width / 2, para->height/10+h*0, w, h, 0);
    add_enemy(terrain, E1, 0, para->width / 2, para->height/10+h*1, w, h, 0);
    add_enemy(terrain, E1, 0, para->width / 2, para->height/10+h*2, w, h, 0);
    add_enemy(terrain, E1, 0, para->width / 2, para->height/10+h*3, w, h, 0);
    add_enemy(terrain, E1, 0, para->width / 2, para->height/10+h*4, w, h, 0);
}
/* Fait apparaitre un groupe d'ennemis */
static void summon_group(Terrain *terrain, Parametre *para) {
    int w, h;
    w = para->sizes.enemy1_width; h = para->sizes.enemy1_height;
    add_enemy(terrain, E1, 0, para->width / 2 + w*0, para->height/10+h*0, w, h, 0);
    add_enemy(terrain, E1, 0, para->width / 2 + w*1, para->height/10+h*1, w, h, 0);
    add_enemy(terrain, E1, 0, para->width / 2 + w*0, para->height/10+h*1, w, h, 0);
    add_enemy(terrain, E1, 0, para->width / 2 - w*1, para->height/10+h*1, w, h, 0);
    add_enemy(terrain, E1, 0, para->width / 2 + w*1, para->height/10+h*2, w, h, 0);
    add_enemy(terrain, E1, 0, para->width / 2 + w*2, para->height/10+h*2, w, h, 0);
    add_enemy(terrain, E1, 0, para->width / 2 - w*1, para->height/10+h*2, w, h, 0);
    add_enemy(terrain, E1, 0, para->width / 2 - w*2, para->height/10+h*2, w, h, 0);
    add_enemy(terrain, E1, 0, para->width / 2 + w*1, para->height/10+h*3, w, h, 0);
    add_enemy(terrain, E1, 0, para->width / 2 - w*1, para->height/10+h*3, w, h, 0);
}
/* Fait tirer les ennemis en fonction du moment atteint dans le tuto */
static int tuto_shooting(Enemy *e, int moment) {
    if (moment == 10 && e->type == E1) {return 1;}
    if (moment == 11 && e->type == E2) {return 1;}
    if (moment == 12 && e->type == E3) {return 1;}
    if (moment == 13 && e->type == E4) {return 1;}
    return 0;
}
/* Met à jours le player, les ennemis et les projectiles */
static void tuto_update(Terrain *terrain, Data* data, int moment) {
    Player *p = terrain->player;
    int i;
    
    player_update(p, &(data->para));

    for (i = terrain->nb_projectiles-1; i >= 0; i--) {
        projectile_update(&(terrain->projectiles[i]), &(data->para), p->x + p->w / 2, p->y + p->h / 2);
    }
    for (i = terrain->nb_enemies-1; i >= 0; i--) {
        if (terrain->nb_projectiles == 0 && tuto_shooting(&(terrain->enemies[i]), moment)) {
            enemy_shoot(terrain, &(data->para), &(terrain->enemies[i]));
            if (i == 2) {
                terrain->projectiles[0].internal_clock = 1*data->para.height/6;
            } else {
                terrain->projectiles[0].internal_clock = 2*data->para.height/6;
            }
        }
    }
}
/* Dessine un bouton dans le tuto */
static void draw_button(MLV_Image* button, int x, int y, char* txt, MLV_Font* font) {
    int w=0, h=0;
    MLV_draw_image(button,x, y);
    if (txt == NULL) return;
    MLV_get_size_of_text_with_font(txt, &w, &h, font);
    MLV_draw_text_with_font(x + 28 - w/2, y + 28 - h/2, txt, font, MLV_COLOR_WHITE);
}
/* Affiche les différentes étapes du tuto */
static void aff_tuto_moment(Terrain* terrain, Data* data, MLV_Font *font_array[], int moment, Power bonus) {
    static int cooldown;
    MLV_Font* font;
    int w, h;
    w = data->para.width;
    h = data->para.height;
    
    font = MLV_load_font("data/fonts/cyberlink.ttf", 25);
    
    if (moment > 20) {
        int tmp=0;
        draw_shop_menu(terrain, &(data->para), &(data->img_list), bonus, terrain->scroll, &tmp);
    } else {
        draw_window_noupdate(&(data->para), terrain, &(data->img_list), 0);
    }
    
    if (moment == 0) {
        MLV_draw_text_box_with_font(20, 50, w-40, 300,
                "Te voici dans \nle TUTORIEL", font_array[20], 1,
                MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_rgba(0,0,0,170), CENTER);
        draw_centered_text(w/2, 2*h/5,              "Appuie sur Entrer pour continuer", font_array[18], MLV_COLOR_WHITE);
        draw_centered_text(w/2, 2*h/5+30,           "Appuie sur Echap pour quitter", font_array[18], MLV_COLOR_WHITE);
        draw_centered_text(w/2, 4*h/5+30,           "Ca c'est toi", font_array[18], MLV_COLOR_WHITE);
        MLV_draw_circle(terrain->player->x+terrain->player->w/2, terrain->player->y+terrain->player->h/2, terrain->player->w+10, MLV_rgba(255, 0, 10, 255));
    } else if (moment == 1) {
        draw_button(data->img_list.button,       w/5,  h/2-3, "z", font);
        draw_button(data->img_list.button,       w/5, h/2+64, "s", font);
        draw_button(data->img_list.button,    w/5-64, h/2+64, "q", font);
        draw_button(data->img_list.button,    w/5+64, h/2+64, "d", font);
        draw_button(    data->img_list.up,    7*w/10,  h/2-3,  "", font);
        draw_button(  data->img_list.down,    7*w/10, h/2+64,  "", font);
        draw_button(  data->img_list.left, 7*w/10-64, h/2+64,  "", font);
        draw_button( data->img_list.right, 7*w/10+64, h/2+64,  "", font);
        draw_centered_text(w/2, 2*h/5,              "Essaie de te deplacer en utilisant", font_array[18], MLV_COLOR_WHITE);
        draw_centered_text(w/2, 2*h/5+20,           "les touches de deplacement", font_array[18], MLV_COLOR_WHITE);
    } else if (moment == 2) {
        draw_centered_text(w/2, 2*h/5,              "Felicitation! Maintenant appuie", font_array[18], MLV_COLOR_WHITE);
        draw_centered_text(w/2, 2*h/5+20,           "sur la touche de tir", font_array[18], MLV_COLOR_WHITE);
        draw_button(data->img_list.button,    w/5+64, h/2+64, "1", font);
        draw_button( data->img_list.space, 7*w/10-64, h/2+64,  "", font);
    } else if (moment == 3) {
        draw_centered_text(w/2, 2*h/5,              "Bravo! Tu peux aussi maintenir la touche", font_array[18], MLV_COLOR_WHITE);
        draw_centered_text(w/2, 2*h/5+20,           "de tir appuyee pour tirer en continu", font_array[18], MLV_COLOR_WHITE);
        draw_button(data->img_list.button,    w/5+64, h/2+64, "1", font);
        draw_button( data->img_list.space, 7*w/10-64, h/2+64,  "", font);
    } else if (moment == 3) {
    } else if (moment == 4) {
        draw_centered_text(w/2, 2*h/5,              "Je vois que tu es capable de maintenir", font_array[18], MLV_COLOR_WHITE);
        draw_centered_text(w/2, 2*h/5+20,           "une touche.", font_array[18], MLV_COLOR_WHITE);
        if (cooldown > 40) {
            draw_centered_text(w/2, 2*h/5+40,       "Essaie maintenant avec le tir charge", font_array[18], MLV_COLOR_WHITE);
        draw_button(data->img_list.button,    w/5+64, h/2+64, "2", font);
        draw_button(data->img_list.button, 7*w/10-64, h/2+64, "c", font);
        } else {
            cooldown++;
        }
    } else if (moment == 5) {
        draw_centered_text(3*w/10, 13*h/15,          "Tu vois le cercle qui se remplit ?", font_array[10], MLV_COLOR_WHITE);
        draw_centered_text(3*w/10, 13*h/15+20,       "C'est le chargement du tir charge", font_array[10], MLV_COLOR_WHITE);
        draw_centered_text(3*w/10, 13*h/15+40,       "Plus tu le charges,", font_array[10], MLV_COLOR_WHITE);
        draw_centered_text(3*w/10, 13*h/15+60,       "plus il fera de degats", font_array[10], MLV_COLOR_WHITE);
    } else if (moment == 6) {
        draw_centered_text(3*w/10, 14*h/15,          "Tu peux le relacher a partir", font_array[10], MLV_COLOR_WHITE);
        draw_centered_text(3*w/10, 14*h/15+20,       "du moment ou le cercle clignote", font_array[10], MLV_COLOR_WHITE);
    } else if (moment == 7) {
        draw_centered_text(w/2, 2*h/5,               "Le tir charge est puissant", font_array[18], MLV_COLOR_WHITE);
        draw_centered_text(w/2, 2*h/5+20,            "il peut traverser les ennemis et", font_array[18], MLV_COLOR_WHITE);
        draw_centered_text(w/2, 2*h/5+40,            "fait beaucoup de degats", font_array[18], MLV_COLOR_WHITE);
    } else if (moment == 8) {
        if (terrain->nb_projectiles == 0) {
            draw_centered_text(w/2, 2*h/5,           "Cependant le tir le plus puissant", font_array[18], MLV_COLOR_WHITE);
            draw_centered_text(w/2, 2*h/5+20,        "est le missile essaie d'en tirer un", font_array[18], MLV_COLOR_WHITE);
            draw_button(data->img_list.button,    w/5+64, h/2+64, "3", font);
            draw_button(data->img_list.button, 7*w/10-64, h/2+64, "v", font);
        } else {
            draw_centered_text(w/2, 2*h/5,           "Il est tres utile contre les groupes d'ennemis", font_array[13], MLV_COLOR_WHITE);
            draw_centered_text(w/2, 2*h/5+20,        "et surpuissant contre les boss", font_array[13], MLV_COLOR_WHITE);
        }
    } else if (moment == 9) {
        draw_centered_text(w/2, h/5,                 "Tiens parlons des ennemis.", font_array[18], MLV_COLOR_WHITE);
        if (cooldown > 50) {
            draw_centered_text(w/2, h/5+20,          "Voici les 4 types d'ennemis principaux", font_array[18], MLV_COLOR_WHITE);
            draw_centered_text(w/2, h/5+40,          "que tu rencontreras", font_array[18], MLV_COLOR_WHITE);
            draw_centered_text(w/2, h/5+60,          "Il faudra esquiver leurs tirs et les", font_array[18], MLV_COLOR_WHITE);
            draw_centered_text(w/2, h/5+80,          "tuer avant qu'ils n'atteignent le bas", font_array[18], MLV_COLOR_WHITE);
        } else {cooldown++;}
    } else if (moment == 10) {
        draw_centered_text(1*w/5, 3*h/10,            "Le premier est basique", font_array[10], MLV_COLOR_WHITE);
        draw_centered_text(1*w/5, 3*h/10+20,         "et tire tout droit", font_array[10], MLV_COLOR_WHITE);
    } else if (moment == 11) {
        draw_centered_text(2*w/5, 3*h/10,            "Le second tire de", font_array[10], MLV_COLOR_WHITE);
        draw_centered_text(2*w/5, 3*h/10+20,         "facon sinusoidale.", font_array[10], MLV_COLOR_WHITE);
        draw_centered_text(2*w/5, 3*h/10+40,         "Mieux mais pas ouf", font_array[10], MLV_COLOR_WHITE);
    } else if (moment == 12) {
        draw_centered_text(3*w/5, 3*h/10,            "Le troisieme tire un", font_array[10], MLV_COLOR_WHITE);
        draw_centered_text(3*w/5, 3*h/10+20,         "missile auto-guide.", font_array[10], MLV_COLOR_WHITE);
        draw_centered_text(3*w/5, 3*h/10+40,         "Lent mais ne te fais pas", font_array[10], MLV_COLOR_WHITE);
        draw_centered_text(3*w/5, 3*h/10+60,         "toucher si tu tiens a la vie", font_array[10], MLV_COLOR_WHITE);
    } else if (moment == 13) {
        if (terrain->nb_enemies > 4) {
            draw_centered_text(4*w/5, 3*h/10,       "SURPRISE! Il donne naissance", font_array[10], MLV_COLOR_WHITE);
            draw_centered_text(4*w/5, 3*h/10+20,    "a deux ennemis", font_array[10], MLV_COLOR_WHITE);
        } else {
            draw_centered_text(4*w/5, 3*h/10,       "Le dernier te vise", font_array[10], MLV_COLOR_WHITE);
            draw_centered_text(4*w/5, 3*h/10+20,    "Enfin essaie tout du moins", font_array[10], MLV_COLOR_WHITE);
            draw_centered_text(4*w/5, 3*h/10+40,    "Et lorsqu'il meurt...", font_array[10], MLV_COLOR_WHITE);
        }
    } else if (moment == 14) {
        draw_centered_text(w/2, h/5,                 "Il y a aussi un ennemi special qui apparait", font_array[18], MLV_COLOR_WHITE);
        draw_centered_text(w/2, h/5+20,              "parfois a la place d'un autre ennemi.", font_array[18], MLV_COLOR_WHITE);
        draw_centered_text(w/2, h/5+40,              "Il est plus dur a tuer mais rapporte", font_array[18], MLV_COLOR_WHITE);
        draw_centered_text(w/2, h/5+60,              "aussi plus de points et une surprise", font_array[18], MLV_COLOR_WHITE);
    } else if (moment == 15) {
        draw_centered_text(w/5, 9*h/10,           "Ici tu as le nombre de tirs", font_array[10], MLV_COLOR_WHITE);
        draw_centered_text(w/10, 9*h/10+20,          "que tu tires", font_array[10], MLV_COLOR_WHITE);
        draw_arrow(w/60+3, 14*h/15, w/30, h/15, MLV_COLOR_WHITE, 1);
    } else if (moment == 16) {
        if (cooldown > 40) {
            draw_centered_text(w/2, 2*h/5,           "Reessaie de tirer maintenant", font_array[18], MLV_COLOR_WHITE);
        } else {
            cooldown++;
            terrain->player->perma_bonus = Quadra_shot;
            draw_centered_text(w/2+50, 3*h/4-cooldown*2, "+3 tirs", font_array[18], MLV_rgba(255, 255, 255, 255-6*cooldown));
        }
    } else if (moment == 17) {
        draw_centered_text(w/5, 9*h/10,            "Ici tu as donc le", font_array[10], MLV_COLOR_WHITE);
        draw_centered_text(w/5, 9*h/10+20,         "chargement du tir charge", font_array[10], MLV_COLOR_WHITE);
        draw_centered_text(2*w/5, 29*h/30,         "Ici ton nombre de points", font_array[10], MLV_COLOR_WHITE);
    } else if (moment == 18) {
        draw_centered_text(w/2, 29*h/30,           "Ici la barre de vie", font_array[10], MLV_COLOR_WHITE);
        draw_centered_text(w/2, 29*h/30+20,        "Et le bouclier est en bleu", font_array[10], MLV_COLOR_WHITE);
    } else if (moment == 19) {
        draw_centered_text(5*w/6, 29*h/30,        "Et enfin les missiles", font_array[10], MLV_COLOR_WHITE);
    } else if (moment == 20) {
        draw_centered_text(w/2, 2*h/5,             "Entre chaque vague d'ennemis tu pourras acheter", font_array[13], MLV_COLOR_WHITE);
        draw_centered_text(w/2, 2*h/5+20,          "des power-up dans le magasin", font_array[13], MLV_COLOR_WHITE);
    } else if (moment == 21) {
        draw_centered_text(w/2, h/10,              "Tu peux selectionner le bonus que tu veux acheter", font_array[12], MLV_COLOR_WHITE);
        draw_centered_text(w/2, h/10+20,           "avec la souris ou les touches du clavier", font_array[12], MLV_COLOR_WHITE);
    } else if (moment == 22) {
        draw_centered_text(w/2, h/10,              "Pour acheter un bonus il faut cliquer", font_array[14], MLV_COLOR_WHITE);
        draw_centered_text(w/2, h/10+20,           "sur \"Acheter\" ou appuyer sur la touche de tir", font_array[14], MLV_COLOR_WHITE);
    } else if (moment == 23) {
        draw_centered_text(w/2, h/10,              "Attends que je te donnes de l'argent faquin", font_array[13], MLV_COLOR_WHITE);
    } else if (moment == 24) {
        draw_centered_text(4*w/5, 2*h/5,           "Ici se trouve le bonus", font_array[10], MLV_COLOR_WHITE);
        draw_centered_text(4*w/5, 2*h/5+20,        "que tu achetes", font_array[10], MLV_COLOR_WHITE);
        draw_arrow(3*w/20, 19*h/20, w/30, h/15, MLV_COLOR_WHITE, 1);
        draw_centered_text(w/5, 28*h/30,           "Ici aussi", font_array[10], MLV_COLOR_WHITE);
        draw_turn_arrow(49*w/60, 9*h/20, w/10, h/30, MLV_COLOR_WHITE, 4);
        draw_centered_text(w/2, 3*h/4,             "Pour utiliser le bonus appuyer sur", font_array[18], MLV_COLOR_WHITE);
        draw_centered_text(w/2, 3*h/4+20,          "le bouton durant une partie", font_array[18], MLV_COLOR_WHITE);
        draw_button(data->img_list.button,    w/5+64, 3*h/4+64, "4", font);
        draw_button(data->img_list.button, 7*w/10-64, 3*h/4+64, "b", font);
    } else if (moment == 25) {
        draw_centered_text(w/2, h/10,              "Tu ne peux porter qu'un seul bonus a la fois", font_array[13], MLV_COLOR_WHITE);
        draw_centered_text(w/2, h/10+20,           "c'est pour ca que certain bonus comme le heal", font_array[13], MLV_COLOR_WHITE);
        draw_centered_text(w/2, h/10+40,           "ou le missile sont consommes a l'achat s'il te", font_array[13], MLV_COLOR_WHITE);
        draw_centered_text(w/2, h/10+60,           "manque de la vie ou des missiles", font_array[13], MLV_COLOR_WHITE);
    } else if (moment == 26) {
        draw_centered_text(w/2, h/10,              "Voila! Maintenant tu peux quitter le shop", font_array[13], MLV_COLOR_WHITE);
        draw_centered_text(w/2, h/10+20,           "(et le tuto) en appuyant sur la touche", font_array[13], MLV_COLOR_WHITE);
        draw_centered_text(w/2, h/10+40,           "de pause (Echap) ou \"Continuer\"", font_array[13], MLV_COLOR_WHITE);
    }
    
    MLV_free_font(font);
    MLV_actualise_window();
}
/* Gère les appuis au clavier durant le shop du tuto */
static void tuto_bonus_keyboard(Power* selected, int key_hist[], Parametre* para) {
    if (key_hist[para->prim_key[Right_key]] || key_hist[para->sec_key[Right_key]]) {
        if (*selected == Aucun) {
            *selected = NB_BONUS-1;
        } else if (*selected < NB_BONUS-1) {
            *selected+=1;
        }
    } else if (key_hist[para->prim_key[Left_key]] || key_hist[para->sec_key[Left_key]]) {
        if (*selected == Aucun) {
            *selected = 0;
        } else if (*selected > 0) {
            *selected-=1;
        }
    } else if (key_hist[para->prim_key[Up_key]] || key_hist[para->sec_key[Up_key]]) {
        if (*selected == Aucun) {
            *selected = 0;
        } else if ((int)*selected - NB_BONUS/2 < 0) {
            *selected = 0;
        } else {
            *selected -= NB_BONUS/2;
        }
    } else if (key_hist[para->prim_key[Down_key]] || key_hist[para->sec_key[Down_key]]) {
        if (*selected == Aucun) {
            *selected = NB_BONUS-1;
        } else if (*selected + NB_BONUS/2 >= NB_BONUS) {
            *selected = NB_BONUS-1;
        } else {
            *selected += NB_BONUS/2;
        }
    }
}
/* Renvoie 1 si m est une étape où l'on attends que l'utilisateur appuie sur un touche */
static int wait_key(int m) {
    return (m==0) || (m==9) || (m >= 10 && m <= 15) || ((m>=17) && (m<=20)) || (m==23) || (m==25);
}
/* Vide le terrain */
static void empty_terrain(Terrain* terrain) {
    terrain->nb_projectiles = 0;
    terrain->nb_enemies = 0;
    terrain->nb_bonus = 0;
}
/* Fait une action en fonction d'un evenement autre qu'une touche ou un clic de souris*/
static void action(Terrain *terrain, Parametre* para, int *moment) {
    int w, h;
    if (*moment == 13 && terrain->nb_enemies == 4) {
        enemy_death(terrain, terrain->enemies+3, para);
        (*moment)--;
    } else if (*moment == 13) {
        empty_terrain(terrain);
        w = para->sizes.enemy1_width; h = para->sizes.enemy1_height;
        add_enemy(terrain, Special, 0, para->width / 2, para->height/2, w, h, 0);
    } else if (*moment == 14) {
        empty_terrain(terrain);
    } else if (*moment == 15) {
        terrain->player->perma_bonus = Quadra_shot;
    } else if (*moment == 17) {
        terrain->player->shield = 30;
    }else if (*moment == 23) {
        terrain->player->money = 1000000;
        terrain->player->nb_missiles = 3;
    }
}

/* Met à jours le terrain en fonction des touches appuyées */
static void update_with_key(Terrain *terrain, Data *data, int key_hist[], int *moment, Power *selected) {
    static int tmp;
    
    if (wait_key(*moment) || (*moment == 13 && terrain->nb_enemies > 4)) {
        if (key_hist[MLV_KEYBOARD_RETURN] || key_hist[MLV_KEYBOARD_SPACE]) {
            action(terrain, &(data->para), moment);
            key_hist[MLV_KEYBOARD_RETURN] = 0;
            key_hist[MLV_KEYBOARD_SPACE] = 0;
            (*moment)++;
        }
    } else if (*moment == 1) {
        move_player(terrain->player, key_hist, &(data->para));
        tmp += abs(terrain->player->ax) + abs(terrain->player->ay);
        if (tmp > 500) {
            init_player(terrain->player, data->para.width/2-data->para.sizes.player_width/2, 9*data->para.height/10, data->para.sizes.player_width, data->para.sizes.player_height);
            (*moment)++;
            tmp = 0;
        }
    } else if (*moment == 2) {
        if (terrain->nb_projectiles == 0) {
            tuto_blaster(terrain, &(data->para), key_hist);
        } else if (terrain->projectiles[0].internal_clock > 20) {
            (*moment)++;
        }
    } else if (*moment == 3) {
        tuto_blaster(terrain, &(data->para), key_hist);
        if (terrain->nb_projectiles > 15) {
            terrain->nb_projectiles = 0;
            summon_line(terrain, &(data->para));
            (*moment)++;
        }
    } else if (*moment == 4) {
        if (terrain->player->charging < 5) {
            tuto_charged(terrain, &(data->para), key_hist);
        } else {
            (*moment)++;
        }
    } else if (*moment == 5) {
        if (terrain->player->charging < MIN_CHARGING) {
            tuto_charged(terrain, &(data->para), key_hist);
        } else {
            (*moment)++;
        }
    } else if (*moment == 6) {
        tuto_charged(terrain, &(data->para), key_hist);
    } else if (*moment == 8) {
        tuto_missile(terrain, &(data->para), key_hist);
    } else if (*moment == 16) {
        tuto_blaster(terrain, &(data->para), key_hist);
        key_hist[MLV_KEYBOARD_RETURN] = 0;
        key_hist[MLV_KEYBOARD_SPACE] = 0;
        if (terrain->nb_projectiles > 0) {
            (*moment)++;
        }
    } else if (*moment == 21) {
        tuto_bonus_keyboard(selected, key_hist, &(data->para));
        if (*selected != Aucun) {
            (*moment)++;
        }
    } else if (*moment == 22) {
        if (key_hist[data->para.prim_key[Blaster_key]] || key_hist[data->para.sec_key[Blaster_key]]) {
            key_hist[MLV_KEYBOARD_RETURN] = 0;
            key_hist[MLV_KEYBOARD_SPACE] = 0;
            (*moment)++;
        }
    } else if (*moment == 24) {
        if (key_hist[data->para.prim_key[Blaster_key]] || key_hist[data->para.sec_key[Blaster_key]]) {
            buy_bonus(terrain, *selected);
            key_hist[MLV_KEYBOARD_RETURN] = 0;
            key_hist[MLV_KEYBOARD_SPACE] = 0;
            (*moment)++;
        }
    } else if (*moment == 26) {
        if (key_hist[MLV_KEYBOARD_ESCAPE]) {
            (*moment)++;
        }
    }
}

/* Met à jours le terrain en fonction du clic appuyées */
static void update_with_mouse(Terrain *terrain, Data *data, int x, int y, MLV_Mouse_button button, MLV_Button_state state, int *moment, Power* selected) {
    if (*moment == 21) {
        select_bonus(selected, &(data->para), x, y);
        if (*selected != Aucun) {
            (*moment)++;
        }
    } else if (*moment == 22) {
        if (in_box(x, y, 3*data->para.width/4, data->para.height/2, data->para.width/4, data->para.height/12)) {
            (*moment)++;
        }
    } else if (*moment == 24) {
        if (in_box(x, y, 3*data->para.width/4, data->para.height/2, data->para.width/4, data->para.height/12)) {
            buy_bonus(terrain, *selected);
            (*moment)++;
        }
    } else if (*moment == 26) {
        if (in_box(x, y, 3*data->para.width/4, 7*data->para.height/12, data->para.width/4, data->para.height/12)) {
            (*moment)++;
        }
    }
}

/* Met à jours le terrain en fonction d'autres evenements */
static void update_with_time(Terrain *terrain, Data *data, int *moment) {
    if (*moment == 6) {
        if (terrain->nb_projectiles > 0) {
            (*moment)++;
        }
    } else if (*moment == 7) {
        if (terrain->nb_enemies == 0) {
            terrain->nb_projectiles = 0;
            terrain->nb_bonus = 0;
            (*moment)++;
            summon_group(terrain, &(data->para));
        }
    } else if (*moment == 8) {
        if (terrain->nb_enemies == 0) {
            terrain->nb_projectiles = 0;
            terrain->nb_bonus = 0;
            summon_list(terrain, &(data->para));
            (*moment)++;
        }
    }
}

/* Initialise un tableau de font */
static void init_font_array(MLV_Font *font_array[], int size, const char* font_name) {
    int i;
    for (i=0; i<size; i++) {
        if ((font_array[i] = MLV_load_font(font_name, (i+1))) == NULL) {
            fprintf(stderr, "Warning: la font %s, de taille %d ne peut pas etre chargée\n", font_name, i+1);
        }
    }
}

/* Boucle principale du tutoriel */
void tutoriel(Data* data) {
    int key_hist[MLV_KEYBOARD_LAST];
    MLV_Button_state state = MLV_RELEASED;
    int x, y, moment, ret, i;
    MLV_Mouse_button button;
    MLV_Keyboard_button key;
    MLV_Font *font_array[NB_FONTS];
    Parametre *para;
    Terrain terrain;
    MLV_Event event;
    Player player;
    struct timespec last, new;
    double accum;
    Power bonus;

    para = &(data->para);
    init_font_array(font_array, NB_FONTS, "data/fonts/retro_computer.ttf");
    init_tab(key_hist, MLV_KEYBOARD_LAST, 0);
    init_terrain(&terrain, para, &player, 0, 0, 0);
    init_player(&player, para->width/2-para->sizes.player_width/2, 9*para->height/10, para->sizes.player_width, para->sizes.player_height);
    moment = ret = x = y = 0;
    bonus = Aucun;
    
    MLV_play_music(data->sound_list.ambiant[0], data->para.music, -1);
    MLV_change_window_size(data->para.width, data->para.height + 50);

    while (moment != 27) {
        clock_gettime(CLOCK_REALTIME, &last);
        aff_tuto_moment(&terrain, data, font_array, moment, bonus);
        event = MLV_get_event(&key, NULL, NULL, NULL, NULL, &x, &y, &button, &state);


        if (event == MLV_KEY) {
            key_hist[key] = state==MLV_PRESSED;
            if (key==MLV_KEYBOARD_ESCAPE && state == MLV_RELEASED)
                break;
        } else if (event == MLV_MOUSE_BUTTON) {
            update_with_mouse(&terrain, data, x, y, button, state, &moment, &bonus);
        } else {
            update_with_time(&terrain, data, &moment);
        }

        update_with_key(&terrain, data, key_hist, &moment, &bonus);
        tuto_update(&terrain, data, moment);
        terrain_collisions(&terrain, &(data->para));
        joue_son(&(data->sound_list), &terrain, &(data->para), 0);
        check_death(&terrain);
        inc_clocks(&terrain);

        clock_gettime(CLOCK_REALTIME, &new);
        accum = (new.tv_sec - last.tv_sec) + ((new.tv_nsec - last.tv_nsec)/BILLION);
        if (accum < (1.0 / FRAME_PER_SEC)) {
            MLV_wait_milliseconds((int)(((1.0 / FRAME_PER_SEC) - accum) * 1000));
        }
    }
    MLV_stop_all_sounds();
    MLV_stop_music();
    for (i=0; i<NB_FONTS; ++i) {
        MLV_free_font(font_array[i]);
    }
    MLV_change_window_size(MENU_WIDTH, MENU_HEIGHT);
}



