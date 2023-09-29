#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define PADDING_PX 8

typedef struct Button {
	void (*function)(void);

	int w, h;
	int textW, textH;
	SDL_Texture *tex;
} Button;

Button createButton(SDL_Renderer*, char*, int, int, int, void (*function));

typedef struct Date {
	int day, month, year;
} Date;

typedef struct Task {
	Button tickBoxButton, editButton;
	Date completionDate;
	char *taskText;
	bool complete;
	SDL_Texture *textTex, *dateTex;
	int textW, textH, dateW, dateH;
} Task;

Task createTask(SDL_Renderer*, char*, int, int, int);

// Linked List functions
typedef struct Node {
	Task data;
	struct Node *next;
} Node;

void listInsert(Node*, int, Task);
void listPush(Node*, Task);
Task* listGetFirst(Node**);
Task* listPop(Node*);
Task* listRemove(Node**, int);

void drawButtonBackground(SDL_Renderer *r, SDL_Rect *buttonRect);
void drawTaskList(SDL_Renderer*, Node*, int, int, int, int, int);
