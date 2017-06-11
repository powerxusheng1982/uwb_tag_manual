#ifndef _POSICAL_H_
#define _POSICAL_H_

typedef struct vec3d	vec3d;
extern int Range_deca[4];

struct vec3d {
	double	x;
	double	y;
	double	z;
};

extern vec3d report;
int Get_Best_Position(void);
#endif

