#include "Queue.h"

#ifndef COMMON_
#define COMMON_

#define MAXDURATION 10
#define WIDTH 79
#define HEIGHT 21
#define SIZEBOMBREGION 53

typedef struct bomb_
{
	int isactive;
	int isexploding;
	int duration;
	coord topleft;
}bomb;


typedef struct bombregion_
{
	coord array[SIZEBOMBREGION];
}bombregion;

typedef struct Points_ {
     int s1_points; 
    int s2_points; 
} Points;

coord make_coord(int x,int y);

void init_bombregion(bombregion * br,bomb b);

coord mk_reward(Queue *s1, Queue *s2);
Points* mk_points();

void get_points(Queue* s1,Queue* s2,Points* p);

int coord_in_bomb(coord c, bombregion br);
void printscreen(Queue *s1, Queue *s2, coord *rew,bomb *b, int * call_no, bombregion * br, Points* p,double x,double y, int z);
void end_game(int col ,Queue *s1 , Queue *s2, Points* p);
int modify_snake(Queue* s1,bomb *b,bombregion br);
int explode(Queue *s1, Queue *s2, bomb *b, int *call_no,bombregion br, Points* p);
int collide (Queue *s1, Queue *s2, char dir1, char dir2);
int correct_value(int x, int dimention );

#endif
