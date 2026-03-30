SRCS=src/main.c src/intMap.c
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
