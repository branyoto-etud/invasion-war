#define _POSIX_C_SOURCE 199309L
#include <time.h>

#include "../include/Partie.h"
#include "../include/Menu.h"

/* Initialise toutes les valeurs de 'tab' de taille 'n' à la valeur 'v' */
void init_tab(int tab[], int n, int v) {
    int i;
    for (i=0; i<n; i++)
        tab[i] = v;
}

/* Modifie les valeurs du tableau key_hist grace à key, state et event */
void use_event(int event, int state, int key, int key_hist[]) {
    if (event != MLV_KEY)
        return;
    key_hist[key] = state==MLV_PRESSED;
}

/* Met à jour les tableaux de projectiles, ennemies et bonus en supprimant les morts */
void check_death(Terrain* terrain) {
    terrain->nb_projectiles = projectile_remove(terrain->projectiles, terrain->nb_projectiles);
    terrain->nb_enemies = enemy_remove(terrain->enemies, terrain->nb_enemies);
    terrain->nb_bonus = bonus_remove(terrain->bonus, terrain->nb_bonus);
}

/* Renvoie 1 si la partie est finie 0 sinon */
int game_ended(Terrain* terrain, Parametre* para, int *boss) {
    if (!terrain->nb_enemies && !terrain->level) {      /* Si le niveau est fini et qu'il y a personne */
        if (*boss == BOSS_COOLDOWN)                     /* Et que le boss est déjà apparu */
            return 1;                                   /* On renvoie 1 */
        if (*boss < BOSS_COOLDOWN)                      /* Si le boss n'est pas apparu et que son horloge est inférieur à BOSS_COOLDOWN */
            *boss+=1;                                   /* On incrémente */
        if (*boss == BOSS_COOLDOWN)
            add_boss(terrain, para);
    }
    return player_dead(*(terrain->player));
}

/* Renvoie -2 si erreur -1 si quitter et le score sinon */
int boucle_de_jeu(Data* data, Player* player, int world_number, int fpos, int score) {
    int key_hist[MLV_KEYBOARD_LAST];
    struct timespec last, new;
    Terrain terrain;
    int quit, boss, shop;
    double accum;
    MLV_Keyboard_button key_sym;
    MLV_Button_state state;
    MLV_Event event;
    
    /* Initialisation */
    quit = boss = shop = 0;
    init_tab(key_hist, MLV_KEYBOARD_LAST, 0); 
    MLV_change_window_size(data->para.width, data->para.height + 50);
    
    if (!init_terrain(&terrain, &(data->para), player, world_number, fpos, score)) {
        fprintf(stderr, "Error: Cannot load the world %d!\n", world_number);
        return -1;
    }
    MLV_play_music(data->sound_list.ambiant[world_number], data->para.music, -1);
    
    /* Main loop */
    while( !quit ) {
        clock_gettime(CLOCK_REALTIME, &last);
        
        /* THIS FUNCTION CALL A SINGLE TIME MLV_actualise_window() */
        draw_window(&(data->para), &terrain, &(data->img_list), boss);
        
        event = MLV_get_event(&key_sym, NULL,NULL,NULL,NULL,NULL,NULL,NULL, &state);
        use_event(event, state, key_sym, key_hist);
        if (!use_key(&terrain, key_hist, &(data->para))) {
            break;
        }
        
        if (!terrain_update(&terrain, &(data->para), &shop, data->fname, data->last_world)) {
            printf("Error when reading file, %d\n", terrain.lvlnum);
            return -1;
        }
        if (shop) {
            shop_menu(&(data->para), &terrain, &(data->img_list));
            init_tab(key_hist, MLV_KEYBOARD_LAST, 0);
            shop = 0;
        }
        terrain_collisions(&terrain, &(data->para));
        
        joue_son(&(data->sound_list), &terrain, &(data->para), boss);
        
        /* Check death (projectiles/ enemies/ bonus) */
        check_death(&terrain);
        quit = game_ended(&terrain, &(data->para), &boss);
        /* Update frames */
        inc_clocks(&terrain);
        
        /* Get the time in nanosecond at the frame ending */
        clock_gettime(CLOCK_REALTIME, &new);
        accum = (new.tv_sec - last.tv_sec) + ((new.tv_nsec - last.tv_nsec)/BILLION);
        /* Wait if frame too short */
        if (accum < (1.0 / FRAME_PER_SEC)) {
            MLV_wait_milliseconds((int)(((1.0 / FRAME_PER_SEC) - accum) * 1000));
        }
    }
    if (terrain.level != NULL) {
        fseek(terrain.level, -1, SEEK_END);
        read_file(&terrain, &(data->para), NULL, data->last_world);
    }
    MLV_wait_milliseconds(50);
    MLV_stop_music();
    MLV_change_window_size(MENU_WIDTH, MENU_HEIGHT);
    if (quit == 0)
        return -1;
    return terrain.score; 
}
