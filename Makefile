SRCS=src/main.c src/intMap.c src/utils.c src/player.c src/raycaster.c
RAYLIB= lib/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11
INCLUDE= -Iinclude
NAME=raycaster_by_namek

all:
	gcc $(SRCS) $(RAYLIB) $(INCLUDE) -o $(NAME)

objs:
	mkdir -p objs

re:
	clean
	all

clean:
	rm $(NAME)
