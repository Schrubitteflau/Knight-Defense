#include "error.h"
#include "way.h"


// Fonction qui retourne un tableau dynamique contenant tous les points (x, y) d'une
// ligne à partir du point p1 vers le point p2. max_range est optionnel, mais il est
// possible de dire à la fonction de s'arrêter au boût d'une certaine distance par rapport
// au point de départ
// Fonction qui retourne un tableau dynamique contenant tous les points (x, y) d'une
// ligne à partir du point p1 vers le point p2
PROJECT_COORD* pr_get_all_points_between(PROJECT_COORD p1, PROJECT_COORD p2, int *taille)
{
	PROJECT_COORD *points_tab, *temp_ptr;
	int i;
	int X, Y;
	int result, pasx, pasy;
	int absX, absY;
	int compteur = 0;

	// Si les 2 points sont les mêmes, alors on n'alloue rien du tout et on retourne la valeur NULL
	if (p1.x == p2.x && p1.y == p2.y)
	{
		pr_error("Les 2 points sont identiques");
		return NULL;
	}

	/* Il faut absolument prévoir 1 zone en mémoire (4 octets car sizeof(PROJECT_COORD) = 4) dans le tableau dynamique
	dans le cas (p1.x == p2.x || p1.y == p2.y) est vrai, car cela signifie que l'on trace une ligne droite. Or dans ce
	cas, la différence en x ou y entre les 2 points est nulle, donc on n'alloue en mémoire que l'équivalent de la
	différence des 2 points sur l'autre axe. Il manquera donc 1 espace (4 octets) d'alloué, ce qui va conduire à un
	débordement sur le tas "heap corruption". Par exemple :

	On veut récupérer l'ensemble des points entre les points (2, 2) et (2, 4).
	On va donc allouer abs(2 - 2) + abs(2 - 4) = 0 + 2 = 2 espaces mémoires, chacun pour contenir 1 coordonnée du type (x, y)
	Or, pour relier les points (2, 2) et (2, 4), 3 coordonnées vont être nécessaires dans le tableau : (2, 2), (2, 3) et (2, 4)
	C'est la raison pour laquelle il faut vérifier la différence entre ces 2 points, sur les 2 axes. Ce qui est fait à l'aide
	d'une macrofonction : diff_non_zero(a, b). */


	// Il est possible de forcer la fonction à le prendre en compte que les 'max_size' premiers points
	// si cette valeur ne faut pas -1

	/* On alloue un tableau dynamique d'une taille forcément suffisante par rapport à ce qui va être stocké
	dedans : les coordonnées en x et y (entiers) qui vont parcourir la ligne de p1.x, p1.y à p2.x, p2.y, le dernier
	point p2.x, p2.y étant inclus dans le tableau. */
	points_tab = (PROJECT_COORD*)calloc(diff_non_zero(p2.x, p1.x) + diff_non_zero(p2.y, p1.y), sizeof(PROJECT_COORD));


	if (points_tab == NULL)
	{
		pr_error("malloc()");
		return NULL;
	}

	X = p2.x - p1.x;
	Y = p2.y - p1.y;
	absX = abs(X);
	absY = abs(Y);
	pasx = (X > 0) ? 1 : -1;
	pasy = (Y > 0) ? 1 : -1;
	result = 0;
	al_draw_pixel(p1.x, p1.x, BLUE);

	if (X == 0)
	{
		for (i = 0; i < absY; i++)
		{
			p1.y += pasy;

			points_tab[compteur].x = p1.x;
			points_tab[compteur++].y = p1.y;
		}
	}
	else if (Y == 0)
	{
		for (i = 0; i < absX; i++)
		{
			p1.x += pasx;

			points_tab[compteur].x = p1.x;
			points_tab[compteur++].y = p1.y;
		}
	}
	else if (absX == absY)
	{
		for (i = 0; i < absX; i++)
		{
			p1.x += pasx;
			p1.y += pasy;

			points_tab[compteur].x = p1.x;
			points_tab[compteur++].y = p1.y;
		}
	}
	else if (absX > absY)
	{
		for (i = 0; i < absX; i++)
		{
			p1.x += pasx;
			result += absY;
			if (result >= absX)
			{
				result -= absX;
				p1.y += pasy;
			}

			points_tab[compteur].x = p1.x;
			points_tab[compteur++].y = p1.y;
		}
	}
	else {
		for (i = 0; i < absY; i++)
		{
			p1.y += pasy;
			result += absX;
			if (result >= absY)
			{
				result -= absY;
				p1.x += pasx;
			}

			points_tab[compteur].x = p1.x;
			points_tab[compteur++].y = p1.y;
		}
	}


	// Puis, comme on sait que l'on a alloué un peu trop de mémoire au début, on corrige ça avec un realloc
	// Déjà, on sauvegarde le pointeur sur le tableau de coordonnées généré
	temp_ptr = points_tab;
	// Puis on modifie la taille du bloc mémoire alloué pour que celui-ci convienne exactement au nombre
	// de données de type PROJECT_COORD alloué
	points_tab = realloc(points_tab, compteur * sizeof(PROJECT_COORD));
	if (points_tab == NULL)
	{
		free(temp_ptr);
		pr_error("realloc()");
		return NULL;
	}
	// On obtient donc un tableau de la bonne taille (compteur) que l'on retourne

	// On renseigne aussi la taille du tableau (nombre de points) par référence
	*taille = compteur;

	return points_tab;
}


// Algorithme retournant le point le plus proche dans la direction donnée (du point origine
// vers cible), avec un rayon défini. Retourne donc le point du cercle le plus proche du point
// d'arrivée
PROJECT_COORD pr_get_point_direction_with_range(PROJECT_COORD origine, PROJECT_COORD cible, int rayon)
{
	int x, y;
	int F1, F2;
	int F, i;
	PROJECT_COORD points[4];
	PROJECT_COORD closer_point = { 0b0111111111111111, 0b0111111111111111 };
	int closer_distance = 0b01111111111111111111111111111111;

	x = origine.x;
	y = origine.y + rayon;
	F = 0;

	while (y >= origine.y)
	{
		points[0] = (PROJECT_COORD) { x, y };
		points[1] = (PROJECT_COORD) { 2 * origine.x - x, y };
		points[2] = (PROJECT_COORD) { 2 * origine.x - x, 2 * origine.y - y };
		points[3] = (PROJECT_COORD) { x, 2 * origine.y - y };

		// Pour chaque point du cercle
		for (i = 0; i < 4; i++)
		{
			// Si le point du cercle est plus proche que celui qui était le plus proche du point
			// recherché avant lui, alors on enregistre sa distance et ses coordonnées
			if ((int)distance(points[i], cible) < closer_distance)
			{
				closer_distance = (int)distance(points[i], cible);
				closer_point = (PROJECT_COORD) { points[i].x, points[i].y };
			}
		}
		
		F1 = F + 1 + 2 * (x - origine.x);
		F2 = F + 1 - 2 * (y - origine.y);

		if(abs(F1) < abs(F2))
		{
		   x += 1;
		   F = F1;
		}
		else
		{
		   y -= 1;
		   F = F2;
		}
	}

	return closer_point;
}


// Fonction qui met bout à bout le tableau de lignes pour ne former qu'une suite de
// coordonnées qui vont représenter un chemin.
PROJECT_WAY* pr_create_way(PROJECT_LINE *lines, int number)
{
	PROJECT_WAY *new_way;
	PROJECT_COORD **all_lines;
	int *size_tab;
	int total_size = 0;
	int total = 0;
	int i, j;


	if (number < 1)
	{
		pr_error("Le nombre de ligne doit etre superieur a 0");
		return NULL;
	}

	// Tableau qui contiendra toutes les tailles de chaque ligne
	size_tab = (int*)malloc(number * sizeof(int));
	if (size_tab == NULL)
	{
		pr_error("malloc()");
		return NULL;
	}

	// Tableau qui contiendra les pointeurs de l'ensemble des points de chaque ligne
	all_lines = (PROJECT_COORD**)malloc(number * sizeof(PROJECT_COORD*));
	if (all_lines == NULL)
	{
		free(size_tab);
		pr_error("malloc()");
		return NULL;
	}

	// On a génère tous les points de toutes les lignes et on récupère la
	// taille cumulée (nombre de points de toutes ces lignes)
	for (i = 0; i < number; i++)
	{
 		all_lines[i] = pr_get_all_points_between(lines[i].first_point, lines[i].last_point, size_tab + i);
		if (all_lines[i] == NULL)
			pr_exit_with_last_error();
		total_size += size_tab[i];
	}


	// Puis on alloue de la mémoire, juste ce qu'il faut pour contenir tous les
	// points de chaque ligne de manière contigue, à la suite
	new_way = (PROJECT_WAY*)malloc(sizeof(PROJECT_WAY));
	if (new_way == NULL)
	{
		free(size_tab);
		for (i = 0; i < number; i++)
			free(all_lines[i]);
		free(all_lines);
		pr_error("malloc()");
		return NULL;
	}

	new_way->all_points = (PROJECT_COORD*)malloc(sizeof(PROJECT_COORD) * total_size);
	if (new_way->all_points == NULL)
	{
		free(size_tab);
		for (i = 0; i < number; i++)
			free(all_lines[i]);
		free(all_lines);
		free(new_way);
		pr_error("malloc()");
		return NULL;
	}

	new_way->size = total_size;


	// Et on copie les points générés de chaque ligne dans le même tableau, à la suite
	for (i = 0; i < number; i++)
	{
		for (j = 0; j < size_tab[i]; j++)
		{
			new_way->all_points[total++] = all_lines[i][j];
		}
	}

	// Enfin, on libère la mémoire qui ne servira plus
	free(size_tab);
	for (i = 0; i < number; i++)
		free(all_lines[i]);
	free(all_lines);

	return new_way;
}


// Retourne un tableau de lignes que l'on peut passer à pr_create_way() pour générer un chemin
// en fonction des paramètres target_seed et origin_seed
PROJECT_LINE* pr_init_way(int target_seed, int origin_seed, int *size_tab)
{
	PROJECT_COORD origin;
	PROJECT_COORD target = target_seed ? (PROJECT_COORD) { 990, 105 } : (PROJECT_COORD) { 620, 655 };
	PROJECT_LINE *lines = NULL;

	switch (origin_seed)
	{
		case FIRST_ORIGIN:
			origin = (PROJECT_COORD) { 0, 125 };
			break;
		
		case SECOND_ORIGIN:
			origin = (PROJECT_COORD) { 0, 315 };
			break;

		case THIRD_ORIGIN:
			origin = (PROJECT_COORD) { 0, 575 };
			break;

		case LAST_ORIGIN:
			origin = (PROJECT_COORD) { 1280, 355 };
			break;
	}

	if (origin.x == 0 && origin.y == 125)
	{
		if (target.x == 990 && target.y == 105)
		{
			*size_tab = 3;

			lines = (PROJECT_LINE*)malloc(*size_tab * sizeof(PROJECT_LINE));

			lines[0].first_point = origin;
			lines[0].last_point = (PROJECT_COORD) { 800, 125 };

			lines[1].first_point = lines[0].last_point;
			lines[1].last_point = (PROJECT_COORD) { 905, 160 };

			lines[2].first_point = lines[1].last_point;
			lines[2].last_point = target;
		}
		else
		{
			*size_tab = 5;

			lines = (PROJECT_LINE*)malloc(*size_tab * sizeof(PROJECT_LINE));

			lines[0].first_point = origin;
			lines[0].last_point = (PROJECT_COORD) { 370, 125 };

			lines[1].first_point = lines[0].last_point;
			lines[1].last_point = (PROJECT_COORD) { 90, 315 };

			lines[2].first_point = lines[1].last_point;
			lines[2].last_point = (PROJECT_COORD) { 535, 575 };

			lines[3].first_point = lines[2].last_point;
			lines[3].last_point = (PROJECT_COORD) { 620, 575 };

			lines[4].first_point = lines[3].last_point;
			lines[4].last_point = target;
		}
	}
	else if (origin.x == 0 && origin.y == 315)
	{
		if (target.x == 990 && target.y == 105)
		{
			*size_tab = 5;

			lines = (PROJECT_LINE*)malloc(*size_tab * sizeof(PROJECT_LINE));

			lines[0].first_point = origin;
			lines[0].last_point = (PROJECT_COORD) { 90, 315 };

			lines[1].first_point = lines[0].last_point;
			lines[1].last_point = (PROJECT_COORD) { 370, 125 };

			lines[2].first_point = lines[1].last_point;
			lines[2].last_point = (PROJECT_COORD) { 800, 125 };

			lines[3].first_point = lines[2].last_point;
			lines[3].last_point = (PROJECT_COORD) { 915, 170 };

			lines[4].first_point = lines[3].last_point;
			lines[4].last_point = target;
		}
		else
		{
			*size_tab = 4;

			lines = (PROJECT_LINE*)malloc(*size_tab * sizeof(PROJECT_LINE));

			lines[0].first_point = origin;
			lines[0].last_point = (PROJECT_COORD) { 90, 315 };

			lines[1].first_point = lines[0].last_point;
			lines[1].last_point = (PROJECT_COORD) { 535, 575 };

			lines[2].first_point = lines[1].last_point;
			lines[2].last_point = (PROJECT_COORD) { 620, 575 };

			lines[3].first_point = lines[2].last_point;
			lines[3].last_point = target;
		}
	}
	else if (origin.x == 0 && origin.y == 575)
	{
		if (target.x == 990 && target.y == 105)
		{
			*size_tab = 4;

			lines = (PROJECT_LINE*)malloc(*size_tab * sizeof(PROJECT_LINE));

			lines[0].first_point = origin;
			lines[0].last_point = (PROJECT_COORD) { 800, 575 };

			lines[1].first_point = lines[0].last_point;
			lines[1].last_point = (PROJECT_COORD) { 1220, 355 };

			lines[2].first_point = lines[1].last_point;
			lines[2].last_point = (PROJECT_COORD) { 905, 160 };

			lines[3].first_point = lines[2].last_point;
			lines[3].last_point = target;
		}
		else
		{
			*size_tab = 2;

			lines = (PROJECT_LINE*)malloc(*size_tab * sizeof(PROJECT_LINE));

			lines[0].first_point = origin;
			lines[0].last_point = (PROJECT_COORD) { 620, 575 };

			lines[1].first_point = lines[0].last_point;
			lines[1].last_point = target;
		}
	}
	else if (origin.x == 1280 && origin.y == 355)
	{
		if (target.x == 990 && target.y == 105)
		{
			*size_tab = 3;

			lines = (PROJECT_LINE*)malloc(*size_tab * sizeof(PROJECT_LINE));

			lines[0].first_point = origin;
			lines[0].last_point = (PROJECT_COORD) { 1220, 355 };

			lines[1].first_point = lines[0].last_point;
			lines[1].last_point = (PROJECT_COORD) { 905, 160 };

			lines[2].first_point = lines[1].last_point;
			lines[2].last_point = target;
		}
		else
		{
			*size_tab = 4;

			lines = (PROJECT_LINE*)malloc(*size_tab * sizeof(PROJECT_LINE));

			lines[0].first_point = origin;
			lines[0].last_point = (PROJECT_COORD) { 1220, 355 };

			lines[1].first_point = lines[0].last_point;
			lines[1].last_point = (PROJECT_COORD) { 800, 575 };

			lines[2].first_point = lines[1].last_point;
			lines[2].last_point = (PROJECT_COORD) { 620, 575 };

			lines[3].first_point = lines[2].last_point;
			lines[3].last_point = target;
		}
	}

	return lines;
}



// Fonction qui retourne un tableau de pointeurs sur way, qui pourront être utilisés pour les ennemis
PROJECT_WAY** pr_create_all_ways(int *size_tab)
{
	int i, j, k = 0;
	int size;
	PROJECT_LINE *lines;
	PROJECT_WAY **way;
	
	*size_tab = 8;

	way = (PROJECT_WAY**)malloc(*size_tab * sizeof(PROJECT_WAY*));
	if (way == NULL)
	{
		pr_error("malloc()");
		return NULL;
	}

	for (i = FIRST_TARGET; i <= LAST_TARGET; i++)
	{
		for (j = FIRST_ORIGIN; j <= LAST_ORIGIN; j++)
		{
			lines = pr_init_way(i, j, &size);
			way[k++] = pr_create_way(lines, size);
			if (way[k] == NULL)
			{
				for (; k > *size_tab; k--)
					free(way[k]);
				free(way);
				return NULL;
			}
			free(lines);
		}
	}

	return way;
}