#include "../include/Affichage.h"
#include "../include/Bonus.h"


/* Echange la valeur du pointeur 'a' avec la valeur du pointeur 'b'
 * param 'a' : Premier Bonus à échanger
 * param 'b' : Second Bonus à échanger
 */
static void swap(Bonus* a, Bonus* b) {
    Bonus t = *a;
    *a = *b;
    *b = t;
}

/* Initialise les valeurs de 'bonus'
 * param 'bonus' : Pointeur de la structure à initialiser
 * param 'power' : Effet produit par le bonus 
 * param 'x' : Abscisse du centre du bonus 
 * param 'y' : Ordonnee du centre du bonus 
 * param 'rayon' : Rayon du bonus
 */
void init_bonus(Bonus* bonus, Power power, int x, int y, int rayon) {
    bonus->x = x-rayon/2;
    bonus->y = y-rayon/2;
    bonus->r = rayon;
    bonus->alive = 1;
    bonus->power = power;
    bonus->internal_clock = 0;
}

/* Met à jour la position du bonus 'b'
 * param 'b' : Bonus à mettre à jour
 * param 'window_height' : Hauteur de la fenetre de jeu
 * retour : 0 si le bonus atteint le bas de l'écran
 *          1 sinon
 */
int bonus_update(Bonus* b, int window_height) {
    /* 1 frame sur 4 */
    b->y += 2;
    
    if (b->y > window_height) {
        b->alive = 0;
        /* printf("Hit border\n"); */
        return 0;
    }
    return 1;
}

/* Parcours le tableau 'tab' de 'n' elements et décale les bonus morts à la fin du tableau
 * param 'tab : Tableau de Bonus 
 * param 'n' : Nombre de bonus contenus dans le tableau
 * retour : la taille du tableau après suppression des bonus morts
 */
int bonus_remove(Bonus tab[], int n) {
    int i, j;
    
    /* Ne peut pas être inférieur à 0 */
    if (n <= 0)
        return 0;
    /* S'il y en a qu'un on renvoie 1 s'il est en vie et 0 s'il est mort (ou p->alive c'est la meme)*/
    if (n == 1)
        return tab[0].alive;
    
    /* Parcours le tableau */
    for (i=0,j=0; i < n && j < n; i++, j++) {
        /* Si l'élément courant est à supprimer */
        if (!tab[i].alive) {
            /* Si l'élément ciblé est aussi à supprimé on augmente l'écartement entre i et j */
            if (!tab[j].alive) {i--;}
            /* Sinon on échange les deux */
            else {swap(tab+i,tab+j);}
        }
    }
    /* On renvoie la nouvelle taille du tableau */
    return n - (j-i);
}

/* Permet de savoir la valeur de 'power'
 * param 'power' : Effet dont on veut savoir le prix
 * retour : La valeur correpondante à 'power'
 *          -1 s'il n'a pas de valeur 
 */
int get_bonus_price(Power power) {
    switch (power) {
        case Score_multiplier: return 50000;
        case Health_boost: return 30000; 
        case Single_shot: return 1000; 
        case Dual_shot: return 5000; 
        case Triple_shot: return 15000; 
        case Quadra_shot: return 30000; 
        case Shield: return 10000; 
        case Heal: return 5000; 
        case Missile: return 100000; 
        case Bullet_time: return 50000; 
        default: return -1;
    }
}

/* Permet d'avoir une courte description de 'power'
 * param 'power' : Effet dont on veut avoir la description
 * retour : La description correpondante à 'power'
 *          Une chaine vide s'il n'a pas de description
 */
const char* get_bonus_description(Power power) {
    switch (power) {
        case Score_multiplier: return "Double les points reçu durant 500 frames.";
        case Health_boost: return "Augmente de 20 points votre maximum \nde point de vie."; 
        case Single_shot: return "Change votre arme pour tirer un seul tir puissant."; 
        case Dual_shot: return "Change votre arme pour tirer deux tirs fort.";
        case Triple_shot: return "Change votre arme pour tirer trois tirs moyen."; 
        case Quadra_shot: return "Change votre arme pour tirer quatres tirs faible."; 
        case Shield: return "Activation d'un bouclier pouvant absorber \n30 points de dégats."; 
        case Heal: return "Reparation du vaisseau vous rendant \n20 points de vie."; 
        case Missile: return "Recharge 1 missile dans votre vaisseau."; 
        case Bullet_time: return "Ralenti tout autours de vous pendant 250 frames."; 
        default: return "";
    }
}
