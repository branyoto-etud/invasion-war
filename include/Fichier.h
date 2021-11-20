#include "../include/Player.h"
#include "../include/Terrain.h"
#include "../include/Parametre.h"

#ifndef STRING_LEN___
#define STRING_LEN___ 128
#endif

#ifndef __FICHIER__
    #define __FICHIER__
    
    #define MAX_SCORES (100)
    
    void save_option(Parametre *para);
    int load_option(Parametre *para);
    
    void save_partie(Player *p, char *fname, int lastworld, int currentworld, int fpos, int score);
    int load_partie(Player *p, char *fname, int* lastworld, int* currentworld, int* fpos, int *score);
    
    int read_file(Terrain *terrain, Parametre *para, char* fname, int last_world);
    
    int load_scoreboard(int scores[], char noms[][STRING_LEN___], int num);
    int save_scoreboard(int scores[], char noms[][STRING_LEN___], int num, int len);
    void add_scoreboard(char *nom, int score, int num);
    
#endif
