EXEC_NAME = todo
MAIN_FILE = main.c
HEADERS = widget.c

all : build

build: $(MAIN_FILE) $(HEADERS)
	gcc $(MAIN_FILE) $(HEADERS) \
	-Wfatal-errors \
	-std=c99 \
	-lSDL2main \
	-lSDL2 \
	-lSDL2_image \
	-lSDL2_ttf \
	-lSDL2_mixer \
	-lm \
	-o $(EXEC_NAME)

run: build
	./$(EXEC_NAME)

clean:
	rm $(EXEC_NAME)