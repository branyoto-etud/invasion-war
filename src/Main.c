#define _POSIX_C_SOURCE 199309L
#include <MLV/MLV_all.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "../include/Tutoriel.h"
#include "../include/Affichage.h"
#include "../include/Parametre.h"
#include "../include/Tutoriel.h"
#include "../include/Fichier.h"
#include "../include/Partie.h"
#include "../include/Sound.h"
#include "../include/Menu.h"


int main() {
    Data data;
    
    /* Initialise toutes les ressources */
    srand(time(NULL));
    if (MLV_init_audio() == -1) {
        return 1;
    }
    MLV_create_window("Invasion-War", "I-War", MENU_WIDTH, MENU_HEIGHT);
    
    if ( !load_option(&(data.para)) ) {
        fprintf(stderr, "Error: Cannot read properly the option file!\n");
        return 1;
    }
    if ( !img_chargement(&(data.img_list), &(data.para)) ) {
        fprintf(stderr, "Error: Cannot load the images properly!\n");
        return 1;
    }
    if ( !init_sound(&(data.sound_list)) ) {
        fprintf(stderr, "Error: Cannot load the sound properly!\n");
        return 1;
    }
    data.fname[0] = '\0';
    data.last_world = 0;
    data.lvl_bonus = 0;
    
    /* Lancement */
    MLV_play_music(data.sound_list.ambiant[NB_MONDE], data.para.music, -1);
    menu_principal(&data);
    MLV_stop_all_sounds();
    MLV_stop_music();
    
    /* Si je met un MLV_free_window() ou un MLV_free_audio() le programme freez */
    return 0;
}
