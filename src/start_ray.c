/*
** EPITECH PROJECT, 2025
** Ray-casting
** File description:
** start_ray
*/
#include "call_csfml.h"
#include "init.h"
#include "level.h"
#include <time.h>
#include "game.h"
#include <string.h>

/*
typedef struct game_s {
    entities_handler_t ent;
    level_t level;
    p_uid_t *uid;
    score_t score;
} game_t;
*/

#define FOV 66
#define NB_TEXT 1
/*
** -1 => camera_x => 1
*/

typedef struct ray_cast_s{
    /* calcul */
    double camera_x;
    double raydir_x;
    double raydir_y;
    int map_x;
    int map_y;
    double delta_dist_x;
    double delta_dist_y;
    double perp_wall_dist;
    int step_x;
    int step_y;
    double side_dist_x;
    double side_dist_y;
    int hit;
    int side;

    /* texture */
    sfTexture *texture[NB_TEXT];
    double wall_x;
    int tex_x;
    sfUint8 *pixels;
    sfImage *image;
    sfTexture *screen_text;
    sfSprite *sprite_screen;
    

    /* display */
    int height_line;
    int draw_start;
    int draw_end;
}ray_cast_t;

/*
** buffer[VARIATION LIGNE][VARIATIOB BOUCLE PRINCIPALE] = color
*/

void init_ray_cast(ray_cast_t *ray_cast, entities_t *player, int x)
{
    ray_cast->camera_x = 2 * x / (double)(1980) - 1; 
    ray_cast->raydir_x = player->dir.x + player->plan.x * ray_cast->camera_x;
    ray_cast->raydir_y = player->dir.y + player->plan.y * ray_cast->camera_x;
    ray_cast->map_x = (int)(player->coord.x);
    ray_cast->map_y = (int)(player->coord.y);
    ray_cast->delta_dist_x = (ray_cast->raydir_x == 0) ? 1e30 : fabs(1 / ray_cast->raydir_x);
    ray_cast->delta_dist_y = (ray_cast->raydir_y == 0) ? 1e30 : fabs(1 / ray_cast->raydir_y);
    ray_cast->hit = 0;
}

void prep_dda(ray_cast_t *ray_cast, entities_t *player)
{
    if (ray_cast->raydir_x < 0){
      ray_cast->step_x = -1;
      ray_cast->side_dist_x = (player->coord.x - ray_cast->map_x) * ray_cast->delta_dist_x;
    }
    else {
        ray_cast->step_x  = 1;
        ray_cast->side_dist_x = (ray_cast->map_x + 1.0 - player->coord.x ) * ray_cast->delta_dist_x;
    }
    if (ray_cast->raydir_y < 0){
        ray_cast->step_y = -1;
        ray_cast->side_dist_y  = (player->coord.y - ray_cast->map_y) * ray_cast->delta_dist_y;
    }
    else{
        ray_cast->step_y = 1;
        ray_cast->side_dist_y = (ray_cast->map_y + 1.0 - player->coord.y) * ray_cast->delta_dist_y;
    } 
}

void execute_dda(ray_cast_t *ray_cast, int map[100][100])
{
    while (ray_cast->hit == 0) {
      if (ray_cast->side_dist_x < ray_cast->side_dist_y) {
        ray_cast->side_dist_x += ray_cast->delta_dist_x;
        ray_cast->map_x += ray_cast->step_x;
        ray_cast->side = 0;
      }
      else {
        ray_cast->side_dist_y += ray_cast->delta_dist_y;
        ray_cast->map_y += ray_cast->step_y;
        ray_cast->side = 1;
      }
      if (map[ray_cast->map_x][ray_cast->map_y] > 0) 
        ray_cast->hit = 1;
    }
}

void cancel_fisheye(ray_cast_t *ray_cast)
{
    if (ray_cast->side == 0)
        ray_cast->perp_wall_dist = (ray_cast->side_dist_x - ray_cast->delta_dist_x);
    else 
        ray_cast->perp_wall_dist = (ray_cast->side_dist_y - ray_cast->delta_dist_y);
}

#define H 1080

void det_line(ray_cast_t *ray_cast)
{
    int lineHeight = (int)(H / ray_cast->perp_wall_dist);
    int drawStart = -lineHeight / 2 + H / 2;
    int drawEnd = lineHeight / 2 + H / 2;

    ray_cast->height_line = lineHeight;
    ray_cast->draw_start = (drawStart < 0) ? 0 : drawStart;
    ray_cast->draw_end = (drawEnd >= H) ? H - 1 : drawEnd;
}

void draw_line(sfRenderWindow *window, int x, ray_cast_t *ray_cast)
{
    sfRectangleShape *line = sfRectangleShape_create();
    int height = ray_cast->draw_end - ray_cast->draw_start;
    sfIntRect area = {ray_cast->tex_x, 0, 1, 64};

    if (ray_cast->perp_wall_dist < 1){
        area.height = (64 * ray_cast->perp_wall_dist);
    }

    sfRectangleShape_setSize(line, (sfVector2f){1, height});
    sfRectangleShape_setPosition(line, (sfVector2f){x, ray_cast->draw_start});

    sfRectangleShape_setTexture(line, ray_cast->texture[0], sfFalse);
    sfRectangleShape_setTextureRect(line, area);

    sfRenderWindow_drawRectangleShape(window, line, NULL);
    sfRectangleShape_destroy(line);
}

/*
** get coord of the pixel in the picture
** ensuite on peut fil le buffer[][] avec x
** la boucle principale
*/

void get_text_pixel(ray_cast_t *ray_cast, entities_t *player)
{   
    ray_cast->wall_x = (ray_cast->side == 0) 
        ? player->coord.y + ray_cast->perp_wall_dist * ray_cast->raydir_y
        : player->coord.x + ray_cast->perp_wall_dist * ray_cast->raydir_x;
    ray_cast->wall_x -= floor((ray_cast->wall_x));
    ray_cast->tex_x = (int)(ray_cast->wall_x * (double)(64));
    //if( ray_cast->side == 0 && ray_cast->raydir_x > 0)
    //    ray_cast->tex_x = 64 - ray_cast->tex_x  - 1;
    //if( ray_cast->side == 1 && ray_cast->raydir_x < 0)
    //    ray_cast->tex_x = 64 - ray_cast->tex_x  - 1;
}

/*
** fill dans le buffer toute la ligne grace
** a get_pixel
*/

void choose_color_wall(sfRenderWindow *window, ray_cast_t *ray_cast, int map[100][100], int x)
{
    sfColor color;

    switch(map[ray_cast->map_x][ray_cast->map_y])//SWITCH POUR DETERMINER LA TEXTURE
    {
      case 1:  color = sfRed;  break; //red
      case 2:  color = sfGreen;  break; //green
      case 3:  color = sfBlue;   break; //blue
      case 4:  color = sfWhite;  break; //white
      default: color = sfYellow; break; //yellow
    }

    if (ray_cast->side == 1){
        color.r /= 2;
        color.g /= 2;
        color.b /= 2;
    }
    //draw the pixels of the stripe as a vertical line
    draw_line(window, x, ray_cast);
    //sfTexture *screen_texture = sfTexture_create(1980, 1080);
    //sfSprite *screen_sprite = sfSprite_create();

    //sfTexture_updateFromPixels(screen_texture, ray_cast->pixels, 1980, 1080, 0, 0);
    //sfSprite_setTexture(screen_sprite, screen_texture, sfTrue);
    //sfRenderWindow_drawSprite(window, screen_sprite, NULL);
        
}

/*
** prend un sfcolor et le place dans un buffer aplatie 1D
*/
void put_color_in_buffer(sfUint8 *array_pixel, sfColor new_pixel, int index_storage)
{
    array_pixel[index_storage] = new_pixel.r;
    array_pixel[index_storage + 1] = new_pixel.g;
    array_pixel[index_storage + 2] = new_pixel.b;
    array_pixel[index_storage + 3] = new_pixel.a;
}

void fill_line(ray_cast_t *ray_cast, int x)
{
    sfColor pixel_to_add = {0, 0, 0, 0};
    double step = 1.0 * 64 / ray_cast->height_line;
    double tex_pos = (ray_cast->draw_start - H / 2 + ray_cast->height_line / 2) * step;
    int tex_y = 0;
    int index = 0;

    for (int y = ray_cast->draw_start; y < ray_cast->draw_end; y++) {
        tex_y = (int)tex_pos % 64;  // masque binaire correct
        ray_cast->tex_x = ray_cast->tex_x % 64;
        tex_pos += step;
        pixel_to_add = sfImage_getPixel(ray_cast->image, ray_cast->tex_x, tex_y);
        index = (y * 1980 + x) * 4;  // index dans buffer 1D
        put_color_in_buffer(ray_cast->pixels, pixel_to_add, index);
    }
}

int draw_screen(ray_cast_t *ray_cast, sfRenderWindow *window)
{
    sfTexture_updateFromPixels(ray_cast->screen_text, ray_cast->pixels, 1980, 1080, 0, 0);
    sfSprite_setTexture(ray_cast->sprite_screen, ray_cast->screen_text, sfTrue);
    sfRenderWindow_drawSprite(window, ray_cast->sprite_screen, NULL);
    return 0;
}

int execute_ray(entities_t *player, sfRenderWindow *window, int map[100][100], ray_cast_t ray_cast)
{
    //ray_cast.pixels = malloc(1980 * 1080 * 4); // RGBA = 4 octets par pixel
    //memset(ray_cast.pixels, 0, 1980 * 1080 * 4); 
    
    for (int x = 0; x < 1980; x++){
        init_ray_cast(&ray_cast, player, x);
        prep_dda(&ray_cast, player);
        execute_dda(&ray_cast, map);
        cancel_fisheye(&ray_cast);
        det_line(&ray_cast);
        get_text_pixel(&ray_cast, player);
        fill_line(&ray_cast, x);
        //choose_color_wall(window, &ray_cast, map, x);
    }
    draw_screen(&ray_cast, window);
    return 0;
}

static
void moove_up(entities_t *entities)
{
    entities->coord.x += entities->dir.x / 100;
    entities->coord.y += entities->dir.y / 100;
}

void moove_player(entities_t *player)
{
    if (sfKeyboard_isKeyPressed(sfKeyZ))
        moove_up(player);
}

int start_ray(game_t *game_handler, sfRenderWindow *window)
{
    player_t *player = game_handler->ent.player;
    entities_t *player_e = player->property;
    sfEvent event;
    ray_cast_t ray_cast;
   
    ray_cast.screen_text = sfTexture_create(1980, 1080);
    ray_cast.sprite_screen = sfSprite_create();
    ray_cast.texture[0] = sfTexture_createFromFile("libdoor.png", NULL);
    ray_cast.image = sfTexture_copyToImage(ray_cast.texture[0]);
    ray_cast.pixels = malloc (1080 * 1980 * 4);// Allocation via malloc car
    memset(ray_cast.pixels, 0, 1980 * 1080 * 4);// la pile pour stack est trop petite
    if (ray_cast.pixels == NULL)
        return 84;
    while (sfRenderWindow_isOpen(window)){
        sfRenderWindow_pollEvent(window, &event);
        if (event.type == sfEvtClosed)
            sfRenderWindow_close(window);
        sfRenderWindow_clear(window, sfBlack); 
        execute_ray(player_e, window, game_handler->level.map[0], ray_cast);
        rotate_player(game_handler->ent.player->property, 0.01);
        moove_player(game_handler->ent.player->property);
        memset(ray_cast.pixels, 0, 1980 * 1080 * 4);
        sfRenderWindow_display(window);
    }
    sfTexture_destroy(ray_cast.texture[0]);
    sfImage_destroy(ray_cast.image);
    return 0;
}