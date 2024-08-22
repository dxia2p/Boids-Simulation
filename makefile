CC = gcc

SDL2_CFLAGS = -I src/include
SDL2_LDFLAGS = -L src/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, output/%.o, $(SRC))

main.exe: $(OBJ)
	$(CC) $(OBJ) $(SDL2_LDFLAGS) -o main.exe

output/%.o: %.c
	$(CC) $(SDL2_CFLAGS) -c $< -o $@ 

clean:
	rm -f output/*.o main.exe