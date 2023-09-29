#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "widget.h"

#define SCREENWIDTH 1000
#define TASK_LIST_Y 80

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
		return 1;
	}

    SDL_Window *window = SDL_CreateWindow("TODO",
										  SDL_WINDOWPOS_CENTERED,
										  SDL_WINDOWPOS_CENTERED,
										  SCREENWIDTH, SCREENWIDTH, 0);
	Uint32 renderFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, renderFlags);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	Task firstTask = createTask(renderer, "Complete COMP15111 notes and videos", 1, 2, 2020);

	Node *head = NULL;
	head = (Node *) malloc(sizeof(Node));
	head->data = firstTask;
	head->next = NULL;

	for (int i=0; i<4; i++) {
		Task next = createTask(renderer, "Complete Exercise sheet 2 for COMP11120 (maths)", 2, i+1, 2021);
		listPush(head, next);
	}

	for (int i=0; i<4; i++) {
		Task next = createTask(renderer, "Complete Exercise sheet 2 for COMP11120 (maths)", i+1, i+1, 2028);
		listPush(head, next);
	}
	listRemove(&head, 6);
	Task task2 = createTask(renderer, "This goes in pos 2", 12, 5, 2004);
	listInsert(head, 2, task2);

	Button addTaskButton = createButton(renderer, "Add Task", 200, 100, 50, NULL);

    bool close = false;
	while (!close) {
		SDL_Event event;
        const Uint8* keys = SDL_GetKeyboardState(NULL);
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || keys[SDL_SCANCODE_ESCAPE]) {
				return 0;
			}
		}

        // Clear the screen
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

		drawTaskList(renderer, head, 10, 100, TASK_LIST_Y, SCREENWIDTH-20, SCREENWIDTH-100-10);
		
		SDL_Rect addRect = {SCREENWIDTH/2 - addTaskButton.textW/2, TASK_LIST_Y/2 - addTaskButton.textH/2, addTaskButton.textW, addTaskButton.textH};
		drawButtonBackground(renderer, &addRect);
		SDL_RenderCopy(renderer, addTaskButton.tex, NULL, &addRect);
        
		// Actually draw the screen and tick the clock
		SDL_RenderPresent(renderer);
		SDL_Delay(1000/60);
	}

	// Clean up resources before exiting
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return 0;
}