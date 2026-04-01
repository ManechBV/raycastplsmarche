#ifndef RAYCASTER_H
# define RAYCASTER_H

#include "raylib.h"
#include "intMap.h"
#include "player.h"

typedef enum    e_coll_dir
{
    NORTH,
    SOUTH,
    EAST,
    WEST
}   t_coll_dir;

typedef struct  s_ray_info
{
    float           length;
    int             map_coll_x;
    int             map_coll_y;
    int             map_coll_val;
    unsigned int    texture_off;
    t_coll_dir      coll_dir;
}   t_ray_info;

t_ray_info  *cast_rays(t_intMap *map, t_player *player, unsigned int out_w);
void        render_ray_infos_to_img(t_ray_info *ray_infos, Image *img, Image *wall_imgs, int scale);

#endif
