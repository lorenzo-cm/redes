all:
	gcc -Wall -c common.c
	gcc -Wall client.c common.o -o client
	gcc -Wall server.c common.o -o server


test:
	gcc -Wall -Iinclude src/maze.c src/main.c -o test


clean:
	rm -f client server common.o test
