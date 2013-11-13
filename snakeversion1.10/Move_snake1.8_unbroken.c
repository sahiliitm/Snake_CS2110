#include<stdio.h>
#include "Snake.h"
#include "Queue.h"
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include<string.h>
#define MAXDURATION 10
#define WIDTH 79
#define HEIGHT 23
#define DBUG(x)			mvprintw(20, 10, "%d%d%d",x,x,x);refresh();sleep(5);


//Structures to be used

//CREATING/INITIALIZING NEW STRUCTURES


// Create a new empty queue



void main()
{
	initscr();
	clear();
	noecho();
	cbreak();	/* Line buffering disabled. pass on everything */
	//raw();	
	refresh();
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_GREEN);
	init_pair(2, COLOR_BLACK , COLOR_RED);	
	init_pair(3, COLOR_WHITE , COLOR_YELLOW);
	init_pair(4, COLOR_WHITE , COLOR_BLUE);
	init_pair(5, COLOR_RED, COLOR_MAGENTA);
	init_pair(6, COLOR_GREEN , COLOR_BLACK);	
	init_pair(7, COLOR_WHITE , COLOR_RED);
	
	long long int runno=0;	
	struct timeval tim;  
	timeout(100);
	int snake1y = 0 , snake2y = 0;	// The number of rows the snake head is below the top left
	int snake1x = 0 , snake2x = WIDTH;			
	Queue *s1 = queue_new();
	s1 = queue_push(s1, make_coord(snake1x, snake1y));
	Queue *s2 = queue_new();
	s2 = queue_push(s2, make_coord(snake2x, snake2y));
	keypad(stdscr, TRUE);
	refresh();
	int ch;
	char snake1dir,snake2dir;
	snake1dir='r';
	snake2dir='l';
	int set_dir1=0 , set_dir2=0;
	srand(time(NULL));
	coord reward = mk_reward(s1, s2); 
	bomb b={1,0,{10,10}};
	int bombx,bomby;	
	//attron(A_BOLD | A_STANDOUT);	
	printscreen(s1, s2, &reward,&b);
	bombregion br;	
	init_bombregion(&br, b);
	int nextexplode=( rand() % 20 ) * 10 +runno+10;
	int call_no=0;	
	double t2=1000,t1=0;
	do 
	{
		set_dir1=0;
		set_dir2=0;
		gettimeofday(&tim, NULL);
		t1 = tim.tv_sec*1000 + ( tim.tv_usec / 1000.0 );  
		timeout(50); 		
						
		runno++;
		if(b.isactive)
		{
			if(b.duration == MAXDURATION )
			{
				explode(s1,s2, &b, &call_no, br);
				
				//	b.isactive=0;
			//	b.duration=0;

				nextexplode = ( rand() % 20 )  +runno + 20 ;
			}			
			else 
				b.duration ++;
		}		
		else if ( runno == nextexplode )
		{
			b.isactive=  1 ;
			b.duration= 0 ;
		
			b.topleft.x = (rand()%WIDTH) ;
			b.topleft.y= (rand()%HEIGHT)  ;
		
		init_bombregion(&br,b);
		}
		int iter;
		for(iter=1;iter<=2;iter++)		
		{
			ch=getch();						
				switch (ch)
				{
				
						
					case (int)'w': if ((snake1dir=='r')||(snake1dir=='l'))
							{
								if (snake1y==0)
									snake1y = HEIGHT;
								else
									snake1y--;
								snake1dir = 'u';
								set_dir1=1;
							}
							break;
				
					case (int)'s':  if ((snake1dir=='r')||(snake1dir=='l'))
							{
								if (snake1y==HEIGHT)
									snake1y = 0;
								else
									snake1y++;
								set_dir1=1;
								snake1dir = 'd';
							}
							break;
								
					case (int)'a': if ((snake1dir=='u')||(snake1dir=='d'))
							{
								if (snake1x==0)
									snake1x = WIDTH;
								else
									snake1x--;
								set_dir1=1;
								snake1dir = 'l';
							}
							break;
				
					case (int)'d': if ((snake1dir=='u')||(snake1dir=='d'))
							{
								if (snake1x==WIDTH)
									snake1x = 0;
								else
									snake1x++;
								set_dir1=1;
								snake1dir = 'r';
							}
							break;
					case 259: if ((snake2dir=='r')||(snake2dir=='l'))
							{
								if (snake2y==0)
									snake2y = HEIGHT;
								else
									snake2y--;
								snake2dir = 'u';
								set_dir2=1;
							}
							break;
					case 258 : if ((snake2dir=='r')||(snake2dir=='l'))
							{
								if (snake2y==HEIGHT)
									snake2y = 0;
								else
									snake2y++;
								set_dir2=1;
								snake2dir = 'd';
							}
							break;
					case 260 : if ((snake2dir=='u')||(snake2dir=='d'))
							{
								if (snake2x==0)
									snake2x = WIDTH;
								else
									snake2x--;
								set_dir2=1;
								snake2dir = 'l';
							}
							break;		
							
					case 261: if ((snake2dir=='u')||(snake2dir=='d'))
							{
								if (snake2x==WIDTH)
									snake2x = 0;
								else
									snake2x++;
								set_dir2=1;
								snake2dir = 'r';
							}
							break;
				}
		}
		if (set_dir1==0)
		{
			switch(snake1dir)
			{
				case 'l': if (snake1x==0)
							snake1x = WIDTH;
						else
							snake1x--;
						break;
				case 'r': if (snake1x==WIDTH)
							snake1x = 0;
						else
							snake1x++;
						break;
				case 'u': if (snake1y==0)
							snake1y = HEIGHT;
						else
							snake1y--;
						break;
				case 'd': if (snake1y==HEIGHT)
							snake1y = 0;
						else
							snake1y++;
						break;
			}
		}
		if (set_dir2==0)
		{
			switch(snake2dir)
			{
				case 'l': if (snake2x==0)
							snake2x = WIDTH;
						else
							snake2x--;
						break;
				case 'r': if (snake2x==WIDTH)
							snake2x = 0;
						else
							snake2x++;
						break;
				case 'u': if (snake2y==0)
							snake2y = HEIGHT;
						else
							snake2y--;
						break;
				case 'd': if (snake2y==HEIGHT)
							snake2y = 0;
						else
							snake2y++;
						break;
			}
		}
		
		s1 = queue_push(s1, make_coord(snake1x, snake1y));
		s2 = queue_push(s2, make_coord(snake2x, snake2y));
		if ((reward.x!=snake1x)||(reward.y!=snake1y))
			s1 = queue_pop(s1);
		else 
			reward = mk_reward(s1, s2);
		
		if ((reward.x!=snake2x)||(reward.y!=snake2y))
			s2 = queue_pop(s2);
		else 
			reward = mk_reward(s1, s2);
		int col = collide(s1, s2, snake1dir, snake2dir);
		end_game(col,s1,s2);



		gettimeofday(&tim, NULL);  
    		double t2 = tim.tv_sec*1000.0 + ( tim.tv_usec/1000.0  );		
		timeout(0);
		while(   t2-t1 < 100  )
		{
			getch();
			getch();
			getch();				
			gettimeofday(&tim, NULL);  
    			t2 = tim.tv_sec*1000.0 + ( tim.tv_usec/1000.0  );		
					
		}		
			
		
		printscreen(s1, s2, &reward,&b);	
		refresh();
	}	
	while (ch!='q');
	clrtoeol();
	refresh();
	endwin();
} 
