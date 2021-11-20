#include <MLV/MLV_all.h>
#include "../include/Parametre.h"
#include "../include/Terrain.h"

#ifndef __SOUND__
#define __SOUND__

typedef struct {
    MLV_Sound* charged_shoot;
    MLV_Sound* player_hit[3];
    MLV_Sound* player_death;
    MLV_Sound* boss_summon;
    MLV_Sound* enemy_shoot;
    MLV_Sound* bonus_pick;
    MLV_Sound* boss_shoot;
    MLV_Sound* boss_death;
    MLV_Sound* explosion;
    MLV_Sound* enemy_hit;
    MLV_Sound* boss_hit;
    MLV_Sound* victory;
    MLV_Sound* missile;
    MLV_Sound* shoot;
    MLV_Sound* pause;
    
    MLV_Music* ambiant[NB_MONDE+1];
} SoundList;

void free_sound(SoundList* sound_list);
int init_bonus_sound(SoundList* sound_list);
int init_sound(SoundList* sound_list);

void joue_son(SoundList* list, Terrain* terrain, Parametre* para, int boss);


#endif
