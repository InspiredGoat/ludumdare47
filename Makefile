SOURCES = main.c batch2d.c
SRC = $(addprefix src/, $(SOURCES))
OBJ = $(addsuffix .o, $(addprefix bin/, $(basename $(notdir $(SRC)))));
INCLUDE = #-I include
CFLAGS = -W -O2# -D_DEBUG_

all: renderer

again: clean renderer

renderer: $(OBJ)
	gcc -W $^ -lGL -lglfw -lX11 -lpthread -lXrandr -lXi -ldl -o $@

bin/%.o : src/%.c
	gcc $(INCLUDE) $(CFLAGS) -c $< -o $@

clean:
	rm bin/*.o renderer

install:
	echo "Can't install surry"

try: renderer
	./renderer

run:
	./renderer
