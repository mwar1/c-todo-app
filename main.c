#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "widget.h"

#define SCREENWIDTH 1000
#define TASK_LIST_Y 80

Node *head;

void addTask(SDL_Renderer *renderer) {
	Task newTask = createTask(renderer, "abc", 10, 12, 2020);
	listPush(head, newTask, false);
}

int mainTaskView(SDL_Renderer *renderer) {
	// Display the list of tasks to the screen
	// Returns 0 if closed, 1 if switching to the add task screen

	Button addTaskButton = createButton(renderer, "Add Task", 200, 100, 50);
	SDL_Rect addRect = {SCREENWIDTH/2 - addTaskButton.textW/2, TASK_LIST_Y/2 - addTaskButton.textH/2, addTaskButton.textW, addTaskButton.textH};

	Task first = createTask(renderer, "testing testing", 20, 3, 2020);
	head = (Node*) malloc(sizeof(Node));
	listPush(head, first, true);

	while (true) {
		SDL_Event event;
        const Uint8* keys = SDL_GetKeyboardState(NULL);
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || keys[SDL_SCANCODE_ESCAPE]) {
				return 0;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);

				// Check if we need to add a task
				if (isButtonClicked(&addRect, mouseX, mouseY)) {
					return 1; // Switch to the add task screen
				}

				Node* current = head;
				while (current != NULL) {
					if (isButtonClicked(&current->data.doneRect, mouseX, mouseY)) {
						current->data.complete = true;
						// TODO: update display
					}
					if (isButtonClicked(&current->data.editRect, mouseX, mouseY)) {
						// Launch add task screen with data filled out
						return 1;
					}
					current = current->next;
				}
			}
			if (event.type == SDL_KEYDOWN) {
				listPop(head);
			}
		}

        // Clear the screen
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

		drawTaskList(renderer, head, 10, 100, TASK_LIST_Y, SCREENWIDTH-20, SCREENWIDTH-100-10);
		
		drawButtonBackground(renderer, &addRect);
		SDL_RenderCopy(renderer, addTaskButton.tex, NULL, &addRect);
        
		// Actually draw the screen and tick the clock
		SDL_RenderPresent(renderer);
		SDL_Delay(1000/60);
	}
}

int addTaskView(SDL_Renderer *renderer) {
	// Allows a task to be added by the user
	// Returns 0 if the user closes the window, 1 if they add the task or go back

	Button addButton = createButton(renderer, "Add", 70, 50, 50);
	SDL_Rect addRect = {SCREENWIDTH/2 - addButton.textW/2, SCREENWIDTH - addButton.textH - PADDING_PX, addButton.textW, addButton.textH};

	Button backButton = createButton(renderer, "Back", 100, 50, 50);
	SDL_Rect backRect = {PADDING_PX, PADDING_PX, backButton.textW, backButton.textH};

	while (true) {
		SDL_Event event;
        const Uint8* keys = SDL_GetKeyboardState(NULL);
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || keys[SDL_SCANCODE_ESCAPE]) {
				return 0;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				if (isButtonClicked(&backRect, mouseX, mouseY)) {
					return 1;
				}
			}
		}

		// Clear the screen
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

		// drawTaskList(renderer, head, 10, 100, TASK_LIST_Y, SCREENWIDTH-20, SCREENWIDTH-100-10);
		
		drawButtonBackground(renderer, &addRect);
		SDL_RenderCopy(renderer, addButton.tex, NULL, &addRect);
		drawButtonBackground(renderer, &backRect);
		SDL_RenderCopy(renderer, backButton.tex, NULL, &backRect);
        
		// Actually draw the screen and tick the clock
		SDL_RenderPresent(renderer);
		SDL_Delay(1000/60);
	}
}

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

	while(mainTaskView(renderer) && addTaskView(renderer));

	// Clean up resources before exiting
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return 0;
}