#include <MLV/MLV_all.h>

#include "../include/Parametre.h"
#include "../include/Affichage.h"
#include "../include/Sound.h"


#ifndef __PARTIE__
#define __PARTIE__

#define FRAME_PER_SEC (48.0)
#define BILLION (1000000000) /* Si je me trompe pas */


typedef struct {
    int last_world;
    int lvl_bonus;
    Parametre para;
    ImgList img_list;
    SoundList sound_list;
    char fname[STRING_LEN___];
} Data;

void init_tab(int tab[], int n, int v);
void use_event(int event, int state, int key, int key_hist[]);
int game_ended(Terrain* terrain, Parametre* para, int *boss);
int boucle_de_jeu(Data* data, Player* player, int world_number, int fpos, int score);
void check_death(Terrain* terrain);

#endif
