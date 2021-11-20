#include "../include/Fichier.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INT (2147483647)

/** Sauvegarde les paramètres dans le fichier option.dat
 * param 'para' : Pointeur vers les paramètres à sauvegarder
 */
void save_option(Parametre *para) { 
    FILE *out;
    int i;
    
    if (( out = fopen("data/option.dat", "w")) == NULL) {
        fprintf(stderr, "Impossible de créer le fichier option.dat\n");
        return;
    }
    /* Sauvegarde les parametres globaux */
    fprintf(out, "%d %d %d %d %d\n", para->width, para->height, para->sound, para->music, para->background);
    
    /* Sauvegarde les activation de bonus */
    for (i=0; i<NB_BONUS; i++) {
        fprintf(out, "%d ",para->power_up[i]);
    }
    fprintf(out, "\n");
    
    /* Sauvegarde les touches primaires du joueur */
    for (i=0; i<NB_KEY; i++) {
        fprintf(out, "%d ", para->prim_key[i]);
    }
    fprintf(out, "\n");
    
    /* Sauvegarde les touches secondaires du joueur */
    for (i=0; i<NB_KEY; i++) {
        fprintf(out, "%d ", para->sec_key[i]);
    }
    fclose(out);
}

/** Charge les options du fichier option.dat (default_option.dat dans le cas échéant)
 * param 'para' : Pointeur vers la structure à remplir
 * retour : 1 si le fichier est lu correctement
 *          0 si un problème surviens lors de la lecture
 */
int load_option(Parametre *para) {
    FILE* in;
    int i;
    
    /* Ouverture du fichier */
    if (( in = fopen("data/option.dat", "r")) == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier option.dat\n");
        if (( in = fopen("data/default_option.dat", "r")) == NULL) {
            fprintf(stderr, "Impossible d'ouvrir le fichier default_option.dat\n");
            return 0;
        }
    }
    /* Charge les parametres globaux */
    if (fscanf(in, "%d%d%d%d%d", &(para->width), &(para->height), &(para->sound), &(para->music), &(para->background)) != 5) {
        return 0;
    }
    
    /* Charge les activation de bonus */
    for (i=0; i<NB_BONUS; i++) {
        if (fscanf(in, "%d",&(para->power_up[i])) == 0) {
            return 0;
        }
        if (para->power_up[i] != 0 && para->power_up[i] != 1) {
            printf("Valeur non valide\n");
            return 0;
        }
    }
    
    /* Charge les touches primaires du joueur */
    for (i=0; i<NB_KEY; i++) {
        if (fscanf(in, "%d",&(para->prim_key[i])) == 0) {
            return 0;
        }
    }
    
    /* Charge les touches secondaires du joueur */
    for (i=0; i<NB_KEY; i++) {
        if (fscanf(in, "%d",&(para->sec_key[i])) == 0) {
            return 0;
        }
    }
    
    init_sizes(&(para->sizes), 0);
    
    fclose(in);
    return 1;
}

/** Sauvegarde la partie dans le fichier 'fname'.sav
 * param 'p' : Pointeur vers le joueur à sauvegarder
 * param 'fname' : Nom du fichier où sauvegarder les données
 * param 'last_world' : Dernier monde débloqué par le joueur
 * param 'current_world' : Numéro du monde courant (0 si le joueur n'est pas dans un monde)
 * param 'fpos' : Position de la tête de lecture dans le fichier du monde
 * param 'score' : Score atteint par le joueur au moment de la sauvegarde
 */
void save_partie(Player *p, char* fname, int last_world, int current_world, int fpos, int score) {
    char path[STRING_LEN___];
    FILE* out;
    
    sprintf(path, "data/save/%s.sav", fname);
    if ((out = fopen(path, "w")) == NULL) {
        fprintf(stderr, "Fichier %s non ouvrable\n", path);
        return;
    }
    fprintf(out, "%d %d %d %d %d %d %d\n", p->pv, p->pv_max, p->shield,
                p->nb_missiles, p->money, p->perma_bonus, p->stock_bonus);
    fprintf(out, "%d %d %d %d\n", last_world, current_world, fpos, score);
    fclose(out);
}


/** Lit un entier entre des bornes dans le fichier 'in' (ouvert en lecture)
 * param 'in' : Fichier à lire
 * param 'v' : pointeur où stocker la valeur lu
 * param 'min' : Borne minimum de l'entier lu
 * param 'max' : Borne maximum de l'entier lu
 * retour : 1 si l'entier est lu correctement et est bien entre 'min' et 'max'
 *          0 sinon
 */
int fget_int_in_bound(FILE* in, void* v, int min, int max) {
    int tmp;
    if (!fscanf(in, "%d", &tmp))
        return 0;
    *((int*)v) = tmp;
    if (!(min <= tmp && tmp <= max))
        return 0;
    return 1;
}

/** Charge les valeurs du fichier 'fname'.sav
 * param 'p' : Pointeur vers le player où stocker les données
 * param 'fname' : Nom du fichier où lire les données
 * param 'lastworld' : Pointeur où mettre le dernier monde débloqué 
 * param 'currentworld' : Pointeur où mettre le monde en cours (0 si aucun monde en cours)
 * param 'fpos' : Pointeur où mettre la position de la tête de lecture du fichier
 * param 'score' : Pointeur où mettre le score actuel du joueur dans le monde courant
 * retour : 0 si le fichier ne peut pas être ouvert ou qu'une valeur lue est non valide (plus de pv que de pv max)
 *          1 sinon
 */
int load_partie(Player *p, char* fname, int* lastworld, int* currentworld, int* fpos, int* score) {
    char path[STRING_LEN___];
    FILE* in;
    
    sprintf(path, "data/save/%s.sav", fname);
    if ((in = fopen(path, "r")) == NULL) {
        fprintf(stderr, "Fichier %s non trouvé\n", path);
        return 0;
    }
    /* Ne pas changer l'ordre (c'est frustrant je sais) */
    if (!fget_int_in_bound(in,          &(p->pv),           1, MAX_HEALTH_BOOST)) return 0;
    if (!fget_int_in_bound(in,      &(p->pv_max),       p->pv, MAX_HEALTH_BOOST)) return 0;
    if (!fget_int_in_bound(in,      &(p->shield),           0,    SHIELD_AMOUNT)) return 0;
    if (!fget_int_in_bound(in, &(p->nb_missiles),           0,     MAX_MISSILES)) return 0;
    if (!fget_int_in_bound(in,       &(p->money),           0,          MAX_INT)) return 0;
    if (!fget_int_in_bound(in, &(p->perma_bonus), Single_shot,      Quadra_shot)) return 0;
    if (!fget_int_in_bound(in, &(p->stock_bonus),           0,       NB_BONUS-1) && p->stock_bonus != Aucun) return 0;
    
    if (!fget_int_in_bound(in,    lastworld, 0,       4)) return 0;
    if (!fget_int_in_bound(in, currentworld, 0,       5)) return 0;
    if (!fget_int_in_bound(in,         fpos, 0, MAX_INT)) return 0;
    if (!fget_int_in_bound(in,        score, 0, MAX_INT)) return 0;
    
    fclose(in);
    
    return 1;
}

/** Parcours un fichier en cherchant le nombre de vagues
 * param 'file' : Fichier ouvert en lecture contant un monde
 * retour : Le nombre de vague
 */
static int find_nb_wave(FILE* file) {
    long tmp = ftell(file);
    int num, nb_wave = 1;
    while (fscanf(file, "%d", &num) != EOF) {
        if (num == -1) nb_wave++;
    }
    fseek(file, tmp, SEEK_SET);
    return nb_wave;
}

/** Lit le fichier terrain->level et fait apparaitre des ennemis à des moments précis
 * param 'terrain' : Structure contenant le terrain où faire apparaitre les ennemis
 * param 'para' : Structure contenant les paramètres de la partie en cours
 * param 'fname' : Nom du fichier où sauvegarder lors de la lecture d'un checkpoint
 * param 'last_world' : Dernier monde atteint par le joueur
 * retour : 0 si le fichier est corrompu/incomplet
 *          1 si tout se passe bien  */
int read_file(Terrain* terrain, Parametre* para, char* fname, int last_world) {
    static int y_app = -10;
    static int offset = 0;
    int type, pattern, speed, x, y;
    int nb_lu;
    
    if (y_app == -10) {
        if (terrain->score == 0 && fscanf(terrain->level, "%*[^\n]\n") != 0)  {
            fprintf(stderr, "Probleme d'initialisation\n");
            return 0;
        }
        terrain->nb_wave = find_nb_wave(terrain->level);
        if (fscanf(terrain->level, "%d", &y_app) != 1) {
            if (terrain->score == 0 ) {
                fprintf(stderr, "Fichier vide\n");
                return 0;
            }
        }
    }
    if (terrain->lvlstate == 2) { terrain->lvlstate = 1;}
    if (y_app < 0) {
        if (terrain->nb_enemies == 0) {
            save_partie(terrain->player, fname, last_world, terrain->lvlnum+1, ftell(terrain->level), terrain->score); /* A modifier  on connait pas encore num et worldnum */
            offset = terrain->scroll+50;
            terrain->lvlstate = 2;
            terrain->nb_wave--;
            nb_lu = fscanf(terrain->level, "%d", &y_app);
        }
    } else {
        int tmp_w, tmp_h;
        while (y_app >= 0 && terrain->scroll-offset >= y_app) {
            if (fscanf(terrain->level, "%d%d%d%d%d", &type, &pattern, &speed, &x, &y) != 5) {
                fprintf(stderr, "Fichier non complet\n");
                return 0;
            }
            if (rand() % 50 == 0)
                type = Special;
            get_enemy_size(&(para->sizes), type, &tmp_w, &tmp_h);
            add_enemy(terrain, type, pattern, para->width * x / 100.0, para->height * y / 100.0, tmp_w, tmp_h, speed);
            if ((nb_lu = fscanf(terrain->level, "%d", &y_app)) != 1) {
                break;
            }
        }
    }
    if (nb_lu == EOF || fname == NULL) {   /* Fin de fichier */
        fclose(terrain->level);
        terrain->level = NULL;
        terrain->lvlstate = 0;
        y_app = -10;
        offset = 0;
    }
    return 1;
}

/** Ajoute un score et un nom dans le fichier de score du monde courant
 * param 'nom' : Nom du joueur
 * param 'score' : Score du joueur
 * param 'num' : Numéro du monde courant
 */
void add_scoreboard(char *nom, int score, int num){
    char noms[MAX_SCORES][STRING_LEN___];
    int scores[MAX_SCORES];
    int len, i, j;
    
    /* Charger les scores */
    len = load_scoreboard(scores, noms, num);
    
    /* Ajout du score (trié) */
    for (i=0; i<len && score < scores[i]; i++) {;}
    
    
    len = len < MAX_SCORES ? len + 1: MAX_SCORES;
    
    for (j=len-1; j>i; j--) {
        scores[j] = scores[j-1];
        strcpy(noms[j], noms[j-1]);
    }
    
    
    scores[i] = score;
    strcpy(noms[i], nom);
    
    /* Sauvegarde des nouveaux scores */
    if (save_scoreboard(scores, noms, num, len) == 0) {
        printf("Error cannot save scores!\n");
    }
}

/** Sauvegarde des scores et des noms dans le fichier du monde courant
 * param 'scores' : Tableau de scores
 * param 'noms' : Tableau de noms
 * param 'num' : Numéro du monde en cours
 * param 'len' : Taille des tableaux 'scores' et 'noms'
 * retour : 0 si le fichier du monde ne peut pas être ouvert
 *          1 si le score est bien sauvegardé
 */
int save_scoreboard(int scores[], char noms[][STRING_LEN___], int num, int len) {
    FILE *out;
    char path[STRING_LEN___];
    int i;
    
    sprintf(path, "data/scores/monde_%d.dat", num);
    
    if ((out = fopen(path, "w")) == NULL) {
        fprintf(stderr, "Fichier %s non ouvrable\n", path);
        return 0;
    }
    
    len = (len > MAX_SCORES) ? MAX_SCORES : len;
    
    for (i=0; i<len; i++) {
        fprintf(out, "%s %d\n", noms[i], scores[i]);
    }
    
    fclose(out);
    return 1;
}

/** Charge les scores du monde courant
 * param 'scores' : Tableau où sont stocké les scores lu
 * param 'noms' : Tableau où sont stocké les noms lu
 * param 'num' : Numéro du monde courant
 * retour : Le nombre de scores chargés
 *          0 si le fichier est vide ou inexistant
 */
int load_scoreboard(int scores[], char noms[][STRING_LEN___], int num) {
    FILE *in;
    char path[STRING_LEN___];
    int i = 0;
    
    sprintf(path, "data/scores/monde_%d.dat", num);
    
    if ((in = fopen(path, "r")) == NULL) {
        fprintf(stderr, "Fichier %s non trouvé\n", path);
        return 0;
    }
    /* Faire un for?? */
    while (i < MAX_SCORES && fscanf(in, "%s%d", noms[i], &(scores[i])) == 2) {
        /* printf("%d : %s %d\n", i, noms[i], scores[i]); */
        i++;
    }
    
    fclose(in);
    return i;
}
