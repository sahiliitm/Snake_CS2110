/*
 * Functions for the one player game- user vs the computer
 * Written by Mitali Bafna, CS12B044
 * 			  Sahil Sharma, CS12B060
 *			  Vidhya Ramaswamy, CS12B061
 */


#include "one_player.h"
#define MAXREW 4

/*
 * Checks if a position collides with either of the two snakes 
 * (for the computer to decide which positions it can move into)
 * Inputs: s2 - human snake
 *		   s1 - computer snake
 *		   auto_head - position being checked
 * Output: 1 - if it collides
 *		   0 - if the position is empty
 */
int auto_collide (Queue *s2, Queue *s1, coord auto_head)
{
	int x, y;

	//checking if it collides with the human snake
	Node * temp = s2->tail;
	while (temp!=NULL)
	{
		
		if ((temp->data.x==auto_head.x) && (temp->data.y==auto_head.y))
			return 1;
		temp = temp->next;	
	}
	
	//checking if it collides with the computer snake
	temp = s1->tail;
	while (temp!=NULL)
	{
		
		if ((temp->data.x==auto_head.x) && (temp->data.y==auto_head.y))
			return 1;
		temp = temp->next;	
	}
	return 0;
}	


/*
 * Returns manhattan distance between two coordinates
 * Inputs: auto_head - coordinate 1
 		   rew - coordinate 2
 * Output: distance between the coordinates
 */	
int auto_dist_to_reward(coord auto_head, coord rew)
{
	int distx=0, disty=0;
	distx+=(auto_head.x>rew.x)?(auto_head.x-rew.x):(rew.x-auto_head.x);
	if (distx>WIDTH/2)
		distx = WIDTH-distx;
	disty+=(auto_head.y>rew.y)?(auto_head.y-rew.y):(rew.y-auto_head.y);
	if (disty>HEIGHT/2)
		disty = HEIGHT-disty;
	return (distx+disty);
}


/*
 * Checks if a position is within the bombregion, when the bomb is active 
 * (for the computer to decide which positions it can move into)
 * Inputs: b - bomb
 		   br - list of coordinates affected by the bomb
 		   auto_head - position being checked
 * Outputs: 1 - if it is in the bomb
 			0 - otherwise
 */
int auto_check_bomb(bomb *b, bombregion *br, coord * new_head)
{
	int i;
	if ((b->isexploding==1)|| (b->isactive==1))
	{
		for (i=0; i<SIZEBOMBREGION; i++)
			if ((br->array[i].x==new_head->x) && (br->array[i].y==new_head->y))
				return 1;
	}
	return 0;
}	


int over[MAXREW];
int perm[MAXREW];

/*
 * Generates all possible permutations of visiting the rewards, and gets the shortest
 * path length
 * Inputs: new_head - position of the computer head
 *		   rew - set of all rewards
 *		   min_distance - minimum distance through all rewards
 *		   ctr - which element we are filling in perm[]
 * Output: nil 
 */
void generate_perm(coord *new_head, reward *rew, int *min_distance, int ctr)
{
	int i, j;
	if (ctr==rew->rewnum)
	{
		int dist = auto_dist_to_reward(*new_head, rew->rewcoord[perm[0]]);
		for (j=1; j<rew->rewnum; j++)
			dist+= auto_dist_to_reward(rew->rewcoord[perm[j-1]], rew->rewcoord[perm[j]]);
		if (*min_distance>dist)
			*min_distance=dist;
		return;
	}
	for (i=0; i<rew->rewnum; i++)
	{
		if (over[i]==0)
		{
			perm[ctr]=i;
			over[i]=1;
			generate_perm(new_head, rew, min_distance, ctr+1);
			over[i]=0;
		}
		
	}
}

/*
 * Computes best possible move for the computer
 * Inputs: s1 - computer snake
 		   s2 - human snake
 		   snake1dir - which direction the computer snake is moving in
 		   rew - set of all rewards
 		   b - bomb
 		   br - list of positions affected by the bomb
 		   p - points of snake1 and snake2
 		   TIME - how fast the game runs
 */
int auto_move(Queue *s1, Queue *s2, char *snake1dir, reward *rew, bomb *b, bombregion *br, Points* p, int TIME)
{
	coord auto_head = get_head(s1);
	coord new_head = auto_head;
	int max_dist = 4*(WIDTH+HEIGHT);

	// 0 is up, 1 is right, 2 is down, 3 is left for the points array
	int points[4]={max_dist, max_dist, max_dist, max_dist};
	switch(*snake1dir)
	{
		case 'u': points[2]=(max_dist+2);
				break;
		case 'r': points[3]=(max_dist+2);
				break;
		case 'd': points[0]=(max_dist+2);
				break;
		case 'l': points[1]=(max_dist+2);
				break;
	}
	int i;
	for (i=0; i<rew->rewnum; i++)
			{
				over[i]=0;
				perm[i]=-1;
			}
	//checking if it can move up
	if (points[0]!=(max_dist+2))
	{
		if (new_head.y==0)
			new_head.y = HEIGHT;
		else
			new_head.y--;
			
		if (auto_collide(s2,s1, new_head)==1)
			points[0]=(max_dist+2);
		else if (auto_check_bomb(b, br, &new_head)==1)
			points[0]=(max_dist-2);
		else
		{
			generate_perm(&new_head, rew, &points[0], 0);
			for (i=0; i<rew->rewnum; i++)
			{
				over[i]=0;
				perm[i]=-1;
			}
		}
	}
	new_head=auto_head;
	//checking if it can move right
	if (points[1]!=(max_dist+2))
	{
		if (new_head.x==WIDTH)
			new_head.x = 0;
		else
			new_head.x++;
		if (auto_collide(s2,s1, new_head)==1)
			points[1]=(max_dist+2);
		else if (auto_check_bomb(b, br, &new_head)==1)
			points[1]=max_dist-2;

		else
		{
			generate_perm(&new_head, rew, &points[1], 0);	
			for (i=0; i<rew->rewnum; i++)
			{
				over[i]=0;
				perm[i]=-1;
			}
		}
	}	
						
	new_head=auto_head;
	//checking if it can move down
	if (points[2]!=(max_dist+2))
	{
		if (new_head.y==HEIGHT)
			new_head.y = 0;
		else
			new_head.y++;
		if (auto_collide(s2,s1, new_head)==1)
			points[2]=(max_dist+2);
		else if (auto_check_bomb(b, br, &new_head)==1)
			points[2]=(max_dist-2);

		else
		{
			generate_perm(&new_head, rew, &points[2], 0);
			for (i=0; i<rew->rewnum; i++)
			{
				over[i]=0;
				perm[i]=-1;
			}
		}
	}					
	new_head=auto_head;
	//checking if it can move left
	if (points[3]!=(max_dist+2))
	{
		if (new_head.x==0)
			new_head.x = WIDTH;
		else
			new_head.x--;
		if (auto_collide(s2, s1, new_head)==1)
			points[3]=(max_dist+2);
		else if (auto_check_bomb(b, br, &new_head)==1)
			points[3]=max_dist-2;
		
		else
		{
			generate_perm(&new_head, rew, &points[3], 0);
			for (i=0; i<rew->rewnum; i++)
			{
				over[i]=0;
				perm[i]=-1;
			}		
		}
	}	
	int min = points[0];
	int pos=0;
	//getting best direction
	for (i=1; i<4; i++)
	{
		if (min>points[i])
		{
			min=points[i];
			pos=i;
		}
	}
	new_head=auto_head;
	timeout(TIME);
	//setting new position of head
	switch(pos)
	{
		case 0:	if (new_head.y==0)
					new_head.y = HEIGHT;
				else
					new_head.y--;
				*snake1dir='u';
				break;
		case 1: if (new_head.x==WIDTH)
					new_head.x = 0;
				else
					new_head.x++;
				*snake1dir='r';
				break;
		case 2: if (new_head.y==HEIGHT)
					new_head.y = 0;
				else
					new_head.y++;
				*snake1dir='d';
				break;
		case 3: if (new_head.x==0)
					new_head.x = WIDTH;
				else
					new_head.x--;
				*snake1dir='l';
				break;
	}
	s1 = queue_push(s1, new_head);
	int flag=0;
	//checking if any rewards were eaten
		for (i=0; i<rew->rewnum; i++)
		{
			if ((rew->rewcoord[i].x==new_head.x)&&(rew->rewcoord[i].y==new_head.y))
			{
				rew = mk_reward(s1, s2, rew, i);
				p->s1_points++;
				flag=1;
				break;
			}
		}
		if (flag==0)
			s1 = queue_pop(s1);
	usleep(TIME*1000);
	return 0;		
	
}

/*
 * Allows user to move
 * Inputs: s1 - computer snake
 		   s2 - human snake
 		   snake2 - current head of human snake
 		   snake2dir - which direction the human snake is moving in
 		   r - set of all rewards
 		   p - points of snake1 and snake2
 * Output: 1 - if user wants to quit 
 		   0 - otherwise
 */
int play_one(Queue *s1, Queue *s2, coord *snake2, char *snake2dir, reward *r,Points* p)
{
		int ch;
		int set_dir2=0;
		ch=getch();						
		switch (ch)
		{
				//quitting game
				case (int) 'q': return 1;
				
				//moves up
				case 259: if ((*snake2dir=='r')||(*snake2dir=='l'))
							{
								if (snake2->y==0)
									snake2->y = HEIGHT;
								else
									snake2->y--;
								*snake2dir = 'u';
								set_dir2=1;
							}
							break;
							
				//moves down
				case 258 : if ((*snake2dir=='r')||(*snake2dir=='l'))
							{
								if (snake2->y==HEIGHT)
									snake2->y = 0;
								else
									snake2->y++;
								set_dir2=1;
								*snake2dir = 'd';
							}
							break;
				
				//moves left
				case 260 : if ((*snake2dir=='u')||(*snake2dir=='d'))
							{
								if (snake2->x==0)
									snake2->x = WIDTH;
								else
									snake2->x--;
								set_dir2=1;
								*snake2dir = 'l';
							}
							break;		
							
				//moves right
				case 261: if ((*snake2dir=='u')||(*snake2dir=='d'))
							{
								if (snake2->x==WIDTH)
									snake2->x = 0;
								else
									snake2->x++;
								set_dir2=1;
								*snake2dir = 'r';
							}
							break;
			}
			
		//if user doesn't input, move in the same direction
		if (set_dir2==0)
		{
			switch(*snake2dir)
			{
				case 'l': if (snake2->x==0)
							snake2->x = WIDTH;
						else
							snake2->x--;
						break;
				case 'r': if (snake2->x==WIDTH)
							snake2->x = 0;
						else
							snake2->x++;
						break;
				case 'u': if (snake2->y==0)
							snake2->y = HEIGHT;
						else
							snake2->y--;
						break;
				case 'd': if (snake2->y==HEIGHT)
							snake2->y = 0;
						else
							snake2->y++;
						break;
			}
		}
		
		s2 = queue_push(s2, *snake2);
		int flag=0, i;
		for (i=0; i<r->rewnum; i++)
		{
			if ((r->rewcoord[i].x==snake2->x)&&(r->rewcoord[i].y==snake2->y))
			{
				r = mk_reward(s1, s2, r, i);
				p->s2_points++;
				flag=1;
				break;
			}
		}
		if (flag==0)
			s2 = queue_pop(s2);
		
	return 0;
}


