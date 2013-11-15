#include "two_player.h"

/*
*Purpose: Moves the 2 snakes according to the user input
* Inputs: s1,s2: Pointers to the queue containing the snake coordinates
*         snake1, snake2: Coordinates of heads of the snakes
*         snake1dir, snake2dir: Initial direction of movement
*         r: Coordinates of reward
*         p: Current points of the players
* Outputs: 1: if user quits game
		   0: otherwise	
*/
int play(Queue *s1, Queue *s2, coord* snake1, coord *snake2, char *snake1dir, char *snake2dir, reward *r,Points* p )
{
		int ch;
		int set_dir1=0;
		int set_dir2=0;
		int iter;
	for (iter=0; iter<2; iter++)
	{
		ch=getch();						
		
		//Get the new head of the snake:
		switch (ch)
		{
				case (int)'q': return 1;
				
				case (int)'w': if (((*snake1dir=='r')||(*snake1dir=='l'))&&(set_dir1==0))
							{
								if (snake1->y==0)
									snake1->y = HEIGHT;
								else
									snake1->y--;
								*snake1dir = 'u';
								set_dir1=1;
							}
							break;
				
				case (int)'s':  if (((*snake1dir=='r')||(*snake1dir=='l'))&&(set_dir1==0))
							{
								if (snake1->y==HEIGHT)
									snake1->y = 0;
								else
									snake1->y++;
								set_dir1=1;
								*snake1dir = 'd';
							}
							break;
								
				case (int)'a': if (((*snake1dir=='u')||(*snake1dir=='d'))&&(set_dir1==0))
							{
								if (snake1->x==0)
									snake1->x = WIDTH;
								else
									snake1->x--;
								set_dir1=1;
								*snake1dir = 'l';
							}
							break;
				
				case (int)'d': if (((*snake1dir=='u')||(*snake1dir=='d'))&&(set_dir1==0))
							{
								if (snake1->x==WIDTH)
									snake1->x = 0;
								else
									snake1->x++;
								set_dir1=1;
								*snake1dir = 'r';
							}
							break;
				case 259: if (((*snake2dir=='r')||(*snake2dir=='l'))&&(set_dir2==0))
							{
								if (snake2->y==0)
									snake2->y = HEIGHT;
								else
									snake2->y--;
								*snake2dir = 'u';
								set_dir2=1;
							}
							break;
				case 258 : if (((*snake2dir=='r')||(*snake2dir=='l'))&&(set_dir2==0))
							{
								if (snake2->y==HEIGHT)
									snake2->y = 0;
								else
									snake2->y++;
								set_dir2=1;
								*snake2dir = 'd';
							}
							break;
				case 260 : if (((*snake2dir=='u')||(*snake2dir=='d'))&&(set_dir2==0))
							{
								if (snake2->x==0)
									snake2->x = WIDTH;
								else
									snake2->x--;
								set_dir2=1;
								*snake2dir = 'l';
							}
							break;		
							
				case 261: if (((*snake2dir=='u')||(*snake2dir=='d'))&&(set_dir2==0))
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
		}
		if (set_dir1==0)
		{
			switch(*snake1dir)
			{
				case 'l': if (snake1->x==0)
							snake1->x = WIDTH;
						else
							snake1->x--;
						break;
				case 'r': if (snake1->x==WIDTH)
							snake1->x = 0;
						else
							snake1->x++;
						break;
				case 'u': if (snake1->y==0)
							snake1->y = HEIGHT;
						else
							snake1->y--;
						break;
				case 'd': if (snake1->y==HEIGHT)
							snake1->y = 0;
						else
							snake1->y++;
						break;
			}
		}
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
	
		//Push the new head onto the queue:
		s1 = queue_push(s1, *snake1);
		s2 = queue_push(s2, *snake2);
		int flag=0, i, check=-1;
	
		//Check if the snakes have eaten the reward:
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
		//If snake2 has not eaten reward, then pop out its tail from queue:
		if (flag==0)
			s2 = queue_pop(s2);
		flag=0;
		coord s1_head = get_head(s1);
		for (i=0; i<r->rewnum; i++)
		{
			if ((r->rewcoord[i].x==snake1->x)&&(r->rewcoord[i].y==snake1->y))
			{
				r = mk_reward(s1, s2, r, i);
				p->s1_points++;
				flag=1;
				break;
			}
		}
		
		//If snake1 has not eaten reward, then pop out its tail from queue:
		if (flag==0)
			s1 = queue_pop(s1);
				
	return 0;
}
