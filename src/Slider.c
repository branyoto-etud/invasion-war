#include "../include/Slider.h"
#include <MLV/MLV_all.h>

int map(int x, int min1, int max1, int min2, int max2) {
    return ( (x - min1) * 1.0 / (max1 - min1) * 1.0 ) * (max2 - min2) + min2;
}

/* Affiche le 'slider'
 * Si 'value' vaut 1 la valeur sera aussi affichée 
 * Si 'value' vaut 0 la valeur ne sera pas affichée */
void affiche_slider(Slider* slider, int value) {
    int i, x = map(slider->value, slider->min, slider->max, slider->x, slider->x+slider->w);

    for (i = -3; i < 3; ++i) {
        MLV_draw_line(slider->x, slider->y + i, slider->x + slider->w, slider->y + i, MLV_COLOR_GREY50);
    }
    MLV_draw_filled_circle(x, slider->y+5, 15, MLV_rgba(0, 0, 0, 50));
    MLV_draw_filled_circle(x, slider->y,   15, MLV_COLOR_WHITE);
    MLV_draw_filled_circle(x, slider->y-2, 10, MLV_rgba(0, 0, 0, 50));
    MLV_draw_filled_circle(x, slider->y,   10, MLV_rgba(125, 50, 200, 255));
    if (value) {
        int w, h;
        MLV_get_size_of_text("%d", &w, &h, slider->value);
        MLV_draw_text(x - w/2, slider->y - h - 5, "%d", MLV_COLOR_BLACK, slider->value);
    }
}

void init_slider(Slider *slider, int min, int max) {
    slider->x = 0;
    slider->y = 0;
    slider->w = max;
    slider->min = min;
    slider->max = max;
    slider->value = min + (min + max) / 2;
}

/* Renvoie 1 si les coos ('x','y') sont dans le slider (avec un écart de 10 pixel de hauteur) 
 *         0 sinon */
int in_slider(Slider* slider, int x, int y) {
    return (slider->x <= x && x <= slider->x + slider->w) &&
            (slider->y - 10 <= y && y <= slider->y + 10);
}


/* Ajoute 'x' à la valeur de 'slider'
 * Renvoie 0 si la valeur obtenue est suppérieur au max du slider et met la valeur du slider à max
 *         1 sinon */
int add_to_slider(Slider *slider, int x) {
    if (slider->value + x > slider->max) {
        slider->value = slider->max;
        return 0;
    }
    slider->value += x;
    return 1;
}
/* Enleve 'x' à la valeur de 'slider'
 * Renvoie 0 si la valeur obtenue est inférieur au min du slider et met la valeur du slider à min
 *         1 sinon */
int remove_from_slider(Slider *slider, int x) {
    if (slider->value - x < slider->min) {
        slider->value = slider->min;
        return 0;
    }
    slider->value -= x;
    return 1;
}
/* Met la valeur de 'slider' à 'x'
 * Renvoie 0 si 'x' n'est pas entre le min et le max du slider (mais met la valeur du slider à la borne la plus proche)
 *         1 sinon */
int set_slider_value(Slider *slider, int x) {
    if (x > slider->max) {
        slider->value = slider->max;
        return 0;
    }
    if (x < slider->min) {
        slider->value = slider->min;
        return 0;
    }
    slider->value = x;
    return 1;
}
/* Met la position du 'slider' aux coordonnées ('x', 'y') */
int modif_slider_pos(Slider *slider, int x, int y) {
    slider->x = x;
    slider->y = y;
    return 1;
}
/* Met la largeur du 'slider' à 'w' >= 0
 * Renvoie 0 si w est < 0
 *         1 sinon */
int modif_slider_width(Slider *slider, int w) {
    if (w < 0) {
        return 0;
    }
    slider->w = w;
    return 1;
}
/* Met le minimum du 'slider' à 'min'
 * Renvoie 0 si le 'min' est suppérieur au max du slider
 *         1 sinon */
int modif_slider_min(Slider *slider, int min) {
    if (min > slider->max)
        return 0;
    slider->min = min;
    return 1;
}
/* Met le maximum du 'slider' à 'max'
 * Renvoie 0 si le 'max' est inférieur au min du slider
 *         1 sinon */
int modif_slider_max(Slider *slider, int max) {
    if (max < slider->min)
        return 0;
    slider->max = max;
    return 1;
}

/* Renvoie la valeur des coordonnées ('x', 'y') dans le 'slider' en fonction de la largeur du slider et de son max
 * Renvoie 0 si les coordonnées ne sont pas dans le slider */
int get_value_with_proportion(Slider *slider, int x, int y) {
    if (!in_slider(slider, x, y))
        return 0;
    return map(x, slider->x, slider->x + slider->w, slider->min, slider->max);
}
