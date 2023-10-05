#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define PADDING_PX 8

typedef struct Button {
	int w, h;
	int textW, textH;
	SDL_Texture *tex;
} Button;

Button createButton(SDL_Renderer *r, char *text, int w, int h, int fontSize);
bool isButtonClicked(SDL_Rect *buttonRect, int mouseX, int mouseY);

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
	SDL_Rect doneRect, editRect;
} Task;

Task createTask(SDL_Renderer *r, char *taskText, int day, int month, int year);

// Linked List functions
typedef struct Node {
	Task data;
	struct Node *next;
} Node;

void listInsert(Node *head, int pos, Task val);
void listPush(Node *head, Task val, bool init);
Task* listGetFirst(Node **head);
Task* listPop(Node *head);
Task* listRemove(Node **head, int pos);

void drawButtonBackground(SDL_Renderer *r, SDL_Rect *buttonRect);
void drawTaskList(SDL_Renderer *r, Node *head, int i, int listX, int listY, int taskWidth, int taskHeight);
