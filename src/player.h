#ifndef PLAYER_H
# define PLAYER_H

typedef struct s_player
{
    float   x;
    float   y;
    float   dir;
    float   fov;
}   t_player;

t_player    *create_player(float x, float y, float dir, float fov);
void        free_player(t_player *player);
void        update_player(t_player *player);
void        draw_player(t_player *player);

#endif
