#include "raycaster.h"
#include "intMap.h"
#include "utils.h"
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX_ITER 32

t_ray_info  *cast_rays(t_intMap *map, t_player *player, unsigned int out_w)
{
    t_ray_info  *ray_info;
    float       dir = player->dir + (player->fov / 2);
    float       dir_step = player->fov / (float)out_w;
    float       x = player->x;
    float       y = player->y;

    ray_info = calloc(out_w, sizeof(t_ray_info));
    if (!ray_info)
    {
        printf("Failed to calloc rayinfo raycaster.c.\n");
        return NULL;
    }
    for (int i = 0; i < out_w; i++)
    {
        correct_dir(&dir);
        float           h_len;
        float           v_len;
        t_coll_dir      coll_dir_h;
        t_coll_dir      coll_dir_v;
        int             map_id[4];
        unsigned int    texture_off_v;
        unsigned int    texture_off_h;

        x = player->x;
        y = player->y;
        // horizontal check
        {
            float   first_step_dy;
            if (dir > deg_to_rad(90) && dir < deg_to_rad(270))
                first_step_dy = (((int)(y / map->scale)) * map->scale) - y;
            else
                first_step_dy = (((int)(y / map->scale) + 1) * map->scale) - y;

            float   first_step_dx = tanf(dir) * first_step_dy;

            x += first_step_dx;
            y += first_step_dy;

            float   step_x;
            if (dir > deg_to_rad(90) && dir < deg_to_rad(270))
                step_x = tanf(dir) * ((float)map->scale * -1);
            else
                step_x = tanf(dir) * ((float)map->scale);

            bool    collide = false;
            int     iter = 0;
            while (collide == false && iter++ < MAX_ITER)
            {
                int map_y;
                if (dir > deg_to_rad(90) && dir < deg_to_rad(270))
                    map_y = y / map->scale - 1;
                else
                    map_y = y / map->scale;

                int map_x = x / map->scale;

                if (map_x >= map->w || map_y >= map->h)
                    collide = true;
                else if (map->map[map_y][map_x] != 0)
                {
                    collide = true;
                    map_id[0] = map_x;
                    map_id[1] = map_y;

                    if (dir > deg_to_rad(90) && dir < deg_to_rad(270))
                        texture_off_h = (unsigned int)x % (unsigned int)map->scale;
                    else
                        texture_off_h = map->scale - ((unsigned int)x % (unsigned int)map->scale);
                }
                else
                {
                    if (dir > deg_to_rad(90) && dir < deg_to_rad(270))
                        y -= map->scale;
                    else
                        y += map->scale;
                    x += step_x;
                }
            }
            h_len = dist(player->x, player->y, x, y);
            if (dir > deg_to_rad(90) && dir < deg_to_rad(270))
                coll_dir_h = SOUTH;
            else
                coll_dir_h = NORTH;
        }

        x = player->x;
        y = player->y;
        // vertical check
        {
            float   first_step_dx;
            if (dir > deg_to_rad(180))
                first_step_dx = (((int)(x / map->scale)) * map->scale) - x;
            else
                first_step_dx = (((int)(x / map->scale) + 1) * map->scale) - x;

            float   new_dir = dir - deg_to_rad(90);
            correct_dir(&new_dir);
            float   first_step_dy = tanf(new_dir) * (first_step_dx * -1);

            x += first_step_dx;
            y += first_step_dy;

            float   step_y;
            if (dir > deg_to_rad(180))
                step_y = tanf(new_dir) * ((float)map->scale);
            else
                step_y = tanf(new_dir) * ((float)map->scale * -1);

            bool    collide = false;
            int     iter = 0;
            while (collide == false && iter++ < MAX_ITER)
            {
                int map_x;
                if (dir > deg_to_rad(180))
                    map_x = x / map->scale - 1;
                else
                    map_x = x / map->scale;

                int map_y = y / map->scale;

                if (map_x >= map->w || map_y >= map->h)
                    collide = true;
                else if (map->map[map_y][map_x] != 0)
                {
                    collide = true;
                    map_id[2] = map_x;
                    map_id[3] = map_y;
                    if (dir > deg_to_rad(180))
                        texture_off_v = map->scale - ((unsigned int)y % (unsigned int)map->scale);
                    else
                        texture_off_v = (unsigned int)y % (unsigned int)map->scale;
                }
                else
                {
                    if (dir > deg_to_rad(180))
                        x -= map->scale;
                    else
                        x += map->scale;
                    y += step_y;
                }
            }
            v_len = dist(player->x, player->y, x, y);
            if (dir > deg_to_rad(180))
                coll_dir_v = WEST;
            else
                coll_dir_v = EAST;
        }
        // debug lines
        /*
        DrawLineEx((Vector2){player->x, player->y}, 
                (Vector2){player->x + sinf(dir) * h_len, player->y + cosf(dir) * h_len},
                5.0, GREEN); 
        DrawLineEx((Vector2){player->x, player->y}, 
                (Vector2){player->x + sinf(dir) * v_len, player->y + cosf(dir) * v_len},
                2.0, RED); 
        */
        if (v_len < h_len)
        {
            ray_info[i].length = v_len;
            ray_info[i].coll_dir = coll_dir_v;
            ray_info[i].map_coll_x = map_id[2];
            ray_info[i].map_coll_y = map_id[3];
            ray_info[i].texture_off = texture_off_v;
        }
        else
        {
            ray_info[i].length = h_len;
            ray_info[i].coll_dir = coll_dir_h;
            ray_info[i].map_coll_x = map_id[0];
            ray_info[i].map_coll_y = map_id[1];
            ray_info[i].texture_off = texture_off_h;
        }
        if (ray_info[i].map_coll_x >= map->w || ray_info[i].map_coll_y >= map->h)
            ray_info[i].map_coll_val = 0;
        else
            ray_info[i].map_coll_val = map->map[ray_info[i].map_coll_y][ray_info[i].map_coll_x];

        float   diff_dir_pdir = dir - player->dir;
        correct_dir(&diff_dir_pdir);
        ray_info[i].length *= cosf(diff_dir_pdir);
        
        /*
        DrawLineEx((Vector2){player->x, player->y}, 
                (Vector2){player->x + sinf(dir) * ray_info[i].length, player->y + cosf(dir) * ray_info[i].length},
                1.0, RED); 
        */
        dir -= dir_step;
    }
    return ray_info;
}

void    render_ray_infos_to_img(t_ray_info *ray_infos, Image *img, Image *wall_imgs, int scale)
{
    ImageClearBackground(img, BLACK);
    for (int i = 0; i < img->width; i++)
    {
        float line_h = (img->height * scale) / ray_infos[i].length;
        //if (line_h > img->height)
        //    line_h = img->height;

        Image   *curr_img = NULL;

        if (ray_infos[i].map_coll_val != 0)
            curr_img = &wall_imgs[ray_infos[i].map_coll_val - 1];
        else
            continue;
        int wall_img_x = (ray_infos[i].texture_off * (curr_img->width - 1)) / scale;
        for (int j = 0; j < (int)line_h; j++)
        {
            int wall_img_y = (j * curr_img->height) / line_h;
            Color   col = WHITE;
            if (wall_img_y < curr_img->height && wall_img_x < curr_img->width)
                col = GetImageColor(*curr_img, wall_img_x, wall_img_y);
            if (ray_infos[i].coll_dir == NORTH || ray_infos[i].coll_dir == SOUTH)
            {
                col.r /= 1.5;
                col.g /= 1.5;
                col.b /= 1.5;
            }
            ImageDrawPixel(img, i, (img->height / 2) - (line_h / 2) + j, col);
        }
        /*
        Color col = WHITE;
        if (ray_infos[i].coll_dir == NORTH || ray_infos[i].coll_dir == SOUTH)
        {
            col.r /= 1.2;
            col.g /= 1.2;
            col.b /= 1.2;
        }
        col.r = (ray_infos[i].texture_off * 255) / 64;
        ImageDrawLine(img, i, (img->height / 2) - (line_h / 2), i, (img->height / 2) + (line_h / 2), col);
        */
    }
}
