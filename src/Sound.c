#include "../include/Projectile.h"
#include "../include/Player.h"
#include "../include/Sound.h"
#include "../include/Enemy.h"

/* Libère les sons contenus dans 'sound_list' */
void free_sound(SoundList* sound_list) {
    MLV_free_sound(sound_list->charged_shoot);
    MLV_free_sound(sound_list->player_hit[0]);
    MLV_free_sound(sound_list->player_hit[1]);
    MLV_free_sound(sound_list->player_hit[2]);
    MLV_free_sound(sound_list->player_death);
    MLV_free_sound(sound_list->boss_summon);
    MLV_free_sound(sound_list->enemy_shoot);
    MLV_free_sound(sound_list->bonus_pick);
    MLV_free_sound(sound_list->boss_shoot);
    MLV_free_sound(sound_list->boss_death);
    MLV_free_sound(sound_list->explosion);
    MLV_free_sound(sound_list->enemy_hit);
    MLV_free_sound(sound_list->boss_hit);
    MLV_free_sound(sound_list->victory);
    MLV_free_sound(sound_list->missile);
    MLV_free_sound(sound_list->shoot);
    MLV_free_sound(sound_list->pause);
}

/* Initialise les sons de 'sound_list'
 * Renvoie 0 si un son ne peut pas etre chargé
 *         1 sinon */
int init_bonus_sound(SoundList* sound_list) {
    free_sound(sound_list);
    if ((sound_list->charged_shoot = MLV_load_sound("data/audio/bonus_sound/charged_shoot.ogg")) == NULL) return 0;
    if ((sound_list->player_hit[0] = MLV_load_sound("data/audio/bonus_sound/player_hit1.ogg")) == NULL) return 0;
    if ((sound_list->player_hit[1] = MLV_load_sound("data/audio/bonus_sound/player_hit2.ogg")) == NULL) return 0;
    if ((sound_list->player_hit[2] = MLV_load_sound("data/audio/bonus_sound/player_hit3.ogg")) == NULL) return 0;
    if ((sound_list->player_death = MLV_load_sound("data/audio/bonus_sound/player_death.ogg")) == NULL) return 0;
    if ((sound_list->boss_summon = MLV_load_sound("data/audio/bonus_sound/boss_summon.ogg")) == NULL) return 0;
    if ((sound_list->enemy_shoot = MLV_load_sound("data/audio/bonus_sound/enemy_shoot.ogg")) == NULL) return 0;
    if ((sound_list->boss_shoot = MLV_load_sound("data/audio/bonus_sound/boss_shoot.ogg")) == NULL) return 0;
    if ((sound_list->boss_death = MLV_load_sound("data/audio/bonus_sound/boss_death.ogg")) == NULL) return 0;
    if ((sound_list->explosion = MLV_load_sound("data/audio/bonus_sound/explosion.ogg")) == NULL) return 0;
    if ((sound_list->enemy_hit = MLV_load_sound("data/audio/bonus_sound/enemy_hit.ogg")) == NULL) return 0;
    if ((sound_list->boss_hit = MLV_load_sound("data/audio/bonus_sound/boss_hit.ogg")) == NULL) return 0;
    if ((sound_list->bonus_pick = MLV_load_sound("data/audio/bonus_sound/bonus.ogg")) == NULL) return 0;
    if ((sound_list->victory = MLV_load_sound("data/audio/bonus_sound/victoire.ogg")) == NULL) return 0;
    if ((sound_list->missile = MLV_load_sound("data/audio/bonus_sound/missile.ogg")) == NULL) return 0;
    if ((sound_list->shoot = MLV_load_sound("data/audio/bonus_sound/shoot.ogg")) == NULL) return 0;
    if ((sound_list->pause = MLV_load_sound("data/audio/bonus_sound/pause.ogg")) == NULL) return 0;
    
    return 1;
}

/* Initialise les sons de 'sound_list'
 * Renvoie 0 si un son ne peut pas etre chargé
 *         1 sinon */
int init_sound(SoundList* sound_list) {
    int i; char tmp[STRING_LEN___];
    
    if ((sound_list->charged_shoot = MLV_load_sound("data/audio/sound/charged_shoot.ogg")) == NULL) return 0;
    if ((sound_list->player_hit[0] = MLV_load_sound("data/audio/sound/player_hit1.ogg")) == NULL) return 0;
    if ((sound_list->player_hit[1] = MLV_load_sound("data/audio/sound/player_hit2.ogg")) == NULL) return 0;
    if ((sound_list->player_hit[2] = MLV_load_sound("data/audio/sound/player_hit3.ogg")) == NULL) return 0;
    if ((sound_list->player_death = MLV_load_sound("data/audio/sound/player_death.ogg")) == NULL) return 0;
    if ((sound_list->boss_summon = MLV_load_sound("data/audio/sound/boss_summon.ogg")) == NULL) return 0;
    if ((sound_list->enemy_shoot = MLV_load_sound("data/audio/sound/enemy_shoot.ogg")) == NULL) return 0;
    if ((sound_list->boss_shoot = MLV_load_sound("data/audio/sound/boss_shoot.ogg")) == NULL) return 0;
    if ((sound_list->boss_death = MLV_load_sound("data/audio/sound/boss_death.ogg")) == NULL) return 0;
    if ((sound_list->explosion = MLV_load_sound("data/audio/sound/explosion.ogg")) == NULL) return 0;
    if ((sound_list->enemy_hit = MLV_load_sound("data/audio/sound/enemy_hit.ogg")) == NULL) return 0;
    if ((sound_list->boss_hit = MLV_load_sound("data/audio/sound/boss_hit.ogg")) == NULL) return 0;
    if ((sound_list->bonus_pick = MLV_load_sound("data/audio/sound/bonus.ogg")) == NULL) return 0;
    if ((sound_list->victory = MLV_load_sound("data/audio/sound/victoire.ogg")) == NULL) return 0;
    if ((sound_list->missile = MLV_load_sound("data/audio/sound/missile.ogg")) == NULL) return 0;
    if ((sound_list->shoot = MLV_load_sound("data/audio/sound/shoot.ogg")) == NULL) return 0;
    if ((sound_list->pause = MLV_load_sound("data/audio/sound/pause.ogg")) == NULL) return 0;
    
    for (i=0; i<NB_MONDE+1; ++i) {
        sprintf(tmp, "data/audio/ambiant/ambiant%d.mp3", i);
        if ((sound_list->ambiant[i] = MLV_load_music(tmp)) == NULL) fprintf(stderr, "Fichier audio ambiant%d.ogg manquant\n", i);
    }
    return 1;
}

/* Joue les sons devant être joué. */
void joue_son(SoundList* list, Terrain* terrain, Parametre* para, int boss) {
    Projectile *p;
    Enemy *e;
    int i;
    
    if (terrain->player->b_cooldown == B_COOLDOWN-1) {MLV_play_sound(list->shoot, para->sound/100.0);}
    if (terrain->player->m_cooldown == M_COOLDOWN-1) {MLV_play_sound(list->missile, para->sound/100.0);}
    if (terrain->player->recovery_time == RECOVERY_TIME) {MLV_play_sound(list->player_hit[MLV_get_random_integer(0, 3)], para->sound/100.0);}
    if (terrain->player->pv == 0) {MLV_play_sound(list->player_death, para->sound/100.0);}
    
    for (i=0; i<terrain->nb_projectiles; i++) {
        p = terrain->projectiles + i;
        if (p->type == PBlaster_Charged && p->internal_clock == 0 && p->alive) {MLV_play_sound(list->charged_shoot, para->sound/100.0);}
        else if (p->type == PMissile && !p->alive && p->internal_clock == 0) {MLV_play_sound(list->explosion, para->sound/100.0);}
        else if (p->type >= BBlaster && p->alive && p->internal_clock == 0) {MLV_play_sound(list->boss_shoot, para->sound/100.0);}
        else if (p->type >= EBlaster && p->alive && p->internal_clock == 0) {MLV_play_sound(list->enemy_shoot, para->sound/100.0);}
    }
    
    for (i=0; i<terrain->nb_enemies; i++) {
        e = terrain->enemies + i;
        if (Boss1 <= e->type && e->type <= Boss5) {
            if (e->damaged == 5) {MLV_play_sound(list->boss_hit, para->sound/100.0);}
            if (e->pv == 0 && e->internal_clock == 0) {MLV_play_sound(list->boss_death, para->sound/100.0);}
        } else {
            if (boss && e->internal_clock == 0 && e->alive) {MLV_play_sound(list->boss_summon, para->sound/100.0);}
            if (e->damaged == 5) {MLV_play_sound(list->enemy_hit, para->sound/100.0);}
        }
    }
    for (i=0; i<terrain->nb_bonus; i++) {
        if (terrain->bonus[i].alive == 0 && terrain->bonus[i].y < para->height-20) {MLV_play_sound(list->bonus_pick, para->sound/100.0);}
    }
}
