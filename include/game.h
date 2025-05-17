/*
** EPITECH PROJECT, 2025
** minishell
** File description:
** my
*/


#ifndef GAME_H
    #define GAME_H
    #include "entities.h"
    #include "call_csfml.h"
    #include "ray_casting.h"
    #include "controller.h"
    #include "menu.h"
    #include "level.h"
    #include "in_game.h"
    #include <stdbool.h>
    #define WINDOW_SIZE_X 1980
    #define WINDOW_SIZE_y 1080
    #define FPS_SET_60 16.666
    #define WALK 100
    #define RUN 200
    #define M_PI 3.14159265358979323846
    #define RAY_SCREEN (1980.0f / 1440.0f)
    #define DEG_TO_RAD(angle) ((angle) * (M_PI / 180.0))
    #define S_START 0
    #define S_PAUSE 1
    #define S_SETTING 2
    #define S_END 3
    #define S_LORE 4
    #define S_GAME 5
    #define WINM_FSCREEN 0
    #define WINM_BIG 1
    #define WINM_LITTLE 2
int wolf3d(void);
/*
** Entitie handler
** contient toute les
** entité
*/

typedef struct setting_s {
    float music_volume;
    float sound_volume;
    size_t win_mode;
    sfVector2f win_size[2];
} setting_t;

typedef struct screen_s {
    menu_t *menu;
    size_t last_menu;
    sfRenderWindow *win;
    sfVideoMode win_mode;
} screen_t;

typedef struct game_handler_s {
    sfEvent event;
    time_management_t general_clock;
    setting_t settings;
    int screen_mode;
    screen_t screen;
    tab_texture_t *texture;
    game_t *game;
    sfTexture *wall;
    sfRectangleShape *square;
    buffer_z_t *buffer_z;
    controller_t controller;
}game_handler_t;

typedef struct entities_class_s entities_class_t;

typedef struct game_mode_s {
    int (*funct)(game_handler_t *);
} game_mode_t;

int launch_game(game_handler_t *game_handler);

/*Game mode*/
int choose_window_mode(game_handler_t *game_handler);
int display_full_map(game_handler_t *game_handler);
int display_menu(game_handler_t *g_handler);

/*Player*/
void move_player(entities_class_t *entities);
void rotate_player(entities_t *player, float rot_speed);
void manage_player(game_handler_t *game_handler);

/*3D*/
float get_coord_wall(entities_class_t *entities, float angle, int precision);
int display_3d_map(game_handler_t *game_handler);
void free_game(game_handler_t *game_handler);
void check_back_to_menu(game_handler_t *handler);

/*get*/
sfVector2f get_coord(sfVector2f coord_window);
bool is_in_real_wall(sfVector2f coord);

/*Controller*/
bool check_l3(void);
bool check_l2(void);
bool check_l1(void);
sfUint8 get_left_stick(void);

bool check_r3(void);
bool check_r2(void);
bool check_r1(void);
sfUint8 get_right_stick(void);

/*Time and clock*/
bool need_refresh_clock(time_management_t *sound_clock, float sec_reset);

#endif //GAME_H
