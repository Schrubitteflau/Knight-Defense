#ifndef __WAY_H__
#define __WAY_H__

#include <math.h>
#include "base.h"

#define				diff_non_zero(a, b)			(abs(a - b) == 0 ? 1 : abs(a - b))
#define				MAX_RANGE_DISABLED			-1
#define				distance(a, b)				sqrt((double)pow((double)(b.x - a.x), 2.0) + pow((double)(b.y - a.y), 2.0))


// Utilisés pour initialiser des chemins (voir pr_init_way())
typedef enum PROJECT_TARGET { FIRST_TARGET, LAST_TARGET } PROJECT_TARGET;
typedef enum PROJECT_ORIGIN { FIRST_ORIGIN, SECOND_ORIGIN, THIRD_ORIGIN, LAST_ORIGIN } PROJECT_ORIGIN;


// Structure définissant un chemin (pour les ennemis par exemple)
// avec un ensemble de points, et le nombre de points
typedef struct PROJECT_WAY
{
	PROJECT_COORD* all_points;
	int size;
} PROJECT_WAY;

// Structure définissant une ligne : un point de départ et un point d'arrivée
typedef struct PROJECT_LINE
{
	PROJECT_COORD first_point;
	PROJECT_COORD last_point;
} PROJECT_LINE;


// Prototypes
PROJECT_COORD* pr_get_all_points_between(PROJECT_COORD p1, PROJECT_COORD p2, int *taille);
PROJECT_WAY* pr_create_way(PROJECT_LINE* lines, int number);
PROJECT_COORD pr_get_point_direction_with_range(PROJECT_COORD origine, PROJECT_COORD cible, int rayon);
PROJECT_LINE* pr_init_way(int target_seed, int origin_seed, int *size_tab);
PROJECT_WAY** pr_create_all_ways(int *size_tab);


#endif