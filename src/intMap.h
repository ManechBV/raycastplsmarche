#ifndef INT_MAP_H
# define INT_MAP_H

typedef struct s_intMap
{
    int             **map;
    unsigned int    w;
    unsigned int    h;
}   t_intMap;

t_intMap    *create_intMap(unsigned int w, unsigned int h);
void        draw_intMap(t_intMap *map, int scale);
void        generate_random_intMap(t_intMap *map, int occ);

#endif
