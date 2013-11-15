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

typedef struct reward_
{
    coord rewcoord[4];
    int rewnum;
} reward;

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

reward * mk_reward(Queue *s1, Queue *s2, reward *r, int x);

Points* mk_points();

void get_points(Queue* s1,Queue* s2,Points* p);

int coord_in_bomb(coord c, bombregion br);

void printscreen(Queue *s1, Queue *s2,reward *r,bomb *b, int * call_no, bombregion * br, Points* p,double x,double y, int z);

void end_game(int col ,Queue *s1 , Queue *s2, Points* p);

int modify_snake(Queue* s1,bomb *b,bombregion br);

int explode(Queue *s1, Queue *s2, bomb *b, int *call_no,bombregion br, Points* p);

int collide (Queue *s1, Queue *s2, char dir1, char dir2);

int correct_value(int x, int dimention );

int coord_near(coord x1, coord x2);

int is_coord_near_reward ( reward * r , int i );

void init_reward ( Queue * s1 , Queue * s2 , reward * r );

#endif

