#include "../include/Menu.h"
#include "../include/Bonus.h"
#include "../include/Slider.h"
#include "../include/Tutoriel.h"
#include "../include/Affichage.h"

Star* alloue_star(int min, int max, int y) {
    Star* newStar = (Star*) malloc(sizeof(Star));
    if (newStar == NULL) {
        printf("   ERREUR D'ALLOCATION \n la star n'a pas pu être ajoutée :(");
        return NULL;
    }

    newStar->y = y;
    newStar->x = (rand() %(max-min)) + min;
    newStar->size = rand() %2 +1;
    newStar->speed = rand() %6 +1;
    newStar->alpha = (rand()%4 +1) *50;
    return newStar;
}
void fill_star_tab(Star** tab, int width, int height, int taille) {
    int i;
    for (i= 0; i < taille; i++) {
        tab[i] = alloue_star(0, width, rand()% height);
    }
}
void update_stars(Star** tab, int width, int height, int taille) {
    int i;
    for (i= 0; i < taille; i++) {
        if (tab[i]->y > height) {
            tab[i] = alloue_star(0, width, 0);
            break;
        }
        tab[i]->y += tab[i]->speed;
    }
}
void draw_stars(Star** tab, int taille) {
    int i;
    for (i= 0; i < taille; i++) {
        MLV_draw_filled_circle(tab[i]->x, tab[i]->y, tab[i]->size, MLV_rgba(255, 255, 255, tab[i]->alpha));
    }
}


/* Renvoie 1 si le clic (mx, my) est dans la boite ancré en (bx, by)
 * et de dimention bw et bh
 * 0 sinon */
int in_box(int mx, int my, int bx, int by, int bw, int bh) {
    return (bx <= mx && mx <= bx + bw) && (by <= my && my <= by + bh);
}

void select_holded_button(MLV_Event event, MLV_Mouse_button button, MLV_Button_state state, Selection hover, int *pressed, Selection *holded, Selection *select) {
    if (event == MLV_MOUSE_BUTTON && button == MLV_BUTTON_LEFT) {
        if (state == MLV_PRESSED) {
            if (holded != NULL) *holded = hover;
            if (pressed != NULL) *pressed = 1;
        } else {
            if (select != NULL) *select = hover;
            if (pressed != NULL) *pressed = 0;
        }
    }
}

/* Renvoie 0 pour quiter le menu 1 sinon */
int new_player(char* name, int taken) {
    Selection hover, holded = None, select = None;
    MLV_Keyboard_button key;
    MLV_Input_box *input_box;
    MLV_Mouse_button button;
    MLV_Button_state state;
    int x, y, pressed;
    MLV_Event event;
    MLV_Image *quit;
    MLV_Font  *font;
    char *text;

    if ((quit = MLV_load_image("data/img/icons/quit.png")) == NULL) return 0; else MLV_resize_image(quit, MENU_WIDTH/6 - 20, MENU_WIDTH/6 - 20);
    if ((font = MLV_load_font("data/fonts/cyberlink.ttf", 20)) == NULL) return 0;
    MLV_get_mouse_position(&x, &y);
    pressed = 0;

    /* on initialise l'input box */
    input_box = MLV_create_input_box_with_font(MENU_WIDTH/4, MENU_HEIGHT/2 - 40, MENU_WIDTH/2, MENU_HEIGHT/10, 
             MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_rgba(25, 25, 25, 255), "", font);
    MLV_activate_input_box(input_box);
    
    do {
        MLV_clear_window(MLV_COLOR_BLACK);
        draw_centered_text(MENU_WIDTH/2, MENU_HEIGHT/2 - 90, "Entrez votre nom!", font, MLV_COLOR_WHITE);
        if (taken) {
            draw_centered_text(MENU_WIDTH/2, MENU_HEIGHT/2 - 60, "Ce nom est déjà pris!", font, MLV_COLOR_RED);
        }
        /* bouton démarrer */
        MLV_draw_adapted_text_box_with_font(MENU_WIDTH/4, MENU_HEIGHT*2/3, "Appuyez sur 'Entrée'\npour commencer!",
          font, MLV_rgba(0, 0, 0, 0), 50, MLV_rgba(155, 225, 170, 255), MLV_rgba(0, 0, 0, 255), MLV_HORIZONTAL_CENTER);

        /* bouton quitter */
        MLV_draw_filled_rectangle(MENU_WIDTH*5/6, MENU_HEIGHT - MENU_WIDTH/6, MENU_WIDTH/6, MENU_WIDTH/6, MLV_rgba(255, 255 - 150*(hover), 255 - 125*(hover), 100 + 50*(hover) + 50*(state == MLV_PRESSED && hover == Quit)));
        MLV_draw_image(quit, MENU_WIDTH*5/6 + 10, MENU_HEIGHT - MENU_WIDTH/6 + 10);
        
        MLV_draw_all_input_boxes();
        MLV_actualise_window();

        event = MLV_get_event(&key, NULL, NULL, &text, &input_box, &x, &y, &button, &state);
        if (MENU_HEIGHT - MENU_WIDTH/6 < y && MENU_WIDTH*5/6 < x) {
            hover = Quit;
        } else {
            hover = None;
        }
        
        select_holded_button(event, button, state, hover, &pressed, &holded, &select);
        if (holded == Quit && select == holded && !pressed) {
            MLV_free_input_box(input_box);
            return 0;
        }
        if (event == MLV_KEY && key == MLV_KEYBOARD_ESCAPE && state == MLV_PRESSED) {
            return 0;
        }
        if (event == MLV_INPUT_BOX && strlen(text) > 0) {
            strcpy(name, text);
            free(text);
            break;
        }
    } while (1);

    MLV_actualise_window();
    MLV_free_font(font);
    MLV_free_image(quit);
    MLV_free_input_box(input_box);
    return 1;
}

/* Renvoie la couleur correspondant au rang 'pos' avec un alpha 'alpha' */
MLV_Color rank(int pos, int alpha) {
    switch(pos) {
        case 1:
        return MLV_rgba(235, 215, 100, alpha);

        case 2:
        return MLV_rgba(200, 200, 230, alpha);

        case 3:
        return MLV_rgba(150, 100, 50, alpha);

        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        return MLV_rgba(20, 160, 170, alpha);

        default:
        return MLV_rgba(170, 200, 200, alpha);
    }
}

MLV_Color set_player_color(int numpage, int alpha) {
    return MLV_rgba((100*numpage) %256, (150*numpage) %256, (200*numpage) %256, alpha);
}

int get_bonus_path(char* path_bonus, int stock, int frame) {
    switch (stock) {
        case Heal:
            sprintf(path_bonus, "data/img/bonus/bonus_heal/frame_%d.png", (frame/8) %8);
            return 1;
        case Shield:
            sprintf(path_bonus, "data/img/bonus/bonus_shield/frame_%d.png", (frame/8) %8);
            return 1;
        case Missile:
            sprintf(path_bonus, "data/img/bonus/bonus_missile/frame_%d.png", (frame/8) %8);
            return 1;
        case Bullet_time:
            sprintf(path_bonus, "data/img/bonus/bonus_bullettime/frame_%d.png", (frame/8) %8);
            return 1;
        case Score_multiplier:
            sprintf(path_bonus, "data/img/bonus/bonus_mult/frame_%d.png", (frame/8) %8);
            return 1;
        default:
            return 0;
    }
}

void load_unicodes(char tab[][STRING_LEN___], Parametre* para) {
  set_unicode(tab[Left_key],       para->prim_key[Left_key]);
  set_unicode(tab[Right_key],      para->prim_key[Right_key]);
  set_unicode(tab[Up_key],         para->prim_key[Up_key]);
  set_unicode(tab[Down_key],       para->prim_key[Down_key]);
  set_unicode(tab[Blaster_key],    para->prim_key[Blaster_key]);
  set_unicode(tab[Charged_key],    para->prim_key[Charged_key]);
  set_unicode(tab[Missile_key],    para->prim_key[Missile_key]);
  set_unicode(tab[Bonus_key],      para->prim_key[Bonus_key]);
  set_unicode(tab[Left_key +8],    para->sec_key[Left_key]);
  set_unicode(tab[Right_key +8],   para->sec_key[Right_key]);
  set_unicode(tab[Up_key +8],      para->sec_key[Up_key]);
  set_unicode(tab[Down_key +8],    para->sec_key[Down_key]);
  set_unicode(tab[Blaster_key +8], para->sec_key[Blaster_key]);
  set_unicode(tab[Charged_key +8], para->sec_key[Charged_key]);
  set_unicode(tab[Missile_key +8], para->sec_key[Missile_key]);
  set_unicode(tab[Bonus_key +8],   para->sec_key[Bonus_key]);
}

void set_unicode(char* str, MLV_Keyboard_button touche) {
  char* tmp = MLV_convert_unicode_to_string(touche);

  switch(touche) {
    case 9:
      sprintf(str, "Tab");
      break;
    case 38:
      sprintf(str, "1");
      break;
    case 233:
      sprintf(str, "2");
      break;
    case 34:
      sprintf(str, "3");
      break;
    case 39:
      sprintf(str, "4");
      break;
    case 40:
      sprintf(str, "5");
      break;
    case 45:
      sprintf(str, "6");
      break;
    case 232:
      sprintf(str, "7");
      break;
    case 95:
      sprintf(str, "8");
      break;
    case 231:
      sprintf(str, "9");
      break;
    case 224:
      sprintf(str, "0");
      break;
    case 32:
      sprintf(str, "Space");
      break;
    case 13:
      sprintf(str, "Enter");
      break;
    case 276:
      sprintf(str, "Left");
      break;
    case 273:
      sprintf(str, "Up");
      break;;
    case 275:
      sprintf(str, "Right");
      break;
    case 274:
      sprintf(str, "Down");
      break;
    default:
      sprintf(str, tmp);
  }
  free(tmp);
}

int set_new_key(Parametre* para, MLV_Keyboard_button key, Selection select) {
  int i;
  if ((256 <= key && key <= 267) || (301 <= key && key <= 308) || (282 <= key && key <= 293)) {
    return 0;
  }
  if (key == 8 || key == 27 || key == 127 || key == 313 || key == 178 || key == 0) {
    return 0;
  }
  for (i = 0; i < NB_KEY; i++) {
    if (para->prim_key[i] == key || para->sec_key[i] == key) {
      return 0;
    }
  }

  switch(select) {
    case Left:
      para->prim_key[Left_key] = key;
      break;
    case Rght:
      para->prim_key[Right_key] = key;
      break;
    case Up:
      para->prim_key[Up_key] = key;
      break;
    case Down:
      para->prim_key[Down_key] = key;
      break;
    case Blst:
      para->prim_key[Blaster_key] = key;
      break;
    case Chrg:
      para->prim_key[Charged_key] = key;
      break;
    case Miss:
      para->prim_key[Missile_key] = key;
      break;
    case Bons:
      para->prim_key[Bonus_key] = key;
      break;
    case Left2:
      para->sec_key[Left_key] = key;
      break;
    case Rght2:
      para->sec_key[Right_key] = key;
      break;
    case Up2:
      para->sec_key[Up_key] = key;
      break;
    case Down2:
      para->sec_key[Down_key] = key;
      break;
    case Blst2:
      para->sec_key[Blaster_key] = key;
      break;
    case Chrg2:
      para->sec_key[Charged_key] = key;
      break;
    case Miss2:
      para->sec_key[Missile_key] = key;
      break;
    case Bons2:
      para->sec_key[Bonus_key] = key;
      break;
    default:
      return 0;
  }
  return 1;
}


/* Retire le 'data/save' et le '.sav' de 'nom' */
void remove_path(char* nom) {
  int i, shift = 10;

  for (i = 0; nom[i + shift] != '.' && nom[i + shift]; i++) {
    nom[i] = nom[i + shift];
  }
  nom[i++] = ' ';
  nom[i] = '\0';
}

/* Revoie une cellule PlayerStats contenant 'nom'
 * et n'ayant pas de suivant ni de précédent */
Liste alloueCellule(char* nom) {
    Liste newCell = (Liste) malloc(sizeof(PlayerStats));
    assert(nom != NULL);

    if (newCell == NULL) {
        printf("   ERREUR D'ALLOCATION \n la cellule '%s' n'a pas pu être ajoutée :(", nom);
        return NULL;
    }

    strcpy(newCell->nom, nom);
    newCell->next = NULL;
    newCell->prev = NULL;
    return newCell;
}

void ajouteFinListe(Liste* lst, PlayerStats *cell) {
    assert(lst != NULL);

    if (cell == NULL) {
        return;
    }

    if (*lst == NULL) {
        *lst = cell;
        return;    
    }
    cell->prev = *lst;
    ajouteFinListe(&(*lst)->next, cell);
}

/* Fonction auxiliaire de ftw.
 * Stock le fichier lu dans le fichier 'OUT'
 * pour pouvoir le réutiliser plus tard 
 * Ne stock que les fichier */
int write_out(const char* fpath, const struct stat *sb, int typeflag) {
  FILE* fichier;
  char player_name[STRING_LEN___];
    
  if (typeflag != FTW_F) {
      return 0;
  }
  fichier = fopen(OUT, "a");

  sprintf(player_name, "%s", fpath);
  remove_path(player_name);
  fputs(player_name, fichier);
  fclose(fichier);
  return 0;
}

/* Lit le fichier 'OUT' et enregistre dans la liste 'lst' les noms
 * de fichier trouvés.
 * Renvoie la taille de 'lst' */
int out_to_list(Liste* lst) {
    int n = 0;
    Liste tmp;
    char nom[STRING_LEN___];
    FILE* out= fopen(OUT, "r");

    /* tant que le fichier n'est pas lu entièrement, */
    while (fscanf(out, "%s ", nom) != EOF) {
        ajouteFinListe(lst, alloueCellule(nom));
        n++;
    }
    if (*lst == NULL) {
        return 0;
    }
    
    /* on lie le premier maillon avec le dernier */
    tmp = *lst;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }
    (*lst)->prev = tmp;
    tmp->next = *lst;

    return n;
}

void afficheListe(Liste lst) {
    /* fonction d'affichage d'une Liste en fonction du champ 'suivant' */
    while (lst != NULL) {
        printf("[%s]-->", lst->nom);
    }
    printf("\n");
}

/* HOVER */
void hover_menu_prcp(Selection *hover, int x, int y) {
    int margin = MENU_HEIGHT*3/8,
        padding = MENU_HEIGHT/40,
        box_width = MENU_WIDTH/2,
        box_height = MENU_HEIGHT/10;
    if ((MENU_WIDTH - box_width)/2 < x && x < (MENU_WIDTH - box_width)/2 + box_width) {
        if (margin < y && y < margin + box_height) {
            *hover = Nouv;
        }
        else if (margin + padding + box_height < y && y < margin + padding + box_height*2) {
            *hover = Chrg;
        }
        else if (margin + (padding + box_height)*2 < y && y < margin + padding*2 + box_height*3) {
            *hover = Lead;
        }
        else {
            *hover = None;
        }
        return;
    }
    else if (MENU_HEIGHT - MENU_WIDTH/6 < y) {
        if (0 < x && x < MENU_WIDTH/6) {
            *hover = Sett;
        }
        else if (MENU_WIDTH*5/6 < x) {
            *hover = Quit;
        }
        else {
            *hover = None;
        }
        return;
    }
    else {
        *hover = None;
    }    
}

void hover_menu_chrg(Selection *hover, int x, int y) {
    if (y < 50) {
        if (x < 50) {
            *hover = Left;
        }
        else if (MENU_WIDTH - 50 < x) {
            *hover = Rght;
        }
        else {
            *hover = None;
        }
        return;
    }
    else if (MENU_HEIGHT-75 < y && y < MENU_HEIGHT-25 && MENU_WIDTH/4 < x && x < MENU_WIDTH*3/4) {
        *hover = Chrg;
    }
    else if (MENU_HEIGHT - MENU_WIDTH/6 < y && MENU_WIDTH*5/6 < x) {
        *hover = Quit;
    }
    else {
        *hover = None;
    }    
}

void hover_menu_mnde(Selection *hover, int x, int y) {
    if (75 <= y && y <= 175) {
        if (x < 50) {
            *hover = Left;
        }
        else if (MENU_WIDTH - 50 < x) {
            *hover = Rght;
        }
        else {
            *hover = None;
        }
        return;
    }
    else if (MENU_HEIGHT-75 < y && y < MENU_HEIGHT-25 && MENU_WIDTH/4 < x && x < MENU_WIDTH*3/4) {
        *hover = Chrg;
    }
    else if (MENU_HEIGHT - MENU_WIDTH/6 < y && MENU_WIDTH*5/6 < x) {
        *hover = Quit;
    }
    else {
        *hover = None;
    }    
}

void hover_menu_ldbd(Selection *hover, int x, int y) {
    if (y < MENU_HEIGHT/20 - 5) {
        if (x < MENU_WIDTH/15) {
            *hover = Left;
        }
        else if (MENU_WIDTH*14/15 < x) {
            *hover = Rght;
        }
        else {
            *hover = None;
        }
    }
    else if (MENU_HEIGHT - MENU_WIDTH/6 < y && MENU_WIDTH*5/6 < x) {
        *hover = Quit;
    }
    else {
        *hover = None;
    }
}

void hover_menu_optn(Selection *hover, int ingame, int x, int y) {
    if (75 <= y && y < 235 && MENU_WIDTH/2 < x && x < MENU_WIDTH*3/4 - 20) {
      if (y < 95) {
        *hover = Left;
      }
      else if (95 <= y && y < 115) {
        *hover = Rght;
      }
      else if (115 <= y && y < 135) {
        *hover = Up;
      }
      else if (135 <= y && y < 155) {
        *hover = Down;
      }
      else if (155 <= y && y < 175) {
        *hover = Blst;
      }
      else if (175 <= y && y < 195) {
        *hover = Chrg;
      }
      else if (195 <= y && y < 215) {
        *hover = Miss;
      }
      else if (215 <= y) {
        *hover = Bons;
      }
      else {
        *hover = None;
      } 
    }
    else if (75 <= y && y < 235 && MENU_WIDTH*3/4 < x && x < MENU_WIDTH - 20) {
      if (y < 95) {
        *hover = Left2;
      }
      else if (95 <= y && y < 115) {
        *hover = Rght2;
      }
      else if (115 <= y && y < 135) {
        *hover = Up2;
      }
      else if (135 <= y && y < 155) {
        *hover = Down2;
      }
      else if (155 <= y && y < 175) {
        *hover = Blst2;
      }
      else if (175 <= y && y < 195) {
        *hover = Chrg2;
      }
      else if (195 <= y && y < 215) {
        *hover = Miss2;
      }
      else if (215 <= y) {
        *hover = Bons2;
      }
      else {
        *hover = None;
      } 
    }
    else if (MENU_HEIGHT*2/3 + 30 <= y && y <= MENU_HEIGHT*2/3 + 120 && 125 <= x && x <= MENU_WIDTH -25) {
      if (y <= MENU_HEIGHT*2/3 + 70) {
        *hover = Musc;        
      }
      else if (MENU_HEIGHT*2/3 + 80 <= y) {
        *hover = Sons;        
      }
      else {
        *hover = None;
      }  
    }
    else if (!ingame && 295 <= y && y <= 395) {
      if (25 <= x && x <= 125) {
        *hover = R600;
      }
      else if (150 <= x && x <= 250) {
        *hover = R700;        
      }
      else if (275 <= x && x <= 375) {
        *hover = R800;        
      }
      else {
        *hover = None;
      } 
    }
    else if (MENU_HEIGHT - MENU_WIDTH/6 < y && MENU_WIDTH*5/6 < x) {
        *hover = Quit;
    }
    else {
        *hover = None;
    }    
}

void hover_menu_pause(Selection *hover, int x, int y, int width, int height) {
    if (width/4 <= x && x <= width*3/4) {
        if (height*3/8 <= y && y <= height*3/8 + height/10) {
            *hover = Strt;
        }
        else if (height*4/8 <= y && y <= height*4/8 + height/10) {
            *hover = Sett;
        }
        else if (height*5/8 <= y && y <= height*5/8 + height/10) {
            *hover = Quit;
        }
        else {
            *hover = None;
        }
    }
    else {
        *hover = None;
    }
}


/* AFFICHAGE */
void draw_menu_prcp(Selection hover, int frame, MLV_Button_state state, int save_found) {
    char path[STRING_LEN___];
    MLV_Font *font;
    MLV_Image *settings, *quit, *logo;

    sprintf(path, "data/img/icons/logo/frame_%d.png", (frame/32)%4);
    if ((settings = MLV_load_image("data/img/icons/settings.png")) == NULL) return; else MLV_resize_image(settings, MENU_WIDTH/6 - 20, MENU_WIDTH/6 - 20);
    if ((quit = MLV_load_image("data/img/icons/quit.png")) == NULL) return; else MLV_resize_image(quit, MENU_WIDTH/6 - 20, MENU_WIDTH/6 - 20);
    if ((logo = MLV_load_image(path)) == NULL) return; else MLV_resize_image(logo, MENU_WIDTH/2, MENU_WIDTH/2);
    if ((font = MLV_load_font("data/fonts/edunline.ttf", 20)) == NULL) return;

    MLV_draw_image(logo, MENU_WIDTH/4, 25);
    
    draw_centered_textbox(MENU_HEIGHT*3/8, MENU_WIDTH/2 - 20*(state == MLV_PRESSED && hover == Nouv), MENU_HEIGHT/10, font, "NOUVELLE PARTIE", MLV_rgba(220, 180 - 50*(hover == Nouv), 250, 110 + 40*(hover == Nouv)));
    draw_centered_textbox(MENU_HEIGHT*4/8, MENU_WIDTH/2 - 20*(state == MLV_PRESSED && hover == Chrg), MENU_HEIGHT/10, font, "CHARGER",         MLV_rgba(220, 180 - 50*(hover == Chrg), 250, 110 + 40*(hover == Chrg)));
    draw_centered_textbox(MENU_HEIGHT*5/8, MENU_WIDTH/2 - 20*(state == MLV_PRESSED && hover == Lead), MENU_HEIGHT/10, font, "LEADERBOARD",     MLV_rgba(220, 180 - 50*(hover == Lead), 250, 110 + 40*(hover == Lead)));

    MLV_draw_filled_rectangle(0, MENU_HEIGHT - MENU_WIDTH/6, MENU_WIDTH/6, MENU_WIDTH/6, MLV_rgba(255 - 100*(hover == Sett), 255, 255, 100 + 50*(hover == Sett) + 50*(state == MLV_PRESSED && hover == Sett)));
    MLV_draw_image(settings, 10, MENU_HEIGHT - MENU_WIDTH/6 + 10);

    MLV_draw_filled_rectangle(MENU_WIDTH*5/6, MENU_HEIGHT - MENU_WIDTH/6, MENU_WIDTH/6, MENU_WIDTH/6, MLV_rgba(255, 255 - 150*(hover == Quit), 255 - 125*(hover == Quit), 100 + 50*(hover == Quit) + 50*(state == MLV_PRESSED && hover == Quit)));
    MLV_draw_image(quit, MENU_WIDTH*5/6 + 10, MENU_HEIGHT - MENU_WIDTH/6 + 10);

    if (!save_found) {
        draw_centered_text(MENU_WIDTH/2, MENU_HEIGHT*13/16, "Aucune sauvegarde trouvée.", font,  MLV_COLOR_WHITE);
    }

    MLV_actualise_window();
    MLV_free_font(font);
    MLV_free_image(logo);
    MLV_free_image(quit);
    MLV_free_image(settings);
}

void draw_menu_chrg(Player player, char* nom, int currentworld, int score, int pages, int numpage, int frame, Selection hover, MLV_Image *bg) {
    int i, r, g, b, bon;
    char path_ship[STRING_LEN___],
         path_miss[STRING_LEN___],
         path_bonus[STRING_LEN___];
    MLV_Font *font;
    MLV_Image *quit,
              *ship,
              *bonus,
              *missile;

    sprintf(path_ship, "data/img/player/spaceship/frame_%d.png", (frame/8)%4);
    sprintf(path_miss, "data/img/projectiles/pmissile/frame_%d.png", (frame/8)%4);
    bon = get_bonus_path(path_bonus, player.stock_bonus, frame);
    if ((ship = MLV_load_image(path_ship)) == NULL) return; else MLV_resize_image(ship, 40, 40);
    if ((missile = MLV_load_image(path_miss)) == NULL) return; else MLV_resize_image(missile, 25, 50);
    if ((quit = MLV_load_image("data/img/icons/quit.png")) == NULL) return; else MLV_resize_image(quit, MENU_WIDTH/6 - 20, MENU_WIDTH/6 - 20);
    if ((font = MLV_load_font("data/fonts/cyberlink.ttf", 20)) == NULL) return;
    
    MLV_clear_window(MLV_COLOR_BLACK);
    MLV_draw_filled_rectangle(0, 0, MENU_WIDTH, MENU_HEIGHT, set_player_color(numpage, 50));
    MLV_get_pixel(MENU_WIDTH/2, MENU_HEIGHT/2, &r, &g, &b, NULL);

    /* bouton quitter */
    MLV_draw_filled_rectangle(MENU_WIDTH*5/6, MENU_HEIGHT - MENU_WIDTH/6, MENU_WIDTH/6, MENU_WIDTH/6, MLV_rgba(255, 255 - 150*(hover == Quit), 255 - 125*(hover == Quit), 100 + 50*(hover == Quit)));
    MLV_draw_image(quit, MENU_WIDTH*5/6 + 10, MENU_HEIGHT - MENU_WIDTH/6 + 10);
    /* flèches de navigation */
    MLV_draw_filled_rectangle(0,              0, 50, 50, MLV_rgba(255, 255, 255, 80 + 100*(hover == Left)));
    MLV_draw_filled_rectangle(MENU_WIDTH -50, 0, 50, 50, MLV_rgba(255, 255, 255, 80 + 100*(hover == Rght)));
    draw_arrow(10,             10, 30, 30, MLV_COLOR_WHITE, 2);
    draw_arrow(MENU_WIDTH -40, 10, 30, 30, MLV_COLOR_WHITE, 3);
    draw_centered_textbox(0, MENU_WIDTH*2/3, 50, font, nom, set_player_color(numpage, 255));

    /* décor du dernier monde + last checkpoint */
    if (!currentworld) {
      MLV_draw_rectangle(25, 75, MENU_HEIGHT/4 +15, MENU_HEIGHT/2, MLV_COLOR_WHITE);
      MLV_draw_filled_rectangle(25, 75, MENU_HEIGHT/4 +15, MENU_HEIGHT/2, MLV_rgba(255, 255, 255, 50));
      MLV_draw_text_with_font(30, MENU_HEIGHT/4, "(NO CHECKPOINT)", font, MLV_COLOR_WHITE);
    } else {
      MLV_draw_image(bg, 25, 75);
      MLV_draw_filled_rectangle(MENU_HEIGHT/4 +15, MENU_HEIGHT/2, 80, 5, MLV_COLOR_WHITE);
    }
    MLV_draw_text_with_font(MENU_WIDTH*2/3, MENU_HEIGHT/2 - 10, "SCORE:", font, MLV_COLOR_WHITE);
    MLV_draw_text_with_font(MENU_WIDTH*2/3, MENU_HEIGHT/2 + 10, "%d pts", font, set_player_color(numpage, 255), score);
    MLV_draw_image(ship, 5 + MENU_HEIGHT/8, MENU_HEIGHT/2);

    /* bonus stocké + nb de tirs */
    MLV_draw_filled_rectangle(25, MENU_HEIGHT*2/3, 100, 100, MLV_rgba(255, 255, 255, 150));
    if (bon == 0) {
        MLV_draw_text_with_font(40, MENU_HEIGHT*2/3 + 40, "(EMPTY)", font, MLV_COLOR_WHITE);
    } else {
        if ((bonus = MLV_load_image(path_bonus)) == NULL) return; else MLV_resize_image(bonus, 80, 80);
        MLV_draw_image(bonus, 25 + 10, MENU_HEIGHT*2/3 + 10);
        MLV_free_image(bonus);
    }
    for (i = 0; i < 4; i++) {
        MLV_draw_filled_circle(150, MENU_HEIGHT*2/3 + (175 - 50*i)/2, 10, (i+1) <= player.perma_bonus ? MLV_rgba(255, 255, 255, 255) : MLV_rgba(255, 255, 255, 100));
    }
    /* argent + nb de missiles */
    MLV_draw_text_with_font(MENU_WIDTH -100, MENU_HEIGHT*2/3, "%5d", font, MLV_COLOR_WHITE, player.money);
    MLV_draw_filled_circle(MENU_WIDTH -25, MENU_HEIGHT*2/3 + 10, 5, MLV_rgba(235, 215, 100, 250));
    for (i = 0; i < MAX_MISSILES; i++) {
        MLV_draw_image(missile, MENU_WIDTH -100 + i*30, MENU_HEIGHT*2/3 +25);
        MLV_draw_filled_rectangle(MENU_WIDTH -100 + i*30, MENU_HEIGHT*2/3 +25, 25, 50, MLV_rgba(r, g, b, 150*(MAX_MISSILES- i > player.nb_missiles)));
    }
    /* c'est tipar */
    draw_centered_textbox(MENU_HEIGHT -75, MENU_WIDTH/2, 50, font, "CHARGER", MLV_rgba(200, 255, 100, 200 + 50*(hover == Chrg)));
    MLV_draw_filled_rectangle(MENU_WIDTH/4, MENU_HEIGHT -70, MENU_WIDTH/2, 20, MLV_rgba(255, 255, 255, 50));

    /* curseur */
    MLV_draw_text_with_font(10, MENU_HEIGHT -50, "%2d", font, set_player_color(numpage, 255), numpage);
    MLV_draw_text_with_font(40, MENU_HEIGHT -50, "/%d", font, MLV_COLOR_WHITE, pages);
    
    MLV_free_image(missile);
    MLV_free_image(ship);
    MLV_free_image(quit);
    MLV_free_font(font);
}

void draw_menu_mnde(int lvl, int last, int frame, Selection hover, MLV_Image *bg) {
    int bg_height = MLV_get_image_height(bg);
    char lvl_monde[STRING_LEN___], path_ship[STRING_LEN___];
    MLV_Image *quit, *ship;
    MLV_Font *font;

    assert(0 <= lvl && lvl <= 4);
    assert(0 <= last && last <= 4);
    sprintf(lvl_monde, "MONDE %d", lvl+1);
    sprintf(path_ship, "data/img/player/spaceship/frame_%d.png", (frame/8)%4);

    if ((ship = MLV_load_image(path_ship)) == NULL) return; else MLV_resize_image(ship, 50, 50);
    if ((quit = MLV_load_image("data/img/icons/quit.png")) == NULL) return; else MLV_resize_image(quit, MENU_WIDTH/6 - 20, MENU_WIDTH/6 - 20);
    if ((font = MLV_load_font("data/fonts/cyberlink.ttf", 20)) == NULL) return;

    /* background */
    if (lvl > last) {
        MLV_draw_image(bg, 0, 0);
        MLV_draw_filled_rectangle(0, 0, MENU_WIDTH, MENU_HEIGHT, MLV_rgba(30, 30, 30, 200));
        draw_centered_text(MENU_WIDTH/2, MENU_HEIGHT/2 - 80, "Pas encore débloqué!", font, MLV_COLOR_WHITE);
    } else {
        MLV_draw_image(bg, 0, (frame/4) % bg_height);
        MLV_draw_image(bg, 0, (frame/4) % bg_height - bg_height);
        MLV_draw_filled_rectangle(0, 0, MENU_WIDTH, MENU_HEIGHT, MLV_rgba(30, 30, 30, 30));
    }
    
    /* boutons de navigation */
    MLV_draw_filled_rectangle(0,               100, 50, 75, MLV_rgba(255, 255, 255, 80 + 50*(hover == Left)));
    MLV_draw_filled_rectangle(MENU_WIDTH - 50, 100, 50, 75, MLV_rgba(255, 255, 255, 80 + 50*(hover == Rght)));
    draw_arrow(10,             120, 30, 30, MLV_COLOR_WHITE, 2);
    draw_arrow(MENU_WIDTH -40, 120, 30, 30, MLV_COLOR_WHITE, 3);
    MLV_draw_filled_rectangle(60, 100, MENU_WIDTH - 120, 75, MLV_rgba(0, 0, 0, 100));
    draw_centered_text(MENU_WIDTH/2, 130, lvl_monde, font, MLV_COLOR_WHITE);

    /* vaisseau */
    MLV_draw_image(ship, MENU_WIDTH/2 - 25, MENU_HEIGHT/2);
    
    /* bouton quitter */
    MLV_draw_filled_rectangle(MENU_WIDTH*5/6, MENU_HEIGHT - MENU_WIDTH/6, MENU_WIDTH/6, MENU_WIDTH/6, MLV_rgba(255, 255 - 150*(hover == Quit), 255 - 125*(hover == Quit), 100 + 50*(hover == Quit)));
    MLV_draw_image(quit, MENU_WIDTH*5/6 + 10, MENU_HEIGHT - MENU_WIDTH/6 + 10);

    draw_centered_textbox(MENU_HEIGHT -75, MENU_WIDTH/2, 50, font, "C'EST PARTI !", MLV_rgba(200, 255, 100 + 125*(lvl > last), 200 + 50*(hover == Chrg)));
    MLV_draw_filled_rectangle(MENU_WIDTH/4, MENU_HEIGHT -70, MENU_WIDTH/2, 20, MLV_rgba(255, 255, 255, 50));

    MLV_actualise_window();
    MLV_free_image(ship);
    MLV_free_image(quit);
    MLV_free_font(font);
}

void draw_menu_ldbd(int lvl, int frame, Selection hover, MLV_Image *bg, MLV_Button_state state) {
    char file_name[STRING_LEN___],
         lvl_monde[STRING_LEN___],
         pseudo[STRING_LEN___];
    int i= 0, score,
        bg_height = MLV_get_image_height(bg);
    FILE* score_file;
    MLV_Image *quit;
    MLV_Font *font;

    assert(0 <= lvl && lvl <= 4);
    
    sprintf(file_name, "data/scores/monde_%d.dat", lvl);
    sprintf(lvl_monde, "MONDE %d", lvl+1);

    if ((quit = MLV_load_image("data/img/icons/quit.png")) == NULL) return; else MLV_resize_image(quit, MENU_WIDTH/6 - 20, MENU_WIDTH/6 - 20);
    if ((font = MLV_load_font("data/fonts/retro_computer.ttf", 20)) == NULL) return;

    /* background */
    MLV_draw_image(bg, 0, (frame/4) % bg_height);
    MLV_draw_image(bg, 0, (frame/4) % bg_height - bg_height);
    MLV_draw_filled_rectangle(0, 0, MENU_WIDTH, MENU_HEIGHT, MLV_rgba(30, 30, 30, 70));
    
    /* boutons de navigation */
    MLV_draw_filled_rectangle(0, 0,                MENU_WIDTH/15, MENU_HEIGHT/20 - 5, MLV_rgba(255, 255, 255, 80 + 50*(hover == Left)));
    MLV_draw_filled_rectangle(MENU_WIDTH*14/15, 0, MENU_WIDTH/15, MENU_HEIGHT/20 - 5, MLV_rgba(255, 255, 255, 80 + 50*(hover == Rght)));
    draw_arrow(0, 0,                MENU_WIDTH/15, MENU_HEIGHT/20 - 5, MLV_COLOR_WHITE, 2);
    draw_arrow(MENU_WIDTH*14/15, 0, MENU_WIDTH/15, MENU_HEIGHT/20 - 5, MLV_COLOR_WHITE, 3);
    MLV_draw_filled_rectangle(MENU_WIDTH/15 +5, 0, MENU_WIDTH*13/15 - 10, MENU_HEIGHT/20 - 5, MLV_rgba(0, 0, 0, 80));
    draw_centered_text(MENU_WIDTH/2, 10, lvl_monde, font, MLV_COLOR_WHITE);
    
    /* bouton quitter */
    MLV_draw_filled_rectangle(MENU_WIDTH*5/6, MENU_HEIGHT - MENU_WIDTH/6, MENU_WIDTH/6, MENU_WIDTH/6, MLV_rgba(255, 255 - 150*(hover == Quit), 255 - 125*(hover == Quit), 100 + 50*(hover == Quit) + 50*(state == MLV_PRESSED && hover == Quit)));
    MLV_draw_image(quit, MENU_WIDTH*5/6 + 10, MENU_HEIGHT - MENU_WIDTH/6 + 10);

    if ((score_file = fopen(file_name, "r")) == NULL) {
        draw_centered_text(MENU_WIDTH/2, MENU_HEIGHT/2 - 80, "Pas encore de scores!", font, MLV_COLOR_WHITE);
    } else {
        /* tableau des scores */
        while (fscanf(score_file, "%s %d\n", pseudo, &score) != EOF && (i++ < 21)) {
            MLV_draw_filled_rectangle(0,               i*MENU_HEIGHT/21, MENU_WIDTH/10 - 5,   MENU_HEIGHT/21 - 5, rank(i, 250));
            MLV_draw_filled_rectangle(MENU_WIDTH/10,   i*MENU_HEIGHT/21, MENU_WIDTH*6/10 - 5, MENU_HEIGHT/21 - 5, rank(i, 100));
            MLV_draw_filled_rectangle(MENU_WIDTH*7/10, i*MENU_HEIGHT/21, MENU_WIDTH*3/10,     MENU_HEIGHT/21 - 5, MLV_rgba(255, 255, 255, 150 - 7*i));

            MLV_draw_text_with_font(5,                   i*MENU_HEIGHT/21, "%d", font, MLV_COLOR_WHITE, i);
            MLV_draw_text_with_font(MENU_WIDTH/10 + 20,   i*MENU_HEIGHT/21, "%s", font, MLV_COLOR_WHITE, pseudo);
            MLV_draw_text_with_font(MENU_WIDTH*7/10 + 10, i*MENU_HEIGHT/21, "%d", font, MLV_COLOR_WHITE, score);

            MLV_draw_filled_rectangle(0,               i*MENU_HEIGHT/21 + 5, MENU_WIDTH/10 - 5, 10, MLV_rgba(255, 255, 255, 80));
            MLV_draw_filled_rectangle(MENU_WIDTH*7/10, i*MENU_HEIGHT/21 + 5, MENU_WIDTH*3/10,   10, MLV_rgba(255, 255, 255, 80));
        }
        fclose(score_file);
    }
    MLV_actualise_window();
    MLV_free_image(quit);
    MLV_free_font(font);
}

void draw_menu_optn(Parametre* para, Slider sliders[], int ingame, Selection hover, Selection select, int frame) {
    char keys[16][STRING_LEN___];
    MLV_Font *font, *title;
    MLV_Image *quit;
    
    if ((quit = MLV_load_image("data/img/icons/quit.png")) == NULL) return; else MLV_resize_image(quit, MENU_WIDTH/6 - 20, MENU_WIDTH/6 - 20);
    if ((font = MLV_load_font("data/fonts/retro_computer.ttf", 15)) == NULL) return;
    if ((title = MLV_load_font("data/fonts/cyberlink.ttf", 20)) == NULL) return;

    MLV_clear_window(MLV_COLOR_BLACK);
    load_unicodes(keys, para);

    /* boutons de navigation */
    draw_centered_text(MENU_WIDTH/2, 22, "- OPTIONS -", title, MLV_rgba(250, 50, 150, 255));
    draw_centered_text(MENU_WIDTH/2, 20, "- OPTIONS -", title, MLV_COLOR_WHITE);

    /* commandes */
    MLV_draw_text_with_font(50, 50, "COMMANDES", title, MLV_rgba(200, 225, 50, 255));
    MLV_draw_text_with_font(20 + MENU_WIDTH/2,   50, "PRIM.", title, MLV_rgba(200, 225, 50, 255));
    MLV_draw_text_with_font(25 + MENU_WIDTH*3/4, 50, "2nd",   title, MLV_rgba(200, 225, 50, 255));

    MLV_draw_text_with_font(25, 75,  "Gauche",      font, MLV_COLOR_WHITE);
    MLV_draw_text_with_font(25, 95,  "Droite",      font, MLV_COLOR_WHITE);
    MLV_draw_text_with_font(25, 115, "Haut",        font, MLV_COLOR_WHITE);
    MLV_draw_text_with_font(25, 135, "Bas",         font, MLV_COLOR_WHITE);
    MLV_draw_text_with_font(25, 155, "Blaster",     font, MLV_COLOR_WHITE);
    MLV_draw_text_with_font(25, 175, "Blst. chrg.", font, MLV_COLOR_WHITE);
    MLV_draw_text_with_font(25, 195, "Missile",     font, MLV_COLOR_WHITE);
    MLV_draw_text_with_font(25, 215, "Bonus",       font, MLV_COLOR_WHITE);

    MLV_draw_text_box_with_font(MENU_WIDTH/2,   75,  MENU_WIDTH/4 - 20, 20, "%s", font, 0, MLV_rgba(255, 255, 255, 255*(hover == Left)),  MLV_rgba(150, 175, 0, 255), MLV_rgba(255, 255, 255, 90 + (select == Left)*(frame %90)),  MLV_HORIZONTAL_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER, select == Left ? "???" : keys[Left_key]);
    MLV_draw_text_box_with_font(MENU_WIDTH/2,   95,  MENU_WIDTH/4 - 20, 20, "%s", font, 0, MLV_rgba(255, 255, 255, 255*(hover == Rght)),  MLV_rgba(150, 175, 0, 255), MLV_rgba(255, 255, 255, 70 + (select == Rght)*(frame %90)),  MLV_HORIZONTAL_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER, select == Rght ? "???" : keys[Right_key]);
    MLV_draw_text_box_with_font(MENU_WIDTH/2,   115, MENU_WIDTH/4 - 20, 20, "%s", font, 0, MLV_rgba(255, 255, 255, 255*(hover == Up)),    MLV_rgba(150, 175, 0, 255), MLV_rgba(255, 255, 255, 90 + (select == Up  )*(frame %90)),  MLV_HORIZONTAL_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER, select == Up   ? "???" : keys[Up_key]);
    MLV_draw_text_box_with_font(MENU_WIDTH/2,   135, MENU_WIDTH/4 - 20, 20, "%s", font, 0, MLV_rgba(255, 255, 255, 255*(hover == Down)),  MLV_rgba(150, 175, 0, 255), MLV_rgba(255, 255, 255, 70 + (select == Down)*(frame %90)),  MLV_HORIZONTAL_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER, select == Down ? "???" : keys[Down_key]);
    MLV_draw_text_box_with_font(MENU_WIDTH/2,   155, MENU_WIDTH/4 - 20, 20, "%s", font, 0, MLV_rgba(255, 255, 255, 255*(hover == Blst)),  MLV_rgba(150, 175, 0, 255), MLV_rgba(255, 255, 255, 90 + (select == Blst)*(frame %90)),  MLV_HORIZONTAL_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER, select == Blst ? "???" : keys[Blaster_key]);
    MLV_draw_text_box_with_font(MENU_WIDTH/2,   175, MENU_WIDTH/4 - 20, 20, "%s", font, 0, MLV_rgba(255, 255, 255, 255*(hover == Chrg)),  MLV_rgba(150, 175, 0, 255), MLV_rgba(255, 255, 255, 70 + (select == Chrg)*(frame %90)),  MLV_HORIZONTAL_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER, select == Chrg ? "???" : keys[Charged_key]);
    MLV_draw_text_box_with_font(MENU_WIDTH/2,   195, MENU_WIDTH/4 - 20, 20, "%s", font, 0, MLV_rgba(255, 255, 255, 255*(hover == Miss)),  MLV_rgba(150, 175, 0, 255), MLV_rgba(255, 255, 255, 90 + (select == Miss)*(frame %90)),  MLV_HORIZONTAL_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER, select == Miss ? "???" : keys[Missile_key]);
    MLV_draw_text_box_with_font(MENU_WIDTH/2,   215, MENU_WIDTH/4 - 20, 20, "%s", font, 0, MLV_rgba(255, 255, 255, 255*(hover == Bons)),  MLV_rgba(150, 175, 0, 255), MLV_rgba(255, 255, 255, 70 + (select == Bons)*(frame %90)),  MLV_HORIZONTAL_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER, select == Bons ? "???" : keys[Bonus_key]);

    MLV_draw_text_box_with_font(MENU_WIDTH*3/4, 75,  MENU_WIDTH/4 - 20, 20, "%s", font, 0, MLV_rgba(255, 255, 255, 255*(hover == Left2)), MLV_rgba(150, 175, 0, 255), MLV_rgba(255, 255, 255, 90 + (select == Left2)*(frame %90)), MLV_HORIZONTAL_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER, select == Left2 ? "???" : keys[Left_key +8]);
    MLV_draw_text_box_with_font(MENU_WIDTH*3/4, 95,  MENU_WIDTH/4 - 20, 20, "%s", font, 0, MLV_rgba(255, 255, 255, 255*(hover == Rght2)), MLV_rgba(150, 175, 0, 255), MLV_rgba(255, 255, 255, 70 + (select == Rght2)*(frame %90)), MLV_HORIZONTAL_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER, select == Rght2 ? "???" : keys[Right_key +8]);
    MLV_draw_text_box_with_font(MENU_WIDTH*3/4, 115, MENU_WIDTH/4 - 20, 20, "%s", font, 0, MLV_rgba(255, 255, 255, 255*(hover == Up2)),   MLV_rgba(150, 175, 0, 255), MLV_rgba(255, 255, 255, 90 + (select == Up2  )*(frame %90)), MLV_HORIZONTAL_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER, select == Up2   ? "???" : keys[Up_key +8]);
    MLV_draw_text_box_with_font(MENU_WIDTH*3/4, 135, MENU_WIDTH/4 - 20, 20, "%s", font, 0, MLV_rgba(255, 255, 255, 255*(hover == Down2)), MLV_rgba(150, 175, 0, 255), MLV_rgba(255, 255, 255, 70 + (select == Down2)*(frame %90)), MLV_HORIZONTAL_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER, select == Down2 ? "???" : keys[Down_key +8]);
    MLV_draw_text_box_with_font(MENU_WIDTH*3/4, 155, MENU_WIDTH/4 - 20, 20, "%s", font, 0, MLV_rgba(255, 255, 255, 255*(hover == Blst2)), MLV_rgba(150, 175, 0, 255), MLV_rgba(255, 255, 255, 90 + (select == Blst2)*(frame %90)), MLV_HORIZONTAL_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER, select == Blst2 ? "???" : keys[Blaster_key +8]);
    MLV_draw_text_box_with_font(MENU_WIDTH*3/4, 175, MENU_WIDTH/4 - 20, 20, "%s", font, 0, MLV_rgba(255, 255, 255, 255*(hover == Chrg2)), MLV_rgba(150, 175, 0, 255), MLV_rgba(255, 255, 255, 70 + (select == Chrg2)*(frame %90)), MLV_HORIZONTAL_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER, select == Chrg2 ? "???" : keys[Charged_key +8]);
    MLV_draw_text_box_with_font(MENU_WIDTH*3/4, 195, MENU_WIDTH/4 - 20, 20, "%s", font, 0, MLV_rgba(255, 255, 255, 255*(hover == Miss2)), MLV_rgba(150, 175, 0, 255), MLV_rgba(255, 255, 255, 90 + (select == Miss2)*(frame %90)), MLV_HORIZONTAL_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER, select == Miss2 ? "???" : keys[Missile_key +8]);
    MLV_draw_text_box_with_font(MENU_WIDTH*3/4, 215, MENU_WIDTH/4 - 20, 20, "%s", font, 0, MLV_rgba(255, 255, 255, 255*(hover == Bons2)), MLV_rgba(150, 175, 0, 255), MLV_rgba(255, 255, 255, 70 + (select == Bons2)*(frame %90)), MLV_HORIZONTAL_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER, select == Bons2 ? "???" : keys[Bonus_key +8]);

    /* résolution */
    MLV_draw_text_with_font(50, 265, "RESOLUTION", title, MLV_rgba(250 - 100*(ingame), 50 + 100*(ingame), 150, 255));
    MLV_draw_rectangle(25,  295, 100, 100, MLV_COLOR_WHITE);
    MLV_draw_rectangle(150, 295, 100, 100, MLV_COLOR_WHITE);
    MLV_draw_rectangle(275, 295, 100, 100, MLV_COLOR_WHITE);
    MLV_draw_filled_rectangle(25,  295, 100, 100, MLV_rgba(250 - 100*(ingame), 50 + 100*(ingame), 150, 100*(hover == R600 && para->width != 600) + 200*(para->width == 600)));
    MLV_draw_filled_rectangle(150, 295, 100, 100, MLV_rgba(250 - 100*(ingame), 50 + 100*(ingame), 150, 100*(hover == R700 && para->width != 700) + 200*(para->width == 700)));
    MLV_draw_filled_rectangle(275, 295, 100, 100, MLV_rgba(250 - 100*(ingame), 50 + 100*(ingame), 150, 100*(hover == R800 && para->width != 800) + 200*(para->width == 800)));
    MLV_draw_text_with_font(55,  315, "600", font, MLV_COLOR_WHITE);
    MLV_draw_text_with_font(180, 315, "700", font, MLV_COLOR_WHITE);
    MLV_draw_text_with_font(305, 315, "800", font, MLV_COLOR_WHITE);
    MLV_draw_text_with_font(70,  335,  "x",  font, MLV_COLOR_WHITE);
    MLV_draw_text_with_font(195, 335,  "x",  font, MLV_COLOR_WHITE);
    MLV_draw_text_with_font(320, 335,  "x",  font, MLV_COLOR_WHITE);
    MLV_draw_text_with_font(55,  355, "800", font, MLV_COLOR_WHITE);
    MLV_draw_text_with_font(180, 355, "900", font, MLV_COLOR_WHITE);
    MLV_draw_text_with_font(305, 355, "950", font, MLV_COLOR_WHITE);


    /* volume */
    MLV_draw_text_with_font(25, MENU_HEIGHT*2/3 + 40, "Musique", font, MLV_rgba(150, 100, 200, 255));
    MLV_draw_text_with_font(65, MENU_HEIGHT*2/3 + 90, "Sons",    font, MLV_rgba(150, 100, 200, 255));

    MLV_draw_filled_rectangle(125, MENU_HEIGHT*2/3 + 30, MENU_WIDTH - 150, 40, MLV_rgba(180, 200 - (hover == Musc)*50, 200, 255));
    MLV_draw_filled_rectangle(125, MENU_HEIGHT*2/3 + 80, MENU_WIDTH - 150, 40, MLV_rgba(180, 200 - (hover == Sons)*50, 200, 255));
    MLV_draw_filled_rectangle(125, MENU_HEIGHT*2/3 + 35, MENU_WIDTH - 150, 15, MLV_rgba(255, 255, 255, 150));
    MLV_draw_filled_rectangle(125, MENU_HEIGHT*2/3 + 85, MENU_WIDTH - 150, 15, MLV_rgba(255, 255, 255, 150));
    affiche_slider(&sliders[0], 0);
    affiche_slider(&sliders[1], 0);

    /* limites */
    MLV_draw_filled_rectangle(0, 40,                   MENU_WIDTH, 210,      MLV_rgba(200, 225, 50,  40));
    MLV_draw_filled_rectangle(0, 250,                  MENU_WIDTH, 160,      MLV_rgba(250, 50,  150, 40));
    MLV_draw_filled_rectangle(0, MENU_HEIGHT*2/3 + 10, MENU_WIDTH, MENU_HEIGHT/3, MLV_rgba(150, 100, 200, 40));
    MLV_draw_filled_rectangle(0, 40,                   MENU_WIDTH, 5, MLV_COLOR_GREY80);
    MLV_draw_filled_rectangle(0, 250,                  MENU_WIDTH, 5, MLV_COLOR_GREY80);
    MLV_draw_filled_rectangle(0, MENU_HEIGHT*2/3 + 10, MENU_WIDTH, 5, MLV_COLOR_GREY80);
    
    /* bouton quitter */
    MLV_draw_filled_rectangle(MENU_WIDTH*5/6, MENU_HEIGHT - MENU_WIDTH/6, MENU_WIDTH/6, MENU_WIDTH/6, MLV_rgba(255, 255 - 150*(hover == Quit), 255 - 125*(hover == Quit), 100 + 50*(hover == Quit)));
    MLV_draw_image(quit, MENU_WIDTH*5/6 + 10, MENU_HEIGHT - MENU_WIDTH/6 + 10);

    MLV_actualise_window();
    MLV_free_image(quit);
    MLV_free_font(title);
    MLV_free_font(font);
}

void draw_menu_pause(int width, int height, Selection hover, MLV_Button_state state) {
    MLV_Font *font;

    if ((font = MLV_load_font("data/fonts/cyberlink.ttf", 50)) == NULL) return;

    draw_centered_text(width/2, height/4, "PAUSE", font, MLV_COLOR_WHITE);

    MLV_draw_filled_rectangle(width/4, height*3/8, width/2 - (state == MLV_PRESSED && hover == Strt), height/10, MLV_rgba(220, 180 - 50*(hover == Strt), 250, 110 + 40*(hover == Strt)));
    MLV_draw_filled_rectangle(width/4, height*4/8, width/2 - (state == MLV_PRESSED && hover == Sett), height/10, MLV_rgba(220, 180 - 50*(hover == Sett), 250, 110 + 40*(hover == Sett)));
    MLV_draw_filled_rectangle(width/4, height*5/8, width/2 - (state == MLV_PRESSED && hover == Quit), height/10, MLV_rgba(250, 50  - 50*(hover == Quit), 150, 110  + 40*(hover == Quit)));
    draw_centered_text(width/2, height*3/8 + height/20, "REPRENDRE", font, MLV_COLOR_WHITE);
    draw_centered_text(width/2, height*4/8 + height/20, "OPTIONS",   font, MLV_COLOR_WHITE);
    draw_centered_text(width/2, height*5/8 + height/20, "QUITTER",   font, MLV_COLOR_WHITE);
    
    MLV_actualise_window();
    MLV_free_font(font);
}

void draw_shop_menu(Terrain* terrain, Parametre* para, ImgList* img_list, Power selected, int frame, int *err_msg) {
    static int warning_msg;
    MLV_Font *big, *medium, *small;
    MLV_Image *image;
    int i,j,tmp,current,w;
    int r;
    
    if ((big = MLV_load_font("data/fonts/cyberlink.ttf", 40)) == NULL) return;
    if ((medium = MLV_load_font("data/fonts/cyberlink.ttf", 21)) == NULL) return;
    if ((small = MLV_load_font("data/fonts/cyberlink.ttf", 14)) == NULL) return;
    
    if (*err_msg == -1) {
        warning_msg = 0;
        return;
    }
    if (*err_msg != 0 && warning_msg == 0)
        warning_msg = 100;
    if (warning_msg > 0)
        warning_msg--;
    if (warning_msg == 0) 
        *err_msg = 0;
    
    r = 2 * warning_msg;
    
    draw_window_noupdate(para, terrain, img_list, 0);
    MLV_clear_window(MLV_rgba(r, 0, 0, 127));
    
    if (*err_msg == 1)
        draw_centered_text(para->width/2, para->height / 10 + 30, "Pas de bonus selectionné", medium, MLV_rgba(r+55, 0, 0, 255));
    else if (*err_msg == 2)
        draw_centered_text(para->width/2, para->height / 10 + 30, "Bonus déjà acheté", medium, MLV_rgba(r+55, 0, 0, 255));
    else if (*err_msg == 3)
        draw_centered_text(para->width/2, para->height / 10 + 30, "Maximum de vie atteint", medium, MLV_rgba(r+55, 0, 0, 255));
    else if (*err_msg == 4)
        draw_centered_text(para->width/2, para->height / 10 + 30, "Pas assez d'argent", medium, MLV_rgba(r+55, 0, 0, 255));
    

    /* Titre */
    MLV_draw_filled_rectangle(0, 0, para->width, para->height/10, MLV_rgba(r, 0, 0, 200));
    draw_centered_text(para->width/2, para->height/20, "SHOP", big, MLV_COLOR_WHITE);
    /* Les bonus statique si non selectionné */
    tmp = (para->width - (para->sizes.bonus_width * 5)) / 6;    /* ecart entre les bonus */
    for (i=0; i<2; i++) {
        for (j=0; j<NB_BONUS/2; j++) {
            current = i * NB_BONUS/2 + j;
            image = get_bonus_image(img_list, current, current==selected?frame%BONUS_FRAME:0);
            MLV_draw_image(image, tmp + j*(tmp+para->sizes.bonus_width), para->height/5 + 2 * i * para->sizes.bonus_height);
            MLV_get_size_of_text_with_font("%d", &w, NULL, small, get_bonus_price(current));
            MLV_draw_text_with_font((1 + j)*tmp + (j+.5)*para->sizes.bonus_width - w/2,
                            para->height/5 + (2*i + 1)*para->sizes.bonus_height,
                            "%d", small, MLV_COLOR_WHITE, get_bonus_price(current));
        }
    }
    
    /* Description */
    MLV_draw_text_box_with_font(0, para->height/2, 3*para->width/4, para->height/6,
                    get_bonus_description(selected), medium, 10,
                    MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_rgba(r, 0, 0, 200),
                    MLV_TEXT_LEFT, MLV_HORIZONTAL_LEFT, MLV_VERTICAL_TOP);
    /* Ancien bonus */
    MLV_draw_rectangle(para->width - para->sizes.bonus_width - 10, para->height/2-10 - para->sizes.bonus_height,
            para->sizes.bonus_width + 10, para->sizes.bonus_height + 10, MLV_COLOR_WHITE);
    if (terrain->player->stock_bonus != Aucun) {
        MLV_draw_image(get_bonus_image(img_list, terrain->player->stock_bonus, frame%BONUS_FRAME),
                para->width - para->sizes.bonus_width - 5, para->height/2 - 5 - para->sizes.bonus_height);
    }
    /* Monnaie */
    MLV_draw_text_box_with_font(10, para->height/2-para->height/18, 2*para->width/5, para->height/18,
                    "%d points", medium, 10,
                    MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_rgba(r, 0, 0, 200),
                    MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER, terrain->player->money);
    /* Boutton achat */
    MLV_draw_filled_rectangle(3*para->width/4, 6*para->height/12, para->width/4, para->height/12, MLV_rgba(220, 200, 70, 200));
    MLV_draw_text_box_with_font(3*para->width/4, 6*para->height/12, para->width/4, para->height/12,
                    "Acheter", medium, 10,
                    MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_rgba(r, 0, 0, 150),
                    MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    /* Boutton continuer */
    MLV_draw_filled_rectangle(3*para->width/4, 7*para->height/12, para->width/4, para->height/12, MLV_rgba(135, 220, 70, 200));
    MLV_draw_text_box_with_font(3*para->width/4, 7*para->height/12, para->width/4, para->height/12,
                    "Continuer", medium, 10,
                    MLV_COLOR_WHITE, MLV_COLOR_WHITE, MLV_rgba(r, 0, 0, 150),
                    MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    
    MLV_free_font(big);
    MLV_free_font(medium);
    MLV_free_font(small);
}

/* MENUS */
void menu_principal(Data* data) {
    static int save_found = 1;
    MLV_Button_state state = MLV_RELEASED;
    MLV_Keyboard_button key;
    Selection select = None, holded = None, hover;
    MLV_Mouse_button button;
    Star* tab[50];
    int x, y, frame, pressed;
    int key_hist[MLV_KEYBOARD_LAST];
    MLV_Event event; 

    frame = pressed = 0;
    MLV_get_mouse_position(&x, &y);
    init_tab(key_hist, MLV_KEYBOARD_LAST, 0);
    fill_star_tab(tab, MENU_WIDTH, MENU_HEIGHT, 50);
    
    /* sélection */ 
    do {
        event = MLV_get_event(&key, NULL, NULL, NULL, NULL, &x, &y, &button, &state);
        if (frame%2) {
            update_stars(tab, MENU_WIDTH, MENU_HEIGHT, 50);
        }
        hover_menu_prcp(&hover, x, y);

        MLV_clear_window(MLV_rgba(25, 0, 40, 255));
        draw_stars(tab, 50);
        draw_menu_prcp(hover, frame, !pressed, save_found);
        
        select_holded_button(event, button, state, hover, &pressed, &holded, &select);
        
        
        if (event == MLV_MOUSE_BUTTON && button == MLV_BUTTON_LEFT) {
            if (state == MLV_PRESSED) {
                holded = hover;
                pressed = 1;
            } else {
                pressed = 0;
                select = hover;
            }
        } else if (event == MLV_KEY) { /* Bonus */
            key_hist[key] = state==MLV_PRESSED;
            if (data->lvl_bonus == 0 && key_hist[MLV_KEYBOARD_b] && key_hist[MLV_KEYBOARD_a]) {
                data->lvl_bonus = 1;
                if (img_chargement_bonus(&(data->img_list), &(data->para)) == 0) {
                    fprintf(stderr, "Erreur: Image manquante. Stopping the program\n");
                }
                if (init_bonus_sound(&(data->sound_list)) == 0) {
                    fprintf(stderr, "Erreur: Son manquant. Stopping the program\n");
                }
            }
        }
        frame++;
    } while (holded == None || holded != select || pressed);

    /* redirection */
    save_found = 1;
    switch(select) {
        case Nouv:
            if (menu_nouv_partie(data)) {
                save_found = menu_charger(data);
            }
            menu_principal(data);
            break;
        case Chrg:
            save_found = menu_charger(data);
            menu_principal(data);
            break;
        case Lead:
            menu_leaderboard(); 
            menu_principal(data);
            break;
        case Sett:
            if (!menu_options(&(data->para), 0)) {
                free_image(&(data->img_list));
                img_chargement(&(data->img_list), &(data->para));
            }
            menu_principal(data);
            break;
        case Quit:
            break;
        default:
            menu_principal(data);
    }
}

/* Lancement d'une nouvelle partie */
/* Renvoie 0 si aucune save n'est créée 1 sinon */
int menu_nouv_partie(Data* data) {
    int currentworld, fpos, score, taken = 0;
    Player player;
    
    do {
        if (!new_player(data->fname, taken)) {        
            return 0;
        }
        taken = load_partie(&player, data->fname, &(data->last_world), &currentworld, &fpos, &score);
    } while (taken);
    
    tutoriel(data);
    
    init_player(&player, 0, 0, data->para.sizes.player_width, data->para.sizes.player_height);
    printf("Creation d'une nouvelle partie de nom %s\n", data->fname);
    save_partie(&player, data->fname, 0, 0, 0, 0);
    return 1;
}

/* Menu de choix de la partie à lancer */
/* Renvoie 0 si il n'y a pas de save
 *         1 si tout vas bien */
int menu_charger(Data* data) {
    int x, y, currentworld, frame = 0,
        nb_parties, fpos, score = 0, curseur = 1, pressed;
    char bg_path[STRING_LEN___];
    FILE* fichier;
    Player player;
    Selection hover, holded, select;
    Liste lst = NULL;
    MLV_Image *bg;
    MLV_Event event;
    MLV_Button_state state = MLV_RELEASED;
    MLV_Keyboard_button key = MLV_KEYBOARD_NONE;
    MLV_Mouse_button button;
    MLV_get_mouse_position(&x, &y);
  
    if (data->fname[0] == '\0') {
        /* on vide le fichier */
        fichier= fopen(OUT, "w");
        fclose(fichier);
        /* on remplit le fichier */
        if (ftw("data/save/", write_out, 1) == -1) {
            fprintf(stderr, "\n   ERREUR: Le dossier './data/save' n'existe pas.\n\n");
            return 0;
        }
        /* on lit le fichier */
        nb_parties = out_to_list(&lst);
        if (lst == NULL) {
            return 0;
        }
        load_partie(&player, lst->nom, &(data->last_world), &currentworld, &fpos, &score);
        sprintf(bg_path, "data/img/background/background_%d.png", currentworld-(currentworld!=0));
        bg = MLV_load_image(bg_path);
        MLV_resize_image(bg, MENU_HEIGHT/4 +15, MENU_HEIGHT/2);

        do {
            frame++;
            
            event = MLV_get_event(&key, NULL, NULL, NULL, NULL, &x, &y, &button, &state);
            draw_menu_chrg(player, lst->nom, currentworld, score, nb_parties, curseur, frame, hover, bg);
            hover_menu_chrg(&hover, x, y);
            MLV_actualise_window();

            select_holded_button(event, button, state, hover, &pressed, &holded, &select);
            if (event == MLV_KEY && state == MLV_PRESSED) {
                if (key == MLV_KEYBOARD_LEFT) {
                    lst = lst->prev;
                    curseur = curseur == 1 ? nb_parties : curseur-1;
                    load_partie(&player, lst->nom, &(data->last_world), &currentworld, &fpos, &score);
                    sprintf(bg_path, "data/img/background/background_%d.png", currentworld-(currentworld!=0));
                    MLV_free_image(bg);
                    bg = MLV_load_image(bg_path);
                    MLV_resize_image(bg, MENU_HEIGHT/4 +15, MENU_HEIGHT/2);
                }
                if (key == MLV_KEYBOARD_RIGHT) {
                    lst = lst->next;
                    curseur = curseur == nb_parties ? 1 : curseur+1;
                    load_partie(&player, lst->nom, &(data->last_world), &currentworld, &fpos, &score);
                    sprintf(bg_path, "data/img/background/background_%d.png", currentworld-(currentworld!=0));
                    MLV_free_image(bg);
                    bg = MLV_load_image(bg_path);
                    MLV_resize_image(bg, MENU_HEIGHT/4 +15, MENU_HEIGHT/2);
                }
                if (key == MLV_KEYBOARD_ESCAPE) {
                    return 1;
                }
            }
            if (!pressed) {
                if (holded == Left && select == Left) {
                    lst = lst->prev;
                    curseur = curseur == 1 ? nb_parties : curseur-1;
                    load_partie(&player, lst->nom, &(data->last_world), &currentworld, &fpos, &score);
                    sprintf(bg_path, "data/img/background/background_%d.png", currentworld-(currentworld!=0));
                    MLV_free_image(bg);
                    bg = MLV_load_image(bg_path);
                    MLV_resize_image(bg, MENU_HEIGHT/4 +15, MENU_HEIGHT/2);
                    select = None;
                }
                if (holded == Rght && select == Rght) {
                    lst = lst->next;
                    curseur = curseur == nb_parties ? 1 : curseur+1;
                    load_partie(&player, lst->nom, &(data->last_world), &currentworld, &fpos, &score);
                    sprintf(bg_path, "data/img/background/background_%d.png", currentworld-(currentworld!=0));
                    MLV_free_image(bg);
                    bg = MLV_load_image(bg_path);
                    MLV_resize_image(bg, MENU_HEIGHT/4 +15, MENU_HEIGHT/2);
                    select = None;
                }
                if (holded == Quit && select == Quit) {
                    return 1;
                }
                if (hover == Chrg && select == Chrg) {
                    break;
                }
            }
        } while (event != MLV_KEY || key != MLV_KEYBOARD_RETURN);
        MLV_free_image(bg);
    } else {
        lst = alloueCellule(data->fname);
    }
    strcpy(data->fname, lst->nom);
    
    do {
        init_player(&player, 0, 0, data->para.sizes.player_width, data->para.sizes.player_height);
        load_partie(&player, lst->nom, &(data->last_world), &currentworld, &fpos, &score);
    } while ( menu_monde(data, &player, currentworld, fpos, score) );
    data->fname[0] = '\0';
    return 1;
}

/* Menu du choix du monde à lancer
 * 1 si on veut retourner dans le menu monde 0 sinon */
int menu_monde(Data* data, Player* player, int currentworld, int fpos, int score) {
    int x, y, frame= 0, ret, pressed, lvl = currentworld-(currentworld!=0);
    char bg_path[STRING_LEN___];
    MLV_Keyboard_button key;
    MLV_Mouse_button button;
    MLV_Button_state state = MLV_RELEASED;
    MLV_Event event;
    Selection hover, select, holded;
    MLV_Image *bg;
    MLV_get_mouse_position(&x, &y);
    holded = select = None;
    sprintf(bg_path, "data/img/background/background_%d.png", lvl);
    bg = MLV_load_image(bg_path);
    MLV_resize_image_with_proportions(bg, MENU_WIDTH, -1);

    if (currentworld != 0) {
        /* On lance le monde currentworld-1 */
        currentworld--;
    } else {
        /* on propose de choisir le monde dans lequel jouer */
        do {
            event = MLV_get_event(&key, NULL, NULL, NULL, NULL, &x, &y, &button, &state);
            hover_menu_mnde(&hover, x, y);
            draw_menu_mnde(lvl, data->last_world, frame, hover, bg);
            select_holded_button(event, button, state, hover, &pressed, &holded, &select);

            if ((event == MLV_KEY && key == MLV_KEYBOARD_LEFT && state == MLV_PRESSED) || (holded == Left && select == Left && !pressed)) {
                lvl = lvl == 0 ? 4 : lvl-1;
                MLV_free_image(bg);
                sprintf(bg_path, "data/img/background/background_%d.png", lvl);
                bg = MLV_load_image(bg_path);
                MLV_resize_image_with_proportions(bg, MENU_WIDTH, -1);
                select = None;
            }
            if ((event == MLV_KEY && key == MLV_KEYBOARD_RIGHT && state == MLV_PRESSED) || (holded == Rght && select == Rght && !pressed)) {
                lvl = lvl == 4 ? 0 : lvl+1;
                MLV_free_image(bg);
                sprintf(bg_path, "data/img/background/background_%d.png", lvl);
                bg = MLV_load_image(bg_path);
                MLV_resize_image_with_proportions(bg, MENU_WIDTH, -1);
                select = None;
            }
            if ((event == MLV_KEY && key == MLV_KEYBOARD_ESCAPE && state == MLV_PRESSED) || (holded == Quit && select == Quit && !pressed)) {
                return 0;
            }
            if (holded == Chrg && select == Chrg && !pressed && lvl <= data->last_world) {
                break;
            }
            frame++;
        } while (event != MLV_KEY || key != MLV_KEYBOARD_RETURN || state != MLV_PRESSED || lvl > data->last_world);
    }

    MLV_stop_music();

    currentworld = lvl;
    ret = boucle_de_jeu(data, player, currentworld, fpos, score);
    MLV_change_window_size(MENU_WIDTH, MENU_HEIGHT);
    MLV_play_sound(data->sound_list.victory, data->para.sound);
    MLV_wait_milliseconds(100);
    MLV_play_music(data->sound_list.ambiant[NB_MONDE], data->para.music, -1);
    if (ret == -2) {
        fprintf(stderr, "Error on loading! Stopping the program!\n");
        exit(1);
    }
    if (ret == -1) {
        return 0;
    }
    /* Save score */
    add_scoreboard(data->fname, ret, currentworld);
    if (!player_dead(*player)) {
        if (data->last_world == currentworld && data->last_world < NB_MONDE-1) {
            save_partie(player, data->fname, data->last_world+1, 0, 0, 0);
        } else {
            save_partie(player, data->fname, data->last_world, 0, 0, 0);
        }
    } else {
        return 0;
    }
    return 1;
}

/* Affiche le leaderboard */
void menu_leaderboard() {
    int x, y, frame = 0, lvl = 0, pressed;
    char bg_path[STRING_LEN___];
    MLV_Keyboard_button key;
    MLV_Mouse_button button;
    MLV_Button_state state = MLV_RELEASED;
    MLV_Event event;
    Selection hover, select, holded;
    MLV_Image *bg;
    select = holded = None;
    pressed = 0;
    MLV_get_mouse_position(&x, &y);

    sprintf(bg_path, "data/img/background/background_%d.png", lvl);
    bg = MLV_load_image(bg_path);
    MLV_resize_image_with_proportions(bg, MENU_WIDTH, -1);

    /* sélection */
    do {
        event = MLV_get_event(&key, NULL, NULL, NULL, NULL, &x, &y, &button, &state);
        hover_menu_ldbd(&hover, x, y);
        draw_menu_ldbd(lvl, frame, hover, bg, !pressed);
        if (event == MLV_MOUSE_BUTTON && button == MLV_BUTTON_LEFT) {
            if (state == MLV_PRESSED) {
                holded = hover;
                pressed = 1;
            } else {
                pressed = 0;
                select = hover;
            }
        }
        if (state == MLV_PRESSED) {
            if ((event == MLV_KEY && key == MLV_KEYBOARD_LEFT) || (event == MLV_MOUSE_BUTTON && button == MLV_BUTTON_LEFT && hover == Left)) {
                lvl = lvl == 0 ? 4 : lvl-1;
                MLV_free_image(bg);
                sprintf(bg_path, "data/img/background/background_%d.png", lvl);
                bg = MLV_load_image(bg_path);
                MLV_resize_image_with_proportions(bg, MENU_WIDTH, -1);
            }
            if ((event == MLV_KEY && key == MLV_KEYBOARD_RIGHT) || (event == MLV_MOUSE_BUTTON && button == MLV_BUTTON_LEFT && hover == Rght)) {
                lvl = lvl == 4 ? 0 : lvl+1;
                MLV_free_image(bg);
                sprintf(bg_path, "data/img/background/background_%d.png", lvl);
                bg = MLV_load_image(bg_path);
                MLV_resize_image_with_proportions(bg, MENU_WIDTH, -1);
            }
            if (event == MLV_KEY && key == MLV_KEYBOARD_ESCAPE) {
                break;
            }
        }
        frame++;
    } while (select != holded || holded != Quit || pressed);
}

/* Menu des options
 * Renvoie 1 si les dimentions sont changées 0 sinon */
int menu_options(Parametre* para, int ingame) {
    /* Ajouter les options (le son, les touches, la taille de l'écran ...) */
    MLV_Button_state state = MLV_RELEASED;
    Selection select = None, hover, holded = None;
    MLV_Keyboard_button key;
    MLV_Mouse_button button;
    Slider sliders[2];
    int x, y, pressed = 0, frame = 0, size;
    MLV_Event event;
    MLV_get_mouse_position(&x, &y);

    init_slider(&sliders[0], 0, 100);
    init_slider(&sliders[1], 0, 100);
    set_slider_value(&sliders[0], para->music);
    set_slider_value(&sliders[1], para->sound);
    modif_slider_width(&sliders[0], MENU_WIDTH - 200);
    modif_slider_width(&sliders[1], MENU_WIDTH - 200);
    modif_slider_pos(&sliders[0], 150, MENU_HEIGHT*2/3 + 50);
    modif_slider_pos(&sliders[1], 150, MENU_HEIGHT*2/3 + 100);
    size = para->width;
    /* sélection */
    do {
        event = MLV_get_event(NULL, NULL, NULL, NULL, NULL, &x, &y, &button, &state);
        MLV_clear_window(MLV_COLOR_BLACK);
        hover_menu_optn(&hover, ingame, x, y);
        draw_menu_optn(para, sliders, ingame, hover, holded, 0);
        
        if (event == MLV_MOUSE_BUTTON && button == MLV_BUTTON_LEFT) {
            if (state == MLV_PRESSED) {
                holded = hover;
                pressed = 1;
            } else {
                pressed = 0;
                select = hover;
            }
        }
        if (pressed) {
          if (Up <= holded && holded <= Bons2 && Up <= hover && hover <= Bons2) {
            select = hover;

            do {
              event = MLV_get_event(&key, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
              MLV_clear_window(MLV_COLOR_BLACK);
              draw_menu_optn(para, sliders, ingame, hover, select, frame*2);
              frame++;
            } while (event != MLV_KEY);

            set_new_key(para, key, select);
            select = None;
            holded = None;
            pressed = 0;
          }
          else if (holded == Musc && hover == Musc) {
            set_slider_value(&sliders[0], (x - sliders[0].x)/(sliders[0].w / sliders[0].max));
            para->music = sliders[0].value;
          }
          else if (holded == Sons && hover == Sons) {
            set_slider_value(&sliders[1], (x - sliders[1].x)/(sliders[1].w / sliders[1].max));
            para->sound = sliders[1].value;
          }
          else if (!ingame) {
            if (holded == R600 || holded == R700 || holded == R800) {
              if (hover == R600) {
                para->width = 600;
                para->height = 800;
              }
              else if (hover == R700) {
                para->width = 700;
                para->height = 900;
              }
              else if (hover == R800) {
                para->width = 800;
                para->height = 950;
              }
            }
          }
        }
        if (event == MLV_KEY && key == MLV_KEYBOARD_ESCAPE) {
            break;
        }
        
    } while (select != holded || holded != Quit || pressed);
    save_option(para);
    if (size != para->width)
        return 0;
    return 1;
}

/* Menu pause */
int menu_pause(Parametre* para) {
    MLV_Button_state state = MLV_RELEASED;
    MLV_Keyboard_button key;
    Selection select = None, hover;
    MLV_Mouse_button button;
    Star* tab[50];
    int x, y, pressed, frame;
    int key_hist[MLV_KEYBOARD_LAST];
    MLV_Event event; 
    MLV_get_mouse_position(&x, &y);
    pressed = frame = 0;

    init_tab(key_hist, MLV_KEYBOARD_LAST, 0);
    fill_star_tab(tab, para->width, para->height, 50);
    
    /* sélection */ 
    do {
        event = MLV_get_event(&key, NULL, NULL, NULL, NULL, &x, &y, &button, &state);
        if (frame%2) {
            update_stars(tab, para->width, para->height, 50);
        }
        hover_menu_pause(&hover, x, y, para->width, para->height);

        MLV_clear_window(MLV_COLOR_BLACK);
        draw_stars(tab, 50);
        draw_menu_pause(para->width, para->height, hover, state);

        if (event == MLV_MOUSE_BUTTON && button == MLV_BUTTON_LEFT) {
            if (state == MLV_PRESSED) {
                pressed = 1;
            } else {
                pressed = 0;
                select = hover;
            }
        }
        frame++;
    } while (select == None || pressed);

    switch (select) {
      case Strt:
        return 1;
      case Sett:
        MLV_change_window_size(MENU_WIDTH, MENU_HEIGHT);
        menu_options(para, 1);
        MLV_change_window_size(para->width, para->height + 50);
        return menu_pause(para);
      default:
        return 0;
    }
    return 1;
}


void select_bonus(Power* selected, Parametre* para, int x, int y) {
    int i, j, tmp, bw, bh;
    bw = para->sizes.bonus_width;
    bh = para->sizes.bonus_height;
    tmp = (para->width - 5 * bw)/6;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < NB_BONUS/2; j++) {
            if (in_box(x, y, tmp + j* (tmp + bw), para->height/5 + 2*i*bh, bw, bh)) {
                *selected = i * NB_BONUS/2 + j;
            }
        }
    }
}

void select_bonus_keyboard(Power* selected, MLV_Keyboard_button button, Parametre* para) {
    
    if (button == para->prim_key[Right_key] || button == para->sec_key[Right_key]) {
        if (*selected == Aucun) {
            *selected = NB_BONUS-1;
        } else if (*selected < NB_BONUS-1) {
            *selected+=1;
        }
    } else if (button == para->prim_key[Left_key] || button == para->sec_key[Left_key]) {
        if (*selected == Aucun) {
            *selected = 0;
        } else if (*selected > 0) {
            *selected-=1;
        }
    } else if (button == para->prim_key[Up_key] || button == para->sec_key[Up_key]) {
        if (*selected == Aucun) {
            *selected = 0;
        } else if ((int)*selected - NB_BONUS/2 < 0) {
            *selected = 0;
        } else {
            *selected -= NB_BONUS/2;
        }
    } else if (button == para->prim_key[Down_key] || button == para->sec_key[Down_key]) {
        if (*selected == Aucun) {
            *selected = NB_BONUS-1;
        } else if (*selected + NB_BONUS/2 >= NB_BONUS) {
            *selected = NB_BONUS-1;
        } else {
            *selected += NB_BONUS/2;
        }
    }
}

int buy_bonus(Terrain* terrain, Power selected) {
    if (selected == Aucun)
        return 1;
    if (terrain->player->money < get_bonus_price(selected))
        return 4;
    
    switch (selected) {
        case Heal:
            if (terrain->player->pv < terrain->player->pv_max) { apply_bonus(terrain->player, selected); }
            else if (selected == terrain->player->stock_bonus) { return 2; }
            else { terrain->player->stock_bonus = selected; }
            break;
        case Missile:
            if (terrain->player->nb_missiles < MAX_MISSILES) { apply_bonus(terrain->player, selected); }
            else if (selected == terrain->player->stock_bonus) { return 2; }
            else { terrain->player->stock_bonus = selected; }
            break;
        case Health_boost:
            if (terrain->player->pv_max < MAX_HEALTH_BOOST) { apply_bonus(terrain->player, selected); }
            else {return 3;}
            break;
        case Single_shot:
        case Dual_shot:
        case Triple_shot:
        case Quadra_shot:
            if (terrain->player->perma_bonus == selected) {return 2;}
            else {apply_bonus(terrain->player, selected);}
            break;
        case Shield:
            if (terrain->player->shield < SHIELD_AMOUNT) { apply_bonus(terrain->player, selected); }
            else if (selected == terrain->player->stock_bonus) { return 2; }
            else { terrain->player->stock_bonus = selected; }
            break;
        case Score_multiplier:
        case Bullet_time:
            if (selected == terrain->player->stock_bonus) { return 2; }
            else {terrain->player->stock_bonus = selected;}
            break;
        default:
            printf("Impossibeul\n");
    }
    terrain->player->money -= get_bonus_price(selected);
    return 0;
}

void shop_menu(Parametre* para, Terrain* terrain, ImgList* img_list) {
    MLV_Mouse_button button;
    MLV_Keyboard_button key;
    MLV_Button_state state;
    Power selected = Aucun;
    MLV_Event event;
    int x, y, frame;
    int err_msg;
    
    frame = err_msg = 0;
    
    do {
        draw_shop_menu(terrain, para, img_list, selected, frame/4, &err_msg);
        MLV_actualise_window();
        event = MLV_get_event(&key, NULL, NULL, NULL, NULL, &x, &y, &button, &state);
        if (event == MLV_MOUSE_BUTTON && button == MLV_BUTTON_LEFT && state == MLV_RELEASED) {
            /* Select bonus */
            select_bonus(&selected, para, x, y);
            /* Buy bonus */
            if (in_box(x, y, 3*para->width/4, para->height/2, para->width/4, para->height/12)) {
                err_msg = buy_bonus(terrain, selected);
            }
        } else if (event == MLV_KEY && state == MLV_PRESSED) {
            /* Select bonus */
            select_bonus_keyboard(&selected, key, para);
            /* Buy bonus */
            if (key == para->prim_key[Blaster_key] || key == para->sec_key[Blaster_key]) {
                err_msg = buy_bonus(terrain, selected);
            }
        }
        
        frame++;
    } while (!((event == MLV_MOUSE_BUTTON && in_box(x, y, 3*para->width/4, 7*para->height/12, para->width/4, para->height/12))
            || (event == MLV_KEY && key == MLV_KEYBOARD_ESCAPE)));
    err_msg = -1;
    draw_shop_menu(NULL, NULL, NULL, 0, 0, &err_msg);
}
