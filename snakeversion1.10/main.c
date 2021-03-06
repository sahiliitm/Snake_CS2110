
#include<string.h>
#include<stdlib.h>
#include "one_player.h"
#include "two_player.h"
#define DBUG(x)			mvprintw(20, 10, "%d%d%d",x,x,x);refresh();sleep(5);
#define MAXLEN 70
#define MAXTIME 15
void main()
{
	initscr();
	clear();
	
	cbreak();	/* Line buffering disabled. pass on everything */	
	refresh();
	
	start_color();
	init_pair(1, COLOR_YELLOW, COLOR_BLUE);
	init_pair(2, COLOR_BLACK , COLOR_RED);	
	init_pair(3, COLOR_WHITE , COLOR_YELLOW);
	init_pair(4, COLOR_WHITE , COLOR_BLUE);
	init_pair(5, COLOR_RED, COLOR_WHITE);
	init_pair(6, COLOR_BLACK , COLOR_BLACK);	
	init_pair(9,COLOR_YELLOW,COLOR_WHITE);
	init_pair(7, COLOR_WHITE , COLOR_RED);
	init_pair(10, COLOR_WHITE , COLOR_WHITE);
	init_pair(11, COLOR_BLACK , COLOR_WHITE);
	init_pair(12, COLOR_RED , COLOR_WHITE);
	init_pair(13, COLOR_BLUE , COLOR_WHITE);
	init_pair(14, COLOR_MAGENTA , COLOR_WHITE);
	
	//Menu:
	int len1=5, len2=1;
	long long int runno=0;	
	struct timeval tim;  
	struct timeval ini;
	int TIME;
	mvprintw(HEIGHT/2,WIDTH/2-20,"Press 'o' for one-player and 't' for two-player:");
	refresh();
	char c;
	
	//Choice of two/one player
	do
	{
		c=getch();	
		if (!((c=='o')||(c=='t'))) 
		{
			clear();
			mvprintw(HEIGHT/2,WIDTH/2-20,"Enter o or t only " );
			refresh();
		}
	}while	(!(  (c=='o') || (c=='t')  ) );
	
	
	
	//choice of level ( of difficulty )
	
	clear();
	mvprintw(HEIGHT/2,WIDTH/2-20,"Enter the level (1-3): " );
	refresh();
	int lev;
	do
	{
		lev = getch();
		if ((lev>'3')||(lev<='0'))
		{
			clear();
			mvprintw(HEIGHT/2,WIDTH/2-20,"Enter 1-3 only " );
			refresh();
		}
	}while	((lev>'3')||(lev<='0'));
	
	switch (lev)
	{
		case '1': TIME = 50;
				break;
		case '2': TIME = 40;
				break;
		case '3': TIME = 30;
				break;
	}
	
	//Length of the Snake:
	if(c=='t')
	{
		clear();
		mvprintw(HEIGHT/2,WIDTH/2-20,"Enter the length for Snake1:" );
		refresh();
		//char s[3];
		char s[4];
		int i;
		do
		{
			for (i=0; i<4; i++)
			{
				s[i]=getch();
				if (((s[i]==' ')||(s[i]=='\n'))&&(i!=0))
				{
					s[i]='\0';
					break;
				}
			}
			len1=(int)atoi(s);

			if(len1>MAXLEN){
				clear();
				mvprintw(HEIGHT/2,WIDTH/2-20,"Enter a length smaller than %d :",MAXLEN );
				refresh();
			}
				
	}while(len1>MAXLEN);

		clear();
		mvprintw(HEIGHT/2,WIDTH/2-20,"Enter the length for Snake2:" );
		refresh();
		do
		{
			for (i=0; i<4; i++)
			{
				s[i]=getch();
				if (((s[i]==' ')||(s[i]=='\n'))&&(i!=0))
				{
					s[i]='\0';
					break;
				}
			}
			len2=(int)atoi(s);

			if(len2>MAXLEN){
				clear();
				mvprintw(HEIGHT/2,WIDTH/2-20,"Enter a length smaller than %d :",MAXLEN );
				refresh();
			}
				
	}while(len2>MAXLEN);
		
	}
	//One Player
	else{
		clear();
		mvprintw(HEIGHT/2,WIDTH/2-20,"Enter the length for the Snake:" );
		refresh();
		char s[4];
		int i;
		do
		{
			for (i=0; i<4; i++)
			{
				s[i]=getch();
				if (((s[i]==' ')||(s[i]=='\n'))&&(i!=0))
				{
					s[i]='\0';
					break;
				}
			}
			len2=(int)atoi(s);

			if(len2>MAXLEN){
				clear();
				mvprintw(HEIGHT/2,WIDTH/2-20,"Enter a length smaller than %d :",MAXLEN );
				refresh();
			}
				
	}while(len2>MAXLEN);
	}
	
	//Getting the duration of match:
	int maxtime=0;
	char temp[3];
	int i;
	clear();
		mvprintw(HEIGHT/2,WIDTH/2-20,"Enter the Duration of the match (in minutes):" );
		refresh();
	do
		{
			for (i=0; i<3; i++)
			{
				temp[i]=getch();
				if (((temp[i]==' ')||(temp[i]=='\n'))&&(i!=0))
				{
					temp[i]='\0';
					break;
				}
			}
			maxtime=(int)atoi(temp);

			if(maxtime>MAXTIME){
				clear();
				mvprintw(HEIGHT/2,WIDTH/2-20,"Length of match should be lesser  than %d :",MAXTIME );
				refresh();
			}
				
	}while(maxtime>MAXTIME);
	
	
	//Inputting number of rewards
	
	reward r;
	mvprintw(HEIGHT/2,WIDTH/2-30,"Enter the number of rewards which should be on the screen at a time:");
	refresh();
	// Choice of Number of rewards
	do
	{
		r.rewnum = getch() - '0';
		if ((r.rewnum > 4)||(r.rewnum<= 0 ))
		{
			clear();
			mvprintw(HEIGHT/2,WIDTH/2-20,"Enter 1-4 only " );
			refresh();
		}
	}while	((r.rewnum>4)||r.rewnum<=0);
	
	int iq;
	for( iq=0 ; iq< 4 ; iq++ )
	r.rewcoord[iq].x = r.rewcoord[iq].y = -1 ;
	
	
	//Initializing structures and variables
	timeout(TIME);
		
	noecho();
	coord snake1 = make_coord(0, 0);
	coord snake2 = make_coord(WIDTH, HEIGHT);
	Queue *s1 = queue_new();
	s1 = queue_push(s1, snake1);
	
	for (i=1; i<len1; i++)
	{
		snake1 = make_coord(i, 0);
		s1 = queue_push(s1, snake1);
	}
	Queue *s2 = queue_new();
	s2 = queue_push(s2, snake2);
	for (i=1; i<len2; i++)
	{
		snake2 = make_coord(WIDTH-i, HEIGHT);
		s2 = queue_push(s2, snake2);
	}
	
	keypad(stdscr, TRUE);
	refresh();
	int ch;
	char snake1dir,snake2dir;
	snake1dir='r';
	snake2dir='l';
	int set_dir1=0 , set_dir2=0;
	srand(time(NULL));
	 
	bomb b={1,0,0,{10,10}};
	int bombx,bomby;
	Points* p=mk_points();	
	get_points(s1,s2,p);
	int call_no=0;
	bombregion br;	
	init_bombregion(&br, b);
	printscreen(s1, s2, &r ,&b,&call_no,&br,p,0,0,0);
	
	int nextexplode=( rand() % 20 ) * 10 +runno+10;
	double t2=1000,t1=0;
	double ini_time;
	gettimeofday(&ini, NULL);
	ini_time = ini.tv_sec*1000 + ( ini.tv_usec / 1000.0 );  
	init_reward (s1, s2, &r );
	
	//Main loop:
	do 
	{
		gettimeofday(&tim, NULL);
		
		//Exploding/Printing of bomb if it is active 
		runno++;
		if(b.isactive)
		{
			if(b.duration == MAXDURATION )
			{
				b.isactive=0;
				b.isexploding=1;
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
			
			        	
		t1 = tim.tv_sec*1000 + ( tim.tv_usec / 1000.0 );  
		
		//If time of the match is over:
		timeout(TIME); 		
		if(  (t1-ini_time)/60000 > (double)maxtime )
			end_game(3,s1, s2,p);
		
		//Move the snakes, by taking user input:	
		if(c=='o')
			ch = play_one(s1, s2, &snake2, &snake2dir, &r,p); 		
		else
			ch =play(s1, s2, &snake1, &snake2, &snake1dir, &snake2dir, &r,p);
		
		if (ch==1)
			break;
		if ( c=='o')
			auto_move(s1, s2, &snake1dir, &r, &b,&br,p, TIME);
		
		//Check if the 2 snakes collided:
		int col = collide(s1, s2, snake1dir, snake2dir);
	
		end_game(col,s1,s2,p);	
		gettimeofday(&tim, NULL);  
   		double t2 = tim.tv_sec*1000.0 + ( tim.tv_usec/1000.0  );		
		
		while(   t2-t1 < 2*TIME )
		{
			timeout(0);
			getch();
			getch();
			getch();
			gettimeofday(&tim, NULL);  
    			t2 = tim.tv_sec*1000.0 + ( tim.tv_usec/1000.0  );		
					
		}		
			
		printscreen(s1, s2, &r,&b, &call_no, &br, p,t1,ini_time,maxtime);	
		refresh();
	}	
	while (ch!='q');
	clrtoeol();
	refresh();
	endwin();
}

 
