#include <MLV/MLV_all.h>
#include "../include/Parametre.h"
#include "../include/Terrain.h"
#include "../include/Player.h"
#include "../include/Sound.h"

#ifndef STRING_LEN___
#define STRING_LEN___ 128
#endif


#ifndef __AFFICHAGE__
    #define __AFFICHAGE__
    /* Taille de la fenêtre */
    #define WINDOW_WIDTH (700.0)
    #define WINDOW_HEIGHT (900.0)
    #define MENU_WIDTH 400
    #define MENU_HEIGHT 600

    /* Nombre de frame pendant laquelle l'imge d'un objet sera la meme */
    #define FRAME_RATE (4)
    
    /* Nombre total de frame par type d'objet */
    #define PROJECT_FRAME (4)
    #define PLAYER_FRAME (4)
    #define ENEMY_FRAME (8)
    #define BONUS_FRAME (8)
    #define DEAD_FRAME (4)
    #define BOSS_FRAME (16)
    
    
    /* Structure stockant toutes les images nescessaire au fonctionnement. */
    typedef struct {
        /* projectiles */
        MLV_Image* pblaster_charged100[PROJECT_FRAME];
        MLV_Image* pblaster_charged80[PROJECT_FRAME];
        MLV_Image* pblaster_charged60[PROJECT_FRAME];
        MLV_Image* pblaster_charged40[PROJECT_FRAME];
        MLV_Image* emissile_auto[PROJECT_FRAME];
        MLV_Image* bmissile_auto[PROJECT_FRAME];
        MLV_Image* eblaster_sin[PROJECT_FRAME];
        MLV_Image* eblaster[PROJECT_FRAME];
        MLV_Image* pblaster[PROJECT_FRAME];
        MLV_Image* pmissile[PROJECT_FRAME];
        MLV_Image* bblaster[PROJECT_FRAME];
        MLV_Image* emissile_auto_d[DEAD_FRAME];
        MLV_Image* bmissile_auto_d[DEAD_FRAME];
        MLV_Image* eblaster_sin_d[DEAD_FRAME];
        MLV_Image* bblaster_d[DEAD_FRAME];
        MLV_Image* pblaster_d[DEAD_FRAME];
        MLV_Image* eblaster_d[DEAD_FRAME];
        MLV_Image* pmissile_d[DEAD_FRAME*2];
        
        /* bonus */
        MLV_Image* bonus_healthboost[BONUS_FRAME];
        MLV_Image* bonus_bullettime[BONUS_FRAME];
        MLV_Image* bonus_missile[BONUS_FRAME];
        MLV_Image* bonus_triple[BONUS_FRAME];
        MLV_Image* bonus_shield[BONUS_FRAME];
        MLV_Image* bonus_single[BONUS_FRAME];
        MLV_Image* bonus_mult[BONUS_FRAME];
        MLV_Image* bonus_heal[BONUS_FRAME];
        MLV_Image* bonus_dual[BONUS_FRAME];
        MLV_Image* bonus_quad[BONUS_FRAME];
        
        /* player */
        MLV_Image* player[PLAYER_FRAME];
        MLV_Image* player_right[PLAYER_FRAME];
        MLV_Image* player_left[PLAYER_FRAME];
        
        /* ennemies */
        MLV_Image* special_d[DEAD_FRAME];
        MLV_Image* enemy1_d[DEAD_FRAME];
        MLV_Image* enemy2_d[DEAD_FRAME];
        MLV_Image* enemy3_d[DEAD_FRAME];
        MLV_Image* enemy4_d[DEAD_FRAME];
        MLV_Image* boss1_d[DEAD_FRAME];
        MLV_Image* boss2_d[DEAD_FRAME];
        MLV_Image* boss3_d[DEAD_FRAME];
        MLV_Image* boss4_d[DEAD_FRAME];
        MLV_Image* boss5_d[DEAD_FRAME];
        MLV_Image* special[ENEMY_FRAME];
        MLV_Image* enemy1[ENEMY_FRAME];
        MLV_Image* enemy2[ENEMY_FRAME];
        MLV_Image* enemy3[ENEMY_FRAME];
        MLV_Image* enemy4[ENEMY_FRAME];
        MLV_Image* boss1[BOSS_FRAME];
        MLV_Image* boss2[BOSS_FRAME];
        MLV_Image* boss3[BOSS_FRAME];
        MLV_Image* boss4[BOSS_FRAME];
        MLV_Image* boss5[BOSS_FRAME];
        
        /* Background and misc. */
        MLV_Image* up;
        MLV_Image* left;
        MLV_Image* down;
        MLV_Image* button;
        MLV_Image* space;
        MLV_Image* right;
        MLV_Image* over_miss;
        MLV_Image* loading[4];
        MLV_Image* background[NB_MONDE];
        
        /* Fonts... */
        MLV_Font* warning_font;
        MLV_Font* font;
    } ImgList;
    
    MLV_Image* load_img(ImgList *lst, char* path, int frame);
    void free_image(ImgList *img_list);
    void init_real_sizes(ImgList* img_list, Sizes* sizes);
    int img_chargement(ImgList * img_list, Parametre *para);
    void free_bonus_image(ImgList * img_list);
    int img_chargement_bonus(ImgList * img_list, Parametre *para);
    MLV_Image* get_blaster_image(ImgList *img_list, int frame, int size);
    MLV_Image* get_dead_projectile_image(ImgList *img_list, ProjectileType type, int frame, int size);
    MLV_Image* get_projectile_image(ImgList *img_list, ProjectileType type, int frame, int size);
    MLV_Image* get_dead_boss_image(ImgList *img_list, EnemyType type, int frame);
    MLV_Image* get_boss_image(ImgList *img_list, EnemyType type, int frame);
    MLV_Image* get_dead_enemy_image(ImgList *img_list, EnemyType type, int frame);
    MLV_Image* get_enemy_image(ImgList *img_list, EnemyType type, int frame);
    MLV_Image* get_bonus_image(ImgList *img_list, Power power, int frame);
    void draw_arrow(int x, int y, int w, int h, MLV_Color color, int orient);
    void draw_turn_arrow(int x, int y, int w, int h, MLV_Color color, int orient);
    void draw_centered_text(int x, int y, const char* text, MLV_Font* font, MLV_Color color);
    void draw_centered_textbox(int y, int box_width, int box_height, MLV_Font *font, char* text, MLV_Color color);
    void draw_projectile(Projectile *p, ImgList *img_list);
    void draw_boss(Enemy *e, ImgList *img_list, int width);
    void draw_enemy(Enemy *e, ImgList *img_list, int width, int mult);
    void draw_bonus(Bonus *b, ImgList *img_list);
    void draw_player(Player* p, ImgList *img_list);
    void draw_stats(Terrain *terrain, Parametre *para, ImgList *img_list);
    void draw_background(Parametre* para, ImgList *img_list, int scroll, int lvlnum);
    void draw_window_noupdate(Parametre *para, Terrain *terrain, ImgList *img_list, int boss);
    void draw_window(Parametre *para, Terrain *terrain, ImgList *img_list, int boss);
    
    
#endif
