#include "widget.h"

Button createButton(SDL_Renderer *r, char* text, int w, int h, int fontSize) {
	TTF_Init();

	Button newButton;

	newButton.w = w;
	newButton.h = h;

	TTF_Font *font = TTF_OpenFont("res/font.otf", fontSize);
	SDL_Surface *textSurf = TTF_RenderText_Solid(font, text, (SDL_Color) {0, 0, 0, 255});
	SDL_Texture *textTex = SDL_CreateTextureFromSurface(r, textSurf);
	newButton.tex = textTex;

	TTF_SizeText(font, text, &newButton.textW, &newButton.textH);

	// Free resources
	SDL_FreeSurface(textSurf);

	return newButton;
}

bool isButtonClicked(SDL_Rect *buttonRect, int mouseX, int mouseY) {
	SDL_Rect mouseRect = {mouseX, mouseY, 1, 1};

	return SDL_HasIntersection(buttonRect, &mouseRect);
}

TextBox createTextBox(SDL_Renderer *r, int x, int y, int w, int h, int fontSize, char* defaultText) {
	TextBox box;
	box.x = x;
	box.y = y;
	box.w = w;
	box.h = h;
	box.defaultText = defaultText;
	box.text = malloc(1);
	box.text[0] = '\0';
	box.textLength = 0;
	box.textW, box.textH = 0;
	box.fontSize = fontSize;
	box.active = false;
	updateBox(r, &box);

	return box;
}

char* getText(TextBox *box) {
	// Get the string of text which has been typed into the box
	return box->text;
}

int addText(TextBox *box, char c) {
	// Add a single character to the textbox
	// Returns 0 on success, 1 otherwise

	box->textLength++;
	box->text = realloc(box->text, box->textLength+1);
	box->text[box->textLength-1] = c;
	box->text[box->textLength] = '\0';

	return 0;
}

int backspace(TextBox *box) {
	// Remove a single character from the text in the textbox
	// Returns 0 on success, 1 otherwise
	if (box->textLength == 0) return 1;

	box->textLength--;
	box->text = realloc(box->text, box->textLength+1);
	box->text[box->textLength] = '\0';

	return 0;
}

void updateBox(SDL_Renderer *r, TextBox *box) {
	TTF_Font *font = TTF_OpenFont("res/font.otf", box->fontSize);
	int newW, newH;
	
	SDL_Surface *textSurf;
	if (box->textLength == 0) {
		textSurf = TTF_RenderText_Solid(font, box->defaultText, (SDL_Color) {100, 100, 100, 255});
		TTF_SizeText(font, box->defaultText, &newW, &newH);
	} else {
		textSurf = TTF_RenderText_Solid(font, box->text, (SDL_Color) {0, 0, 0, 255});
		TTF_SizeText(font, box->text, &newW, &newH);
	}
	box->textW = newW;
	box->textH = newH;

	box->textTex = SDL_CreateTextureFromSurface(r, textSurf);
}

bool isTextBoxClicked(TextBox *box, int mouseX, int mouseY) {
	SDL_Rect mouseRect = {mouseX, mouseY, 1, 1};
	SDL_Rect boxRect = {box->x, box->y, box->w, box->h};

	return SDL_HasIntersection(&boxRect, &mouseRect);
}

Task createTask(SDL_Renderer *r, char *taskText, int day, int month, int year) {
	Button tickButton = createButton(r, "Done", 70, 50, 24);
	Button editButton = createButton(r, "Edit", 70, 50, 24);
	Date newDate = {day, month, year};
	Task newTask = {tickButton, editButton, newDate, taskText, false};

	TTF_Font *font = TTF_OpenFont("res/font.otf", 25);
	SDL_Surface *textSurf = TTF_RenderText_Solid(font, newTask.taskText, (SDL_Color) {0, 0, 0, 255});
	newTask.textTex = SDL_CreateTextureFromSurface(r, textSurf);

	TTF_SizeText(font, newTask.taskText, &newTask.textW, &newTask.textH);

	// Display the due date
	char dateString[16];
	snprintf(dateString, 16, "Due: %d/%d/%d", day, month, year);
	SDL_Surface *dateSurf = TTF_RenderText_Solid(font, dateString, (SDL_Color) {0, 0, 0, 255});
	newTask.dateTex = SDL_CreateTextureFromSurface(r, dateSurf);

	TTF_SizeText(font, dateString, &newTask.dateW, &newTask.dateH);

	// Free resources
	SDL_FreeSurface(textSurf);
	SDL_FreeSurface(dateSurf);

	return newTask;
}

void listInsert(Node *head, int pos, Task val) {
    // Add 'val' at position 'pos' in the linked list
    Node *current = head;
	for (int i=0; i<pos-1; i++) {
		current = current->next;
	}

	Node *insert = (Node *) malloc(sizeof(Node));
	insert->data = val;
	insert->next = current->next;
	current->next = insert;
}


void listPush(Node *head, Task val, bool init) {
    // Add to the end of the linked list
	// 'init' is used to signal this is the first push to the linked list
	if (init) {
		head->data = val;
		head->next = NULL;
		return;
	}
	
	Node *current;

	current = head;
	while (current->next != NULL) {
		current = current->next;
	}

	current->next = (Node*) malloc(sizeof(Node));
	current->next->data = val;
	current->next->next = NULL;
}


Task* listGetFirst(Node** head) {
    // Returns the first element in the linked list
    Task *result = NULL;

    if (*head == NULL) {
        return result;
    }

    Node *next_node = NULL;
    next_node = (*head)->next;
    result = &(*head)->data;
    free(*head);
    *head = next_node;

    return result;
}


Task* listPop(Node* head) {
    // Removes and returns the final element in the linked list
    Task *result = NULL;
	if (head->next == NULL) {
		result = &head->data;
		free(head);
		return result;
	}

	Node *current = head;
	while (current->next->next != NULL) {
		current = current->next;
	}

 	result = &current->next->data;
 	free(current->next);
	current->next = NULL;

	return result;
}


Task* listRemove(Node** head, int pos) {
    // Removes and returns the element at position 'pos' from the linked list
    Task *result = NULL;
	Node *current = *head;
	Node *tempNode = NULL;

	if (pos == 0) {
		return listGetFirst(head);
	}

	for (int i=0; i<pos-1; i++) {
		if (current->next == NULL) {
			return result;
		}
		current = current->next;
	}

	tempNode = current->next;
	result = &tempNode->data;
	current->next = tempNode->next;
	free(tempNode);

	return result;	
}

void drawButtonBackground(SDL_Renderer *r, SDL_Rect *buttonRect) {
	SDL_SetRenderDrawColor(r,0, 0, 0, 255);
	SDL_Rect paddedRect = {buttonRect->x - PADDING_PX/2, buttonRect->y, buttonRect->w + PADDING_PX, buttonRect->h};
	SDL_RenderDrawRect(r, &paddedRect);
}

void displayTasks(SDL_Renderer *r, Node *head, int i, int listX, int listY, int taskWidth, int taskHeight) {
	TTF_Init();
	if (head == NULL) return;
	Task cur = head->data;

	// Draw task background
	SDL_SetRenderDrawColor(r, 125, 125, 125, 255);
	SDL_Rect taskRect = {listX + PADDING_PX, listY + (i * (taskHeight + PADDING_PX)) + PADDING_PX, taskWidth, taskHeight};
	SDL_RenderFillRect(r, &taskRect);


	// Draw the two buttons and the text
	// Nicely space the text and the two buttons
	SDL_Rect doneRect = {taskRect.x + PADDING_PX, taskRect.y + taskRect.h/2 - cur.tickBoxButton.textH, cur.tickBoxButton.w, cur.tickBoxButton.h};
	SDL_Rect editRect = {taskRect.x + taskRect.w - PADDING_PX - cur.editButton.w, taskRect.y + taskRect.h/2 - cur.editButton.textH, cur.editButton.w, cur.editButton.h};
	SDL_Rect dateRect = {editRect.x - cur.dateW - PADDING_PX, taskRect.y + taskRect.h/2 - cur.dateH/2, cur.dateW, cur.textH};

	head->data.doneRect = doneRect;
	head->data.editRect = editRect;

	if (cur.textW > dateRect.x - doneRect.x - doneRect.w - PADDING_PX) cur.textW = dateRect.x - doneRect.x - doneRect.w - PADDING_PX;
	if (cur.textH > taskHeight - PADDING_PX*2) cur.textH = taskHeight - PADDING_PX*2;
	// TODO: centre task text
	SDL_Rect textRect = {doneRect.x + doneRect.w + PADDING_PX, taskRect.y + taskRect.h/2 - cur.textH/2, cur.textW, cur.textH};

	drawButtonBackground(r, &doneRect);
	drawButtonBackground(r, &editRect);

	SDL_SetRenderDrawColor(r, 75, 75, 75, 255);
	
	if (SDL_RenderCopy(r, cur.tickBoxButton.tex, NULL, &doneRect) != 0) {
		printf("Error code tick box: %s\n", SDL_GetError());
	}

	if (SDL_RenderCopy(r, cur.textTex, NULL, &textRect) != 0) {
		printf("Error code text: %s\n", SDL_GetError());
	}

	if (SDL_RenderCopy(r, cur.editButton.tex, NULL, &editRect) != 0) {
		printf("Error code edit: %s\n", SDL_GetError());
	}
	SDL_RenderCopy(r, cur.dateTex, NULL, &dateRect);

    if (head->next != NULL)
        displayTasks(r, head->next, ++i, listX, listY, taskWidth, taskHeight);
}

void drawTaskList(SDL_Renderer *r, Node* taskListHead, int x, int y, int taskHeight,
				  int taskListWidth, int taskListHeight) {
	// Draws all Tasks in the list to the screen.
	// Will cut tasks in half if they don't completely fit, to allow scrolling.

	// Draw the background
	SDL_SetRenderDrawColor(r, 200, 200, 200, 255);
	SDL_Rect taskListRect = {x, y, taskListWidth, taskListHeight};
	SDL_RenderFillRect(r, &taskListRect);

	displayTasks(r, taskListHead, 0, x, y, taskListWidth - 2*PADDING_PX, taskHeight);
}

void drawTextBox(SDL_Renderer *r, TextBox *box) {
	// Draw a textbox to the screen at position x, y
	if (box->active) {
		SDL_SetRenderDrawColor(r, 200, 200, 200, 255);
	} else {
		SDL_SetRenderDrawColor(r, 220, 220, 220, 255);
	}

	SDL_Rect rect = {box->x, box->y, box->w, box->h};
	SDL_RenderFillRect(r, &rect);

	SDL_Rect textRect = {box->x + PADDING_PX, box->y + box->h/2 - box->textH/2, box->textW, box->textH};
	SDL_RenderCopy(r, box->textTex, NULL, &textRect);
}
