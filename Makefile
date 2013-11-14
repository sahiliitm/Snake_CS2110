#Makefile 
#

Snake  : main.o queue.o snake.o one_player.o two_player.o
	gcc  $^ -o $@ -lncurses


main.o: main.c Queue.h Snake.h one_player.h two_player.h
	gcc -c -g  $< -o  $@ -lncurses


queue.o: Queue.c Queue.h Snake.h
	gcc -c -g $< -o  $@ -lncurses

snake.o: Snake.c Queue.h Snake.h
	gcc -c -g  $< -o  $@ -lncurses
	
one_player.o: one_player.c Snake.h
	gcc -c -g  $< -o  $@ -lncurses
	
two_player.o: two_player.c Snake.h
	gcc -c -g  $< -o  $@ -lncurses
	
.PHONY: clean


clean:
	rm -rf *.o
	rm -rf snake.o queue.o main.o Snake


