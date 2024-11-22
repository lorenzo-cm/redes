all:
	mkdir obj
	gcc -Wall -Iinclude -c src/maze.c -o obj/maze.o
	gcc -Wall -Iinclude -c src/common.c -o obj/common.o
	gcc -Wall -Iinclude -c src/common_maze.c -o obj/common_maze.o

	gcc -Wall -Iinclude -c src/server_maze.c -o obj/server_maze.o
	gcc -Wall -Iinclude -c src/client_maze.c -o obj/client_maze.o
	
	mkdir bin

	gcc -Wall -Iinclude src/client.c obj/common.o obj/common_maze.o obj/client_maze.o obj/maze.o -o bin/client
	gcc -Wall -Iinclude src/server.c obj/common.o obj/common_maze.o obj/server_maze.o obj/maze.o -o bin/server

test:
	gcc -Wall -Iinclude src/maze.c src/main.c -o test


clean:
	rm -rf bin obj
