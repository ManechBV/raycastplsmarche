SRCS=src/main.c src/intMap.c src/utils.c src/player.c src/raycaster.c
RAYLIB= -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
INCLUDE= -Iinclude
NAME=raycaster_by_namek

all:
	gcc $(SRCS) $(INCLUDE) $(RAYLIB) -o $(NAME)

re:
	clean
	all

clean:
	rm $(NAME)
