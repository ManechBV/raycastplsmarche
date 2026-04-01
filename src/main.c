#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define SCALE 64
#define OUT_W 640
#define OUT_H 360

#include "utils.h"
#include "intMap.h"
#include "player.h"
#include "raycaster.h"

int main(void)
{
    InitWindow(1280, 720, "Raycaster by Namek");
    SetTargetFPS(60);

    t_intMap    *Map;
    Map = create_intMap(16, 16, SCALE);
    if (!Map)   return (-1);

    generate_random_intMap(Map, 2, 4);
    generate_border_walls(Map);

    t_player    *Player;
    Player = create_player(200.0f, 200.0f, 0.0f, deg_to_rad(60.0f)); 
    if (!Player)   return (-1);

    t_ray_info  *ray_infos;
    Image       out_img = GenImageColor(OUT_W, OUT_H, GRAY);
    Texture2D   render_texture = LoadTextureFromImage(out_img);

    Image   wall_imgs[2] = {
        LoadImage("res/wallhd.png"),
        LoadImage("res/jonkler.jpg")
    };

    while (!WindowShouldClose())
    {
        update_player(Player);
        ray_infos = cast_rays(Map, Player, OUT_W);
        render_ray_infos_to_img(ray_infos, &out_img, wall_imgs, SCALE);
        free(ray_infos);
        UpdateTexture(render_texture, out_img.data);

        BeginDrawing();
            ClearBackground(BLACK);
            //draw_intMap(Map);
            //draw_player(Player);
            DrawTextureEx(render_texture, (Vector2){0, 0}, 0.0, 2.0, WHITE);
        EndDrawing();
    }

    UnloadImage(wall_imgs[0]);
    UnloadImage(wall_imgs[1]);
    UnloadTexture(render_texture);
    UnloadImage(out_img);

    free_player(Player);
    free_intMap(Map);

    CloseWindow();

    return 0;
}
