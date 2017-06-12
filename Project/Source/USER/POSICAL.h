#ifndef _POSICAL_H_
#define _POSICAL_H_

//typedef struct vec3d	vec3d;



typedef struct{
	double	x;
	double	y;
	double	z;
} vec3d;

extern int Range_deca[4];
extern vec3d report;
extern vec3d m_anchorArray[4];

void Reference_Position_Init(void);
void Reference_Position_Set(int index, vec3d anchor);
vec3d Reference_Position_Get(int index);

int Get_Best_Position(void);
#endif

