#ifndef __SLIDER__
#define __SLIDER__

typedef struct {
    int x, y;
    int w;
    int min, max;   /* [min, max] */
    int value;    
} Slider;

void affiche_slider(Slider* slider, int value);
void init_slider(Slider *slider, int min, int max);
int in_slider(Slider* slider, int x, int y);

int get_value_with_proportion(Slider *slider, int x, int y);

int add_to_slider(Slider *slider, int x);
int remove_from_slider(Slider *slider, int x);
int set_slider_value(Slider *slider, int x);

int modif_slider_pos(Slider *slider, int x, int y);
int modif_slider_width(Slider *slider, int w);
int modif_slider_min(Slider *slider, int min);
int modif_slider_max(Slider *slider, int max);

#endif
