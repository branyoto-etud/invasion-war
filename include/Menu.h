#include <MLV/MLV_all.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdio.h>
#include <ftw.h>

#include "../include/Affichage.h"
#include "../include/Parametre.h"
#include "../include/Fichier.h"
#include "../include/Terrain.h"
#include "../include/Partie.h"

#ifndef __MENU__
    #define __MENU__

    #define OUT "bin/out.txt" /* fichier répertoriant les informations sur les fichiers de sauvegarde (me juge pas) */

    typedef enum {
        Up,
        Up2,
        Left,
        Rght,
        Down,
        Chrg,
        Blst,
        Miss,
        Bons,
        Left2,
        Rght2,
        Down2,
        Blst2,
        Chrg2,
        Miss2,
        Bons2,
        None,
        Nouv,
        Lead,
        Sett,
        Musc,
        Sons,
        R600,
        R700,
        R800,
        Strt,
        Quit
    } Selection;

    typedef struct star {
      int x, y;
      int speed;
      int size;
      int alpha;
    } Star;

    typedef struct pstat {
      char nom[STRING_LEN___];
      struct pstat *next;
      struct pstat *prev;
    } PlayerStats, *Liste;
    
    int in_box(int mx, int my, int bx, int by, int bw, int bh);
    int get_bonus_path(char* path_bonus, int stock, int frame);
    int set_new_key(Parametre* para, MLV_Keyboard_button key, Selection select);
    void load_unicodes(char tab[][STRING_LEN___], Parametre* para);
    void set_unicode(char* str, MLV_Keyboard_button touche);
    MLV_Color rank(int pos, int alpha);
    MLV_Color set_player_color(int numpage, int alpha);

    void afficheListe(Liste lst);
    void remove_path(char* nom);

    int  menu_monde(Data* data, Player* player, int state, int fpos, int score);
    int  menu_options(Parametre* para, int ingame);
    int  menu_nouv_partie(Data* data);
    int  menu_charger(Data* data);
    void menu_leaderboard();
    void menu_principal(Data* data);
    int  menu_pause(Parametre* para);
    
    int buy_bonus(Terrain* terrain, Power selected);
    void select_bonus(Power* selected, Parametre* para, int x, int y);
    void select_bonus_keyboard(Power* selected, MLV_Keyboard_button button, Parametre* para);
    void shop_menu(Parametre* para, Terrain* terrain, ImgList* img_list);
    void draw_shop_menu(Terrain* terrain, Parametre* para, ImgList* img_list, Power selected, int frame, int *err_msg);
#endif
