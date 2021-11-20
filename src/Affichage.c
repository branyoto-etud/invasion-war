#include "../include/Affichage.h"

#define PROJECT_SCALE (.1)
#define PLAYER_SCALE (.1)
#define BONUS_SCALE (.08)
#define ENEMY_SCALE (.08)
#define BOSS_SCALE (.5)
#define NB_IMAGES (344)


/* Renvoie le minimum entre a et b */
static double dmin(double a, double b) {
    return a>b ? b : a;
}

/* Chargement */

/* Charge une image
 * param 'lst' : Structure contenant l'image présente dans l'écran de chargement
 * param 'path' : Chemin de l'image à charger
 * param 'frame' : Numéro de l'image à chargé (-1 s'il n'y a pas de numéro)
 * retour : L'image chargée
 *          NULL si l'image ne peut pas être chargée
 */
MLV_Image* load_img(ImgList *lst, char* path, int frame) {
    char full_path[STRING_LEN___];
    static int i;
    int w, h;
    if (frame == -2) {
        i = 0;
        return NULL;
    }
    if (frame < 0) {
        sprintf(full_path, "%s.png", path);
    } else {
        sprintf(full_path, "%s%d.png", path, frame);
    }
    i++;
    MLV_draw_filled_rectangle(0, MENU_HEIGHT/2-90, MENU_WIDTH, MENU_HEIGHT, MLV_COLOR_BLACK);
    MLV_draw_filled_rectangle(35, MENU_HEIGHT/2-20, MENU_WIDTH-70, 40, MLV_COLOR_WHITE);
    MLV_draw_filled_rectangle(40, MENU_HEIGHT/2-15, i*(MENU_WIDTH-80)/NB_IMAGES, 30, MLV_rgba(150, 220, 50, 255));
    MLV_draw_filled_rectangle(40, MENU_HEIGHT/2-10, i*(MENU_WIDTH-80)/NB_IMAGES, 10, MLV_rgba(255, 255, 255, 100));
    
    MLV_get_size_of_text("%d / %d", &w, &h, i, NB_IMAGES);
    MLV_draw_text(MENU_WIDTH/2 - w/2, MENU_HEIGHT/2 - h/2, "%d / %d", MLV_rgba(50, 100, 10, 255), i, NB_IMAGES);
    draw_centered_text(MENU_WIDTH/2, MENU_HEIGHT/2-60, full_path, NULL, MLV_COLOR_WHITE);
    if (i > 1)
        MLV_draw_image(lst->loading[(i/20)%4], MENU_WIDTH-100, MENU_HEIGHT-100);
    MLV_actualise_window();
    return MLV_load_image(full_path);
}

/* Initialise la structure Sizes en utilisant la taille des images de 'img_list'
 * param 'img_list' : Liste des images chargées
 * param 'sizes' : Structure à initialiser
 */
void init_real_sizes(ImgList* img_list, Sizes* sizes) {
    if (img_list->player[0] != NULL)
        MLV_get_image_size(img_list->player[0], &(sizes->player_width), &(sizes->player_height));
    
    if (img_list->enemy1[0] != NULL)
        MLV_get_image_size(img_list->enemy1[0], &(sizes->enemy1_width), &(sizes->enemy1_height));
    if (img_list->enemy2[0] != NULL)
        MLV_get_image_size(img_list->enemy2[0], &(sizes->enemy2_width), &(sizes->enemy2_height));
    if (img_list->enemy3[0] != NULL)
        MLV_get_image_size(img_list->enemy3[0], &(sizes->enemy3_width), &(sizes->enemy3_height));
    if (img_list->enemy4[0] != NULL)
        MLV_get_image_size(img_list->enemy4[0], &(sizes->enemy4_width), &(sizes->enemy4_height));
    if (img_list->boss1[0] != NULL)
        MLV_get_image_size(img_list->boss1[0], &(sizes->boss_width), &(sizes->boss_height));
    
    if (img_list->pblaster_charged40[0] != NULL)
        MLV_get_image_size(img_list->pblaster_charged40[0], &(sizes->pcharged40_width), &(sizes->pcharged40_height));
    if (img_list->pblaster_charged60[0] != NULL)
        MLV_get_image_size(img_list->pblaster_charged60[0], &(sizes->pcharged60_width), &(sizes->pcharged60_height));
    if (img_list->pblaster_charged80[0] != NULL)
        MLV_get_image_size(img_list->pblaster_charged80[0], &(sizes->pcharged80_width), &(sizes->pcharged80_height));
    if (img_list->pblaster_charged100[0] != NULL)
        MLV_get_image_size(img_list->pblaster_charged100[0], &(sizes->pcharged100_width), &(sizes->pcharged100_height));
    if (img_list->pmissile[0] != NULL)
        MLV_get_image_size(img_list->pmissile[0], &(sizes->missile_width), &(sizes->missile_height));
    if (img_list->pmissile_d[0] != NULL)
        MLV_get_image_size(img_list->pmissile_d[0], &(sizes->explosion_width), &(sizes->explosion_height));
    if (img_list->pblaster[0] != NULL)
        MLV_get_image_size(img_list->pblaster[0], &(sizes->pblaster_width), &(sizes->pblaster_height));
    if (img_list->eblaster[0] != NULL)
        MLV_get_image_size(img_list->eblaster[0], &(sizes->eblaster_width), &(sizes->eblaster_height));
    if (img_list->bblaster[0] != NULL)
        MLV_get_image_size(img_list->bblaster[0], &(sizes->bblaster_width), &(sizes->bblaster_height));
    if (img_list->emissile_auto[0] != NULL)
        MLV_get_image_size(img_list->emissile_auto[0], &(sizes->emissile_auto_width), &(sizes->emissile_auto_height));
    if (img_list->bmissile_auto[0] != NULL)
        MLV_get_image_size(img_list->bmissile_auto[0], &(sizes->bmissile_auto_width), &(sizes->bmissile_auto_height));
    
    if (img_list->background[0] != NULL) 
        MLV_get_image_size(img_list->background[0], NULL, &(sizes->background_height));
    
    if (img_list->bonus_mult[0] != NULL)
        MLV_get_image_size(img_list->bonus_mult[0], &(sizes->bonus_width), &(sizes->bonus_height));
}

void free_image(ImgList *img_list) {
    int i;
    for (i=0; i<4; i++) {
        MLV_free_image(img_list->loading[i]);
    }
    
    for (i=0; i<PROJECT_FRAME; i++) {
        MLV_free_image(img_list->pblaster_charged40[i]);
        MLV_free_image(img_list->pblaster_charged60[i]);
        MLV_free_image(img_list->pblaster_charged80[i]);
        MLV_free_image(img_list->pblaster_charged100[i]);
        MLV_free_image(img_list->eblaster_sin[i]);
        MLV_free_image(img_list->emissile_auto[i]);
        MLV_free_image(img_list->bmissile_auto[i]);
        MLV_free_image(img_list->pblaster[i]);
        MLV_free_image(img_list->pmissile[i]);
        MLV_free_image(img_list->bblaster[i]);
        MLV_free_image(img_list->eblaster[i]);
    }
    for (i=0; i<PLAYER_FRAME; i++) {
        MLV_free_image(img_list->player[i]);
        MLV_free_image(img_list->player_left[i]);
        MLV_free_image(img_list->player_right[i]);
    }
    for (i=0; i<ENEMY_FRAME; i++) {
        MLV_free_image(img_list->enemy1[i]);
        MLV_free_image(img_list->enemy2[i]);
        MLV_free_image(img_list->enemy3[i]);
        MLV_free_image(img_list->enemy4[i]);
        MLV_free_image(img_list->special[i]);
    }
    for (i=0; i<DEAD_FRAME; i++) {
        MLV_free_image(img_list->enemy1_d[i]);
        MLV_free_image(img_list->enemy2_d[i]);
        MLV_free_image(img_list->enemy3_d[i]);
        MLV_free_image(img_list->enemy4_d[i]);
        MLV_free_image(img_list->special_d[i]);
        MLV_free_image(img_list->boss1_d[i]);
        MLV_free_image(img_list->boss2_d[i]);
        MLV_free_image(img_list->boss3_d[i]);
        MLV_free_image(img_list->boss4_d[i]);
        MLV_free_image(img_list->boss5_d[i]);
        MLV_free_image(img_list->emissile_auto_d[i]);
        MLV_free_image(img_list->bmissile_auto_d[i]);
        MLV_free_image(img_list->eblaster_sin_d[i]);
        MLV_free_image(img_list->eblaster_d[i]);
        MLV_free_image(img_list->pblaster_d[i]);
        MLV_free_image(img_list->bblaster_d[i]);
        MLV_free_image(img_list->pmissile_d[i]);
        MLV_free_image(img_list->pmissile_d[i+4]);
    }
    for (i=0; i<BONUS_FRAME; i++) {
        MLV_free_image(img_list->bonus_quad[i]);
        MLV_free_image(img_list->bonus_mult[i]);
        MLV_free_image(img_list->bonus_dual[i]);
        MLV_free_image(img_list->bonus_heal[i]);
        MLV_free_image(img_list->bonus_single[i]);
        MLV_free_image(img_list->bonus_shield[i]);
        MLV_free_image(img_list->bonus_triple[i]);
        MLV_free_image(img_list->bonus_missile[i]);
        MLV_free_image(img_list->bonus_bullettime[i]);
        MLV_free_image(img_list->bonus_healthboost[i]);
    }
    for (i=0; i<BOSS_FRAME; i++) {
        MLV_free_image(img_list->boss1[i]);
        MLV_free_image(img_list->boss2[i]);
        MLV_free_image(img_list->boss3[i]);
        MLV_free_image(img_list->boss4[i]);
        MLV_free_image(img_list->boss5[i]);
    }
    /* Background */ 
    MLV_free_image(img_list->background[0]);
    MLV_free_image(img_list->background[1]);
    MLV_free_image(img_list->background[2]);
    MLV_free_image(img_list->background[3]);
    MLV_free_image(img_list->background[4]);
    
    MLV_free_image(img_list->button);
    MLV_free_image(img_list->space);
    MLV_free_image(img_list->right);
    MLV_free_image(img_list->left);
    MLV_free_image(img_list->down);
    MLV_free_image(img_list->up);
    MLV_free_image(img_list->over_miss);
    MLV_free_font(img_list->font);
    MLV_free_font(img_list->warning_font);
    
    
}

/* Charge toutes les images nescessaires au jeu
 * param 'img_list' : Liste des images à initialiser
 * param 'para' : Paramètre du jeu
 * retour : 0 si une image ne peut pas être chargée
 *          1 si toutes les images sont bien chargées
 */
int img_chargement(ImgList * img_list, Parametre *para) {
    int i;
    double ratio = dmin((para->width / WINDOW_WIDTH), ( para->height / WINDOW_HEIGHT));
    double projectiles_scale = PROJECT_SCALE * ratio;
    double player_scale = PLAYER_SCALE * ratio;
    double enemy_scale = ENEMY_SCALE * ratio;
    double boss_scale = BOSS_SCALE * ratio;
    double bonus_scale = BONUS_SCALE * ratio;
    
    MLV_clear_window(MLV_COLOR_BLACK);
    draw_centered_text(MENU_WIDTH/2, MENU_HEIGHT/4, "Chargement des images... ", NULL, MLV_COLOR_WHITE);
    load_img(img_list, "", -2);
    for (i=0; i<4; i++) {
        if ((img_list->loading[i] = load_img(img_list, "data/img/icons/loading/getevent", i)) == NULL) return 0;  else MLV_resize_image(img_list->loading[i], 100, 100);
    }
    
    for (i=0; i<PROJECT_FRAME; i++) {
        if ((img_list->pblaster_charged40[i] = load_img(img_list, "data/img/projectiles/pblaster_charged/frame_", i)) == NULL) return 0;  else MLV_scale_image(img_list->pblaster_charged40[i], 1.0/35.0);
        if ((img_list->pblaster_charged60[i] = load_img(img_list, "data/img/projectiles/pblaster_charged/frame_", i)) == NULL) return 0;  else MLV_scale_image(img_list->pblaster_charged60[i], 1.5/35.0);
        if ((img_list->pblaster_charged80[i] = load_img(img_list, "data/img/projectiles/pblaster_charged/frame_", i)) == NULL) return 0;  else MLV_scale_image(img_list->pblaster_charged80[i], 2.0/35.0);
        if ((img_list->pblaster_charged100[i] = load_img(img_list, "data/img/projectiles/pblaster_charged/frame_", i)) == NULL) return 0;  else MLV_scale_image(img_list->pblaster_charged100[i], 2.5/35.0);
        if ((img_list->eblaster_sin[i] = load_img(img_list, "data/img/projectiles/eblaster_sin/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->eblaster_sin[i], projectiles_scale * 2);
        if ((img_list->emissile_auto[i] = load_img(img_list, "data/img/projectiles/emissile_auto/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->emissile_auto[i], projectiles_scale * 4);
        if ((img_list->bmissile_auto[i] = load_img(img_list, "data/img/projectiles/emissile_auto/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->bmissile_auto[i], projectiles_scale * 6);
        if ((img_list->pblaster[i] = load_img(img_list, "data/img/projectiles/pblaster/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->pblaster[i], projectiles_scale);
        if ((img_list->pmissile[i] = load_img(img_list, "data/img/projectiles/pmissile/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->pmissile[i], projectiles_scale);
        if ((img_list->bblaster[i] = load_img(img_list, "data/img/projectiles/bblaster/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->bblaster[i], projectiles_scale);
        if ((img_list->eblaster[i] = load_img(img_list, "data/img/projectiles/eblaster/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->eblaster[i], projectiles_scale * 2);
    }
    for (i=0; i<PLAYER_FRAME; i++) {
        if ((img_list->player[i] = load_img(img_list, "data/img/player/spaceship/frame_", i)) == NULL) return 0;  else MLV_scale_image(img_list->player[i], player_scale);
        if ((img_list->player_left[i] = load_img(img_list, "data/img/player/spaceship_left/frame_", i)) == NULL) return 0;  else MLV_scale_image(img_list->player_left[i], player_scale);
        if ((img_list->player_right[i] = load_img(img_list, "data/img/player/spaceship_right/frame_", i)) == NULL) return 0;  else MLV_scale_image(img_list->player_right[i], player_scale);
    }
    for (i=0; i<ENEMY_FRAME; i++) {
        if ((img_list->enemy1[i] = load_img(img_list, "data/img/enemies/enemy1/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->enemy1[i], enemy_scale);
        if ((img_list->enemy2[i] = load_img(img_list, "data/img/enemies/enemy2/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->enemy2[i], enemy_scale);
        if ((img_list->enemy3[i] = load_img(img_list, "data/img/enemies/enemy3/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->enemy3[i], enemy_scale);
        if ((img_list->enemy4[i] = load_img(img_list, "data/img/enemies/enemy4/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->enemy4[i], enemy_scale);
        if ((img_list->special[i] = load_img(img_list, "data/img/enemies/special/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->special[i], enemy_scale);
    }
    for (i=0; i<DEAD_FRAME; i++) {
        if ((img_list->enemy1_d[i] = load_img(img_list, "data/img/enemies/enemy1_d/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->enemy1_d[i], enemy_scale);
        if ((img_list->enemy2_d[i] = load_img(img_list, "data/img/enemies/enemy2_d/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->enemy2_d[i], enemy_scale);
        if ((img_list->enemy3_d[i] = load_img(img_list, "data/img/enemies/enemy3_d/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->enemy3_d[i], enemy_scale);
        if ((img_list->enemy4_d[i] = load_img(img_list, "data/img/enemies/enemy4_d/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->enemy4_d[i], enemy_scale);
        if ((img_list->special_d[i] = load_img(img_list, "data/img/enemies/special_d/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->special_d[i], enemy_scale);
        if ((img_list->boss1_d[i] = load_img(img_list, "data/img/enemies/boss1_d/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->boss1_d[i], boss_scale);
        if ((img_list->boss2_d[i] = load_img(img_list, "data/img/enemies/boss2_d/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->boss2_d[i], boss_scale);
        if ((img_list->boss3_d[i] = load_img(img_list, "data/img/enemies/boss3_d/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->boss3_d[i], boss_scale);
        if ((img_list->boss4_d[i] = load_img(img_list, "data/img/enemies/boss4_d/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->boss4_d[i], boss_scale);
        if ((img_list->boss5_d[i] = load_img(img_list, "data/img/enemies/boss5_d/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->boss5_d[i], boss_scale);
        if ((img_list->emissile_auto_d[i] = load_img(img_list, "data/img/projectiles/emissile_auto_d/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->emissile_auto_d[i], projectiles_scale * 4);
        if ((img_list->bmissile_auto_d[i] = load_img(img_list, "data/img/projectiles/emissile_auto_d/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->bmissile_auto_d[i], projectiles_scale * 6);
        if ((img_list->eblaster_sin_d[i] = load_img(img_list, "data/img/projectiles/eblaster_sin_d/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->eblaster_sin_d[i], projectiles_scale * 2);
        if ((img_list->eblaster_d[i] = load_img(img_list, "data/img/projectiles/eblaster_d/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->eblaster_d[i], projectiles_scale * 2);
        if ((img_list->pblaster_d[i] = load_img(img_list, "data/img/projectiles/pblaster_d/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->pblaster_d[i], projectiles_scale / 2);
        if ((img_list->bblaster_d[i] = load_img(img_list, "data/img/projectiles/bblaster_d/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->bblaster_d[i], projectiles_scale * 4);
        if ((img_list->pmissile_d[i] = load_img(img_list, "data/img/projectiles/pmissile_d/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->pmissile_d[i], projectiles_scale * 4);
        if ((img_list->pmissile_d[i+4] = load_img(img_list, "data/img/projectiles/pmissile_d/frame_", i+4)) == NULL) return 0; else MLV_scale_image(img_list->pmissile_d[i+4], projectiles_scale * 4);
    }
    for (i=0; i<BONUS_FRAME; i++) {
        if ((img_list->bonus_quad[i] = load_img(img_list, "data/img/bonus/bonus_quad/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->bonus_quad[i], bonus_scale);
        if ((img_list->bonus_mult[i] = load_img(img_list, "data/img/bonus/bonus_mult/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->bonus_mult[i], bonus_scale);
        if ((img_list->bonus_dual[i] = load_img(img_list, "data/img/bonus/bonus_dual/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->bonus_dual[i], bonus_scale);
        if ((img_list->bonus_heal[i] = load_img(img_list, "data/img/bonus/bonus_heal/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->bonus_heal[i], bonus_scale);
        if ((img_list->bonus_single[i] = load_img(img_list, "data/img/bonus/bonus_single/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->bonus_single[i], bonus_scale);
        if ((img_list->bonus_shield[i] = load_img(img_list, "data/img/bonus/bonus_shield/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->bonus_shield[i], bonus_scale);
        if ((img_list->bonus_triple[i] = load_img(img_list, "data/img/bonus/bonus_triple/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->bonus_triple[i], bonus_scale);
        if ((img_list->bonus_missile[i] = load_img(img_list, "data/img/bonus/bonus_missile/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->bonus_missile[i], bonus_scale);
        if ((img_list->bonus_bullettime[i] = load_img(img_list, "data/img/bonus/bonus_bullettime/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->bonus_bullettime[i], bonus_scale);
        if ((img_list->bonus_healthboost[i] = load_img(img_list, "data/img/bonus/bonus_healthboost/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->bonus_healthboost[i], bonus_scale);
    }
    for (i=0; i<BOSS_FRAME; i++) {
        if ((img_list->boss1[i] = load_img(img_list, "data/img/enemies/boss1/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->boss1[i], boss_scale);
        if ((img_list->boss2[i] = load_img(img_list, "data/img/enemies/boss2/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->boss2[i], boss_scale);
        if ((img_list->boss3[i] = load_img(img_list, "data/img/enemies/boss3/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->boss3[i], boss_scale);
        if ((img_list->boss4[i] = load_img(img_list, "data/img/enemies/boss4/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->boss4[i], boss_scale);
        if ((img_list->boss5[i] = load_img(img_list, "data/img/enemies/boss5/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->boss5[i], boss_scale);
    }
    /* Background */ 
    if ((img_list->background[0] = load_img(img_list, "data/img/background/background_0", -1)) == NULL)     return 0; else MLV_resize_image_with_proportions(img_list->background[0], para->width, -1);
    if ((img_list->background[1] = load_img(img_list, "data/img/background/background_1", -1)) == NULL)   return 0; else MLV_resize_image_with_proportions(img_list->background[1], para->width, -1);
    if ((img_list->background[2] = load_img(img_list, "data/img/background/background_2", -1)) == NULL)    return 0; else MLV_resize_image_with_proportions(img_list->background[2], para->width, -1);
    if ((img_list->background[3] = load_img(img_list, "data/img/background/background_3", -1)) == NULL) return 0; else MLV_resize_image_with_proportions(img_list->background[3], para->width, -1);
    if ((img_list->background[4] = load_img(img_list, "data/img/background/background_4", -1)) == NULL)   return 0; else MLV_resize_image_with_proportions(img_list->background[4], para->width, -1);
    
    if ((img_list->button = load_img(img_list, "data/img/icons/button", -1)) == NULL) return 0;
    if ((img_list->space = load_img(img_list, "data/img/icons/space", -1)) == NULL) return 0;
    if ((img_list->right = load_img(img_list, "data/img/icons/right", -1)) == NULL) return 0;
    if ((img_list->left = load_img(img_list, "data/img/icons/left", -1)) == NULL) return 0;
    if ((img_list->down = load_img(img_list, "data/img/icons/down", -1)) == NULL) return 0;
    if ((img_list->up = load_img(img_list, "data/img/icons/up", -1)) == NULL) return 0;
    if ((img_list->over_miss = load_img(img_list, "data/img/icons/over_missile", -1)) == NULL) return 0;
    if ((img_list->font = MLV_load_font("data/fonts/plasma.ttf", 20 * (para->width / WINDOW_WIDTH))) == NULL) return 0;
    if ((img_list->warning_font = MLV_load_font("data/fonts/plasma.ttf", 100 * (para->width / WINDOW_WIDTH))) == NULL) return 0;
    
    /* Stock les largeur/hauteur des elements du jeu */
    init_real_sizes(img_list, &(para->sizes));
    return 1;
}

/* Libère les images bonus (Surprise donc détail dans Rapport) */
void free_bonus_image(ImgList * img_list) {
    int i;
    for (i=0; i<PROJECT_FRAME; i++) {
        MLV_free_image(img_list->pblaster_charged40[i]);
        MLV_free_image(img_list->pblaster_charged60[i]);
        MLV_free_image(img_list->pblaster_charged80[i]);
        MLV_free_image(img_list->pblaster_charged100[i]);
        MLV_free_image(img_list->bmissile_auto[i]);
        MLV_free_image(img_list->pblaster[i]);
        MLV_free_image(img_list->pmissile[i]);
        MLV_free_image(img_list->bblaster[i]);
    }
    for (i=0; i<PLAYER_FRAME; i++) {
        MLV_free_image(img_list->player[i]);
        MLV_free_image(img_list->player_left[i]);
        MLV_free_image(img_list->player_right[i]);
    }
    for (i=0; i<DEAD_FRAME; i++) {
        MLV_free_image(img_list->boss1_d[i]);
        MLV_free_image(img_list->boss2_d[i]);
        MLV_free_image(img_list->boss3_d[i]);
        MLV_free_image(img_list->boss4_d[i]);
        MLV_free_image(img_list->boss5_d[i]);
        MLV_free_image(img_list->bmissile_auto_d[i]);
        MLV_free_image(img_list->pblaster_d[i]);
        MLV_free_image(img_list->bblaster_d[i]);
        MLV_free_image(img_list->pmissile_d[i]);
        MLV_free_image(img_list->pmissile_d[i+4]);
    }
    for (i=0; i<BOSS_FRAME; i++) {
        MLV_free_image(img_list->boss1[i]);
        MLV_free_image(img_list->boss2[i]);
        MLV_free_image(img_list->boss3[i]);
        MLV_free_image(img_list->boss4[i]);
        MLV_free_image(img_list->boss5[i]);
    }
    
    
}

/* Surprise les informations concernant cette partie sont dans le rapport */
int img_chargement_bonus(ImgList * img_list, Parametre *para) {
    int i;
    double ratio = dmin((para->width / WINDOW_WIDTH), ( para->height / WINDOW_HEIGHT));
    double projectiles_scale = PROJECT_SCALE * ratio;
    double player_scale = PLAYER_SCALE * ratio;
    double boss_scale = BOSS_SCALE * ratio;
    
    free_bonus_image(img_list);
    draw_centered_text(MENU_WIDTH/2, MENU_HEIGHT/4, "Breaking through the space and time!!! ", NULL, MLV_COLOR_WHITE);
    for (i=0; i<PROJECT_FRAME; i++) {
        if ((img_list->pblaster_charged40[i] = load_img(img_list, "data/img/bonus_level/pblasterc", -1)) == NULL) return 0;  else MLV_scale_image(img_list->pblaster_charged40[i], 1.0/35.0);
        if ((img_list->pblaster_charged60[i] = load_img(img_list, "data/img/bonus_level/pblasterc", -1)) == NULL) return 0;  else MLV_scale_image(img_list->pblaster_charged60[i], 1.5/35.0);
        if ((img_list->pblaster_charged80[i] = load_img(img_list, "data/img/bonus_level/pblasterc", -1)) == NULL) return 0;  else MLV_scale_image(img_list->pblaster_charged80[i], 2.0/35.0);
        if ((img_list->pblaster_charged100[i] = load_img(img_list, "data/img/bonus_level/pblasterc", -1)) == NULL) return 0;  else MLV_scale_image(img_list->pblaster_charged100[i], 2.5/35.0);
        if ((img_list->bmissile_auto[i] = load_img(img_list, "data/img/bonus_level/bauto", -1)) == NULL) return 0; else MLV_scale_image(img_list->bmissile_auto[i], projectiles_scale * 6);
        if ((img_list->pblaster[i] = load_img(img_list, "data/img/bonus_level/pblaster/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->pblaster[i], projectiles_scale);
        if ((img_list->pmissile[i] = load_img(img_list, "data/img/bonus_level/pmissile", -1)) == NULL) return 0; else MLV_scale_image(img_list->pmissile[i], projectiles_scale);
        if ((img_list->bblaster[i] = load_img(img_list, "data/img/bonus_level/bblaster", -1)) == NULL) return 0; else MLV_scale_image(img_list->bblaster[i], projectiles_scale);
    }
    for (i=0; i<PLAYER_FRAME; i++) {
        if ((img_list->player[i] = load_img(img_list, "data/img/bonus_level/bor", -1)) == NULL) return 0;  else MLV_scale_image(img_list->player[i], player_scale);
        if ((img_list->player_left[i] = load_img(img_list, "data/img/bonus_level/bor", -1)) == NULL) return 0;  else MLV_scale_image(img_list->player_left[i], player_scale);
        if ((img_list->player_right[i] = load_img(img_list, "data/img/bonus_level/bor", -1)) == NULL) return 0;  else MLV_scale_image(img_list->player_right[i], player_scale);
    }
    for (i=0; i<DEAD_FRAME; i++) {
        if ((img_list->boss1_d[i] = load_img(img_list, "data/img/bonus_level/zip_d", -1)) == NULL) return 0; else MLV_scale_image(img_list->boss1_d[i], boss_scale);
        if ((img_list->boss2_d[i] = load_img(img_list, "data/img/bonus_level/zip_d", -1)) == NULL) return 0; else MLV_scale_image(img_list->boss2_d[i], boss_scale);
        if ((img_list->boss3_d[i] = load_img(img_list, "data/img/bonus_level/zip_d", -1)) == NULL) return 0; else MLV_scale_image(img_list->boss3_d[i], boss_scale);
        if ((img_list->boss4_d[i] = load_img(img_list, "data/img/bonus_level/zip_d", -1)) == NULL) return 0; else MLV_scale_image(img_list->boss4_d[i], boss_scale);
        if ((img_list->boss5_d[i] = load_img(img_list, "data/img/bonus_level/zip_d", -1)) == NULL) return 0; else MLV_scale_image(img_list->boss5_d[i], boss_scale);
        if ((img_list->bmissile_auto_d[i] = load_img(img_list, "data/img/bonus_level/bauto", -1)) == NULL) return 0; else MLV_scale_image(img_list->bmissile_auto_d[i], projectiles_scale * 6);
        if ((img_list->pblaster_d[i] = load_img(img_list, "data/img/bonus_level/pblaster/frame_", i)) == NULL) return 0; else MLV_scale_image(img_list->pblaster_d[i], projectiles_scale / 2);
        if ((img_list->bblaster_d[i] = load_img(img_list, "data/img/bonus_level/bblaster", -1)) == NULL) return 0; else MLV_scale_image(img_list->bblaster_d[i], projectiles_scale * 4);
        if ((img_list->pmissile_d[i] = load_img(img_list, "data/img/bonus_level/pmissile", -1)) == NULL) return 0; else MLV_scale_image(img_list->pmissile_d[i], projectiles_scale * 4);
        if ((img_list->pmissile_d[i+4] = load_img(img_list, "data/img/bonus_level/pmissile", -1)) == NULL) return 0; else MLV_scale_image(img_list->pmissile_d[i+4], projectiles_scale * 4);
    }
    for (i=0; i<BOSS_FRAME; i++) {
        if ((img_list->boss1[i] = load_img(img_list, "data/img/bonus_level/zip", -1)) == NULL) return 0; else MLV_scale_image(img_list->boss1[i], boss_scale);
        if ((img_list->boss2[i] = load_img(img_list, "data/img/bonus_level/zip", -1)) == NULL) return 0; else MLV_scale_image(img_list->boss2[i], boss_scale);
        if ((img_list->boss3[i] = load_img(img_list, "data/img/bonus_level/zip", -1)) == NULL) return 0; else MLV_scale_image(img_list->boss3[i], boss_scale);
        if ((img_list->boss4[i] = load_img(img_list, "data/img/bonus_level/zip", -1)) == NULL) return 0; else MLV_scale_image(img_list->boss4[i], boss_scale);
        if ((img_list->boss5[i] = load_img(img_list, "data/img/bonus_level/zip", -1)) == NULL) return 0; else MLV_scale_image(img_list->boss5[i], boss_scale);
    }
    
    return 1;
}

/* Renvoie une image de 'img_list' correspondant à la taille 'size' passée en paramètre d'un PBlaster_Charged */
MLV_Image* get_blaster_image(ImgList *img_list, int frame, int size) {
    double min = MIN_CHARGING / CHARGING_RATIO;
    double max = MAX_CHARGING / CHARGING_RATIO;
    double ecart = (max - min) / 4.0;  /* On divise l'écart en 4 */
    
    if (size <= min + 1 * ecart) { return img_list->pblaster_charged40[frame]; }
    if (size <= min + 2 * ecart) { return img_list->pblaster_charged60[frame]; }
    if (size <= min + 3 * ecart) { return img_list->pblaster_charged80[frame]; }
    return img_list->pblaster_charged100[frame];
}

/* Renvoie une image de 'img_list' correspondant au 'type' à la 'frame' et à la 'taille' d'un projectile mort */
MLV_Image* get_dead_projectile_image(ImgList *img_list, ProjectileType type, int frame, int size) {
    switch (type) {
        case PBlaster_Charged: return get_blaster_image(img_list, frame, size);
        case BMissile_auto: return img_list->bmissile_auto_d[frame];
        case EMissile_auto: return img_list->emissile_auto_d[frame];
        case EBlaster_sin: return img_list->eblaster_sin_d[frame];
        case PBlaster: return img_list->pblaster_d[frame];
        case PMissile: return img_list->pmissile_d[frame];
        case EBlaster: return img_list->eblaster_d[frame];
        case BBlaster: return img_list->bblaster_d[frame];
        default: return NULL;
    }
}

/* Renvoie une image de 'img_list' correspondant au 'type' à la 'frame' et à la 'taille' d'un projectile */
MLV_Image* get_projectile_image(ImgList *img_list, ProjectileType type, int frame, int size) {
    switch (type) {
        case PBlaster_Charged: return get_blaster_image(img_list, frame, size);
        case BMissile_auto: return img_list->bmissile_auto[frame];
        case EMissile_auto: return img_list->emissile_auto[frame];
        case EBlaster_sin: return img_list->eblaster_sin[frame];
        case PBlaster: return img_list->pblaster[frame];
        case PMissile: return img_list->pmissile[frame];
        case EBlaster: return img_list->eblaster[frame];
        case BBlaster: return img_list->bblaster[frame];
        default: return NULL;
    }
}

/* Renvoie une image de 'img_list' correspondant au 'type' à la 'frame' d'un boss mort */
MLV_Image* get_dead_boss_image(ImgList *img_list, EnemyType type, int frame) {
    switch (type) {
        case Boss1: return img_list->boss1_d[frame];
        case Boss2: return img_list->boss2_d[frame];
        case Boss3: return img_list->boss3_d[frame];
        case Boss4: return img_list->boss4_d[frame];
        case Boss5: return img_list->boss5_d[frame];
        default: return NULL;
    }
}

/* Renvoie une image de 'img_list' correspondant au 'type' à la 'frame' d'un boss */
MLV_Image* get_boss_image(ImgList *img_list, EnemyType type, int frame) {
    switch (type) {
        case Boss1: return img_list->boss1[frame];
        case Boss2: return img_list->boss2[frame];
        case Boss3: return img_list->boss3[frame];
        case Boss4: return img_list->boss4[frame];
        case Boss5: return img_list->boss5[frame];
        default: return NULL;
    }
}

/* Renvoie une image de 'img_list' correspondant au 'type' à la 'frame' d'un ennemi mort */
MLV_Image* get_dead_enemy_image(ImgList *img_list, EnemyType type, int frame) {
    switch (type) {
        case E1:   return img_list->enemy1_d[frame];
        case E2:   return img_list->enemy2_d[frame];
        case E3:   return img_list->enemy3_d[frame];
        case E4:   return img_list->enemy4_d[frame];
        case Special: return img_list->special_d[frame];
        default: return NULL;
    }
    return NULL;
}

/* Renvoie une image de 'img_list' correspondant au 'type' à la 'frame' d'un ennemi */
MLV_Image* get_enemy_image(ImgList *img_list, EnemyType type, int frame) {
    switch (type) {
        case E1:   return img_list->enemy1[frame];
        case E2:   return img_list->enemy2[frame];
        case E3:   return img_list->enemy3[frame];
        case E4:   return img_list->enemy4[frame];
        case Special: return img_list->special[frame];
        default: return NULL;
    }
    return NULL;
}

/* Renvoie une image de 'img_list' correspondant au 'power' à la 'frame' d'un bonus */
MLV_Image* get_bonus_image(ImgList *img_list, Power power, int frame) {
    switch (power) {
        case Score_multiplier: return img_list->bonus_mult[frame];
        case Health_boost: return img_list->bonus_healthboost[frame]; 
        case Single_shot: return img_list->bonus_single[frame]; 
        case Dual_shot: return img_list->bonus_dual[frame]; 
        case Triple_shot: return img_list->bonus_triple[frame]; 
        case Quadra_shot: return img_list->bonus_quad[frame]; 
        case Shield: return img_list->bonus_shield[frame]; 
        case Heal: return img_list->bonus_heal[frame]; 
        case Missile: return img_list->bonus_missile[frame]; 
        case Bullet_time: return img_list->bonus_bullettime[frame]; 
        default: return NULL;
    }
}
/* Enchange l'entier 'a' et l'entier 'b' */
static void swap(int* a, int* b) {
    int t = *a; *a = *b; *b = t;
}

/* Dessine une fleche ancré en ('x','y') de dimention 'w' et 'h', de couleur
 * 'color' et faisant face au Nord (0), Sud (1), Est (2) ou Ouest (3) */
void draw_arrow(int x, int y, int w, int h, MLV_Color color, int orient) {
    int vx[8], vy[8];
    
    if (orient < 2) {
        swap(&x, &y);
        swap(&w, &h);
    }
    if (orient%2 == 0) {
        x += w;
        w *= -1;
    }
    
    vx[0] =       x; vy[0] = y+3*h/8;
    vx[1] = x+3*w/5; vy[1] = y+3*h/8;
    vx[2] = x+3*w/5; vy[2] = y+1*h/8;
    vx[3] =     x+w; vy[3] = y+1*h/2;
    vx[4] = x+3*w/5; vy[4] = y+7*h/8;
    vx[5] = x+3*w/5; vy[5] = y+5*h/8;
    vx[6] =       x; vy[6] = y+5*h/8;
    
    if (orient < 2) {
        MLV_draw_filled_polygon(vy, vx, 7, color);
    } else {
        MLV_draw_filled_polygon(vx, vy, 7, color);
    }
    
}

/* Dessine une fleche coudé ancré en ('x','y') de dimention 'w' et 'h', de couleur
 * 'color' et faisant face au Nord (0, 1), Sud (2, 3), Est (4, 5) ou Ouest (6, 7) */
void draw_turn_arrow(int x, int y, int w, int h, MLV_Color color, int orient) {
    int vx[16], vy[16], l;
    orient = (orient + 4) % 8;
    l = orient >= 4 ? w : h;
    
    if (orient%2) {
        y += h; h *= -1;
    }
    if (orient%4 == 2 || orient%4 == 3) {
        x += w; w *= -1;
    }
    if (orient == 2 || orient == 3 || orient == 5 || orient == 7) {
        l *= -1;
    }
    if (orient >= 4) {
        swap(&w, &h); swap(&x, &y);
    }
    
    vx[0] =       x; vy[0] = y+1*h/8;
    vx[1] = x+3*l/8; vy[1] = y+1*h/8;
    vx[2] = x+3*l/8; vy[2] = y+3*h/8;
    vx[3] = x+3*w/5; vy[3] = y+3*h/8;
    vx[4] = x+3*w/5; vy[4] = y+1*h/8;
    vx[5] =     x+w; vy[5] = y+1*h/2;
    vx[6] = x+3*w/5; vy[6] = y+7*h/8;
    vx[7] = x+3*w/5; vy[7] = y+5*h/8;
    vx[8] =       x; vy[8] = y+5*h/8;
    
    if (orient >= 4) {
        MLV_draw_filled_polygon(vy, vx, 9, color);
    } else {
        MLV_draw_filled_polygon(vx, vy, 9, color);
    }
}


/* Affiche un texte 'text' centré en ('x', 'y') de police 'font' et de couleur 'color' */
void draw_centered_text(int x, int y, const char* text, MLV_Font* font, MLV_Color color) {
    int w, h;
    if (font == NULL) {
        MLV_get_size_of_text(text, &w, &h);
        MLV_draw_text(x-w/2, y-h/2, text, color);
    } else {
        MLV_get_size_of_text_with_font(text, &w, &h, font);
        MLV_draw_text_with_font(x-w/2, y-h/2, text, font, color);
    }
}

/* Dessine une boite de texte centré dans une boite en (MENU_WIDTH/2, y) et de dimention (box_width, box_height)
 * Contenant le texte 'text' de police 'font' et de couleur 'color' */
void draw_centered_textbox(int y, int box_width, int box_height, MLV_Font *font, char* text, MLV_Color color) {
    MLV_draw_filled_rectangle((MENU_WIDTH - box_width)/2, y, box_width, box_height, color);
    draw_centered_text(MENU_WIDTH/2, y + box_height/2, text, font, MLV_COLOR_WHITE);  
}

void draw_cursor(int x, int y, int pv) {
    MLV_Color color = MLV_rgba(255, 215 + 29 * (pv > MAX_HEALTH*2), 200, 255);
    int vx[3], vy[3];
    vx[0] = x  ; vy[0] = y+24;
    vx[1] = x+3; vy[1] = y+30;
    vx[2] = x-3; vy[2] = y+30;
    MLV_draw_filled_polygon(vx, vy, 3, color);
    MLV_draw_filled_rectangle(x-1, y, 3, 30, color);
}

/* Affiche le projectile 'p' sur l'écran */
void draw_projectile(Projectile *p, ImgList *img_list) {
    int frame = (p->internal_clock / FRAME_RATE ) % PROJECT_FRAME;
    int dead_frame = (p->internal_clock / FRAME_RATE / 2);
    MLV_Image *image; 
    
    if (p->alive)                  image = get_projectile_image(img_list, p->type, frame, p->damage);
    else if (p->type == PMissile)  image = get_dead_projectile_image(img_list, p->type, dead_frame % (DEAD_FRAME*2), p->damage);
    else                           image = get_dead_projectile_image(img_list, p->type, dead_frame % DEAD_FRAME, p->damage);
    
    if (image != NULL) 
        MLV_draw_image(image, p->x - p->w/2, p->y - p->h/2);
    else
        MLV_draw_filled_rectangle(p->x - p->w/2, p->y - p->h/2, p->w, p->h, MLV_COLOR_RED);
}

/* Affiche le boss 'e' sur l'écran (width est la largeur de l'écran) */
void draw_boss(Enemy *e, ImgList *img_list, int width) {
    int frame = (e->internal_clock / FRAME_RATE);
    MLV_Image* image;
    
    if (e->damaged)     {image = get_dead_boss_image(img_list, e->type, frame % 2); e->damaged--; }
    else if (e->alive)   image = get_boss_image(img_list, e->type, frame % BOSS_FRAME);
    else                 image = get_dead_boss_image(img_list, e->type, frame % DEAD_FRAME);
    
    if (image != NULL) {
        MLV_draw_image(image, e->x , e->y);
    } else {
        MLV_draw_filled_rectangle(e->x, e->y, e->w, e->h, MLV_COLOR_PURPLE);
    }
    
    /* Lifebar */
    if (e->pv > 0)
        MLV_draw_filled_rectangle(10, 10, e->pv * (width - 20) / enemy_pv(e->type), 30, MLV_COLOR_LIMEGREEN);
    
}

/* Affiche l'ennemi 'e' sur l'écran
 * width est la largeur de l'écran
 * mult vaut 0 si le multiplicateur est inactif 1 sinon */
void draw_enemy(Enemy *e, ImgList *img_list, int width, int mult) {
    int frame = (e->internal_clock / FRAME_RATE);
    MLV_Image* image;
    
    if (Boss1 <= e->type && e->type <= Boss5) {
        draw_boss(e, img_list, width);
        return;
    }
    
    if (e->damaged > 0)     {image = get_dead_enemy_image(img_list, e->type, frame % 2); e->damaged--; }
    else if (e->alive)   image = get_enemy_image(img_list, e->type, frame % ENEMY_FRAME);
    else                 image = get_dead_enemy_image(img_list, e->type, frame % DEAD_FRAME);
    
    if (image != NULL) {
        MLV_draw_image(image, e->x , e->y);
    } else {
        MLV_draw_filled_rectangle(e->x, e->y, e->w, e->h, MLV_COLOR_BLUE);
    }
    /* Lifebar */
    if (e->alive) {
        MLV_draw_filled_rectangle(e->x, e->y - e->h/2 , e->pv * e->w / enemy_pv(e->type), 10, MLV_COLOR_LIMEGREEN);
    } else {
        MLV_draw_text(e->x, e->y-10, "+ %d", MLV_COLOR_WHITE, (100 * (e->type + 1)) * mult);
    }
}

/* Affiche le bonus 'b' sur l'écran */
void draw_bonus(Bonus *b, ImgList *img_list) {
    MLV_Image* image;
    
    int frame = (b->internal_clock / FRAME_RATE ) % BONUS_FRAME;
    image = get_bonus_image(img_list, b->power, frame);
    
    if (image != NULL)
        MLV_draw_image(image, b->x , b->y);
}

/* Affiche le joueur sur l'écran */
void draw_player(Player* p, ImgList *img_list) {
    int frame = (p->internal_clock / FRAME_RATE ) % PLAYER_FRAME;
    MLV_Image* image;

    if (p->vx < 0) {
        image = img_list->player_left[frame];
    } else if (p->vx > 0) {
        image = img_list->player_right[frame];
    } else {
        image = img_list->player[frame];
    }
    
    if (image != NULL)
        MLV_draw_image(image, p->x, p->y);
}

/* Limite la valeur de 'v' à max
 * Renvoie la nouvelle valeur de 'v' */
static int limit_value(int v, int max) {
    if (max < v) return max;
    return v;
}

/* Affiche les informations environantes (vie, missile, score...) */
void draw_stats(Terrain *terrain, Parametre *para, ImgList *img_list) {
    Player *player = terrain->player;
    int i, color;
    /* A: taskbar */
    MLV_draw_filled_rectangle(0, para->height, para->width - 90, 50, MLV_rgba(200, 200, 200, 255));
    MLV_draw_filled_rectangle(0, para->height + 5, para->width - 90, 20, MLV_rgba(230, 230, 230, 255));
    MLV_draw_filled_circle(para->width/6, para->height + 15, 50, MLV_rgba(200, 200, 200, 255));
    MLV_draw_filled_circle(para->width/6, para->height + 20, 40, MLV_rgba(230, 230, 230, 255));
    MLV_draw_filled_circle(para->width/6, para->height + 15, 40, MLV_rgba(20,  20,  20,  255));
    
    /* A: charged blaster */
    if (player->charging >= MIN_CHARGING) {
        color= 250 * ((player->internal_clock /5) %2);
        MLV_draw_filled_circle(para->width/6, para->height + 15, player->charging*40.0/MAX_CHARGING, MLV_rgba(color, 255, 255, 255));
    } else {
        MLV_draw_filled_circle(para->width/6, para->height + 15, player->charging*40.0/MAX_CHARGING, MLV_rgba(50, 50, 50, 255));
    }

    /* A: number of shots */
    for (i = 0; i < 4; i++) {
        MLV_draw_filled_circle(25, para->height+40 - 10*i, 5, (i+1) <= player->perma_bonus ? MLV_rgba(255, 255, 255, 255) : MLV_rgba(20,  20,  20,  255));
    }

    /* A: missiles left */
    for (i = 0; i < MAX_MISSILES; i++) {
        if ((i+1 <= player->nb_missiles)) {
            MLV_draw_filled_rectangle(para->width - 30*(i+1), para->height, 30, 50, MLV_rgba(50,  100, 150, 255));
            MLV_draw_filled_rectangle(para->width - 30*(i+1), para->height, 30, player->m_cooldown/6, MLV_rgba(50, 50, 50, 200));
        }
        else {
            MLV_draw_filled_rectangle(para->width - 30*(i+1), para->height, 30, 50, MLV_rgba(50, 50, 50, 255));
        }
        MLV_draw_image(img_list->over_miss, para->width - 30*(i+1), para->height);
    }

    /* A: barre de vie */
    MLV_draw_filled_rectangle(para->width/4 + 23, para->height + 8, MAX_HEALTH * 3 + 4, 34, MLV_rgba(50,  50,  50,  50));
    MLV_draw_filled_rectangle(para->width/4 + 25, para->height + 10, MAX_HEALTH * 3, 30, MLV_rgba(50,  50,  50,  255));
    
    MLV_draw_filled_rectangle(para->width/4 + 25, para->height + 10, limit_value(player->pv, MAX_HEALTH) * 3, 30, MLV_rgba(250, 50,  150, 255));
    if (player->pv > MAX_HEALTH)
        MLV_draw_filled_rectangle(para->width/4 + 25, para->height + 10, limit_value(player->pv - MAX_HEALTH, MAX_HEALTH) * 3, 30, MLV_rgba(255, 135,  80, 255));
    if (player->pv > 2*MAX_HEALTH)
        MLV_draw_filled_rectangle(para->width/4 + 25, para->height + 10, limit_value(player->pv - 2*MAX_HEALTH, MAX_HEALTH) * 3, 30, MLV_rgba(255, 220,  80, 255));
    if (player->invincible)
        MLV_draw_filled_rectangle(para->width/4 + 25, para->height + 10, MAX_HEALTH * 3, 30, MLV_rgba(150, 255,  80, 255));
    
    for (i=10; i<MAX_HEALTH; i+=10) {
        MLV_draw_filled_rectangle(para->width/4 + 25 +3*i, para->height + 35, 1, 5, MLV_COLOR_WHITE);
    }
    if (player->pv_max > MAX_HEALTH) {
        int amount = player->pv_max-MAX_HEALTH-(player->pv_max>2*MAX_HEALTH?MAX_HEALTH:0);
        draw_cursor(para->width/4 + 25 + amount * 3, para->height + 10, player->pv_max);
    }
    
    /* B: barre de shield  */
    if (player->shield)
        MLV_draw_filled_rectangle(para->width/4 + 25, para->height + 10, player->shield * 10, 10, MLV_rgba(60, 200,  215, 255));
    
    MLV_draw_filled_rectangle(para->width/4 + 25, para->height + 15, MAX_HEALTH * 3, 15, MLV_rgba(255, 255, 255, 50));
    MLV_draw_filled_rectangle(para->width/4 + 25, para->height + 18, MAX_HEALTH * 3, 5,  MLV_rgba(255, 255, 255, 50));
    
    /* Numero du bonus stocké (temporaire) */
    if (terrain->player->stock_bonus != Aucun)
        MLV_draw_image(get_bonus_image(img_list, terrain->player->stock_bonus, (terrain->scroll/2)%BONUS_FRAME), para->width/6-para->sizes.bonus_width/2, para->height + 15-para->sizes.bonus_height/2);
    if (terrain->level != NULL)
        MLV_draw_text_with_font(10, 10, "%d wave%s left", img_list->font, MLV_COLOR_WHITE, terrain->nb_wave, terrain->nb_wave==1?"":"s");
    
    /* A: score */
    if (terrain->score_mult != 0) {
        MLV_draw_text_with_font(para->width/4, para->height-20, "%d", img_list->font, MLV_rgba(240, 75, 165, 255) , terrain->score);
    } else {
        MLV_draw_text_with_font(para->width/4, para->height-20, "%d", img_list->font, MLV_COLOR_WHITE, terrain->score);
    }
}

/* Affiche le fond d'écran */
void draw_background(Parametre* para, ImgList *img_list, int scroll, int lvlnum) {
    MLV_Image* image = img_list->background[lvlnum];
    int i;
    
    if (image == NULL) {            /* Si pas d'image on plante pas mais on affiche un bg en noir */
        MLV_draw_filled_rectangle(0, 0, para->width, para->height, MLV_COLOR_BLACK);
    }
    
    scroll %= para->sizes.background_height;
    for (i=0; (i-1)*para->sizes.background_height <= para->height; i++) {
        MLV_draw_image(image, 0, - (i+1) * para->sizes.background_height + para->height + scroll);
    }
}

/* Affiche TOUT (sans faire d'appel à MLV_actualise_window()) */
void draw_window_noupdate(Parametre *para, Terrain *terrain, ImgList *img_list, int boss) {
    int i;
    
    if (para->background)
        draw_background(para, img_list, 4 * terrain->scroll, terrain->lvlnum);
    else
        MLV_clear_window(MLV_COLOR_BLACK);
    
    for (i=0; i<terrain->nb_projectiles; i++) {
        draw_projectile(&(terrain->projectiles[i]), img_list);
    }
    
    for (i=terrain->nb_enemies-1; i>=0; i--) {
        draw_enemy(&(terrain->enemies[i]), img_list, para->width, 1 + (terrain->score_mult != 0));
    }
    
    for (i=0; i<terrain->nb_bonus; i++) {
        draw_bonus(&(terrain->bonus[i]), img_list);
    }
    
    if (terrain->bullet_time != 0)
        MLV_draw_filled_rectangle(0, 0, para->width, para->height, MLV_rgba(255, 255, 255, 50));
    
    draw_player( terrain->player, img_list);
    draw_stats(terrain, para, img_list);
    
    if (0 < boss && boss < BOSS_COOLDOWN) {
        draw_centered_text(para->width / 2, para->height / 2, "BOSS INCOMING!", img_list->warning_font, MLV_rgba(255, (boss * 10)%256, (boss * 10)%256, 255));
    } 
}

/* Affiche TOUT */
void draw_window(Parametre *para, Terrain *terrain, ImgList *img_list, int boss) {
    draw_window_noupdate(para, terrain, img_list, boss);
    MLV_actualise_window();
}

