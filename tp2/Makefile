all:
	mkdir -p obj
	gcc -Wall -Iinclude -c src/common.c -o obj/common.o

	gcc -Wall -lpthread -Iinclude -c src/server_core.c -o obj/server_core.o
	gcc -Wall -lpthread -Iinclude -c src/client_core.c -o obj/client_core.o
	gcc -Wall -lpthread -Iinclude -c src/neighbors.c -o obj/neighbors.o
	
	mkdir -p bin

	gcc -Wall -lpthread -Iinclude src/client.c obj/common.o obj/client_core.o obj/neighbors.o -o bin/client -lm
	gcc -Wall -lpthread -Iinclude src/server-mt.c obj/common.o obj/server_core.o -o bin/server

clean:
	rm -rf bin obj
