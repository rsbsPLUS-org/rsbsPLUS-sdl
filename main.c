//SPDX-License-Identifier: BSD-3-Clause
//SPDX-FileCopyrightText: 2020 Lorenzo Cauli (lorecast162)

//SDL Defines
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_gamecontroller.h>

static void printSDLErrorAndReboot(void);

static void printIMGErrorAndReboot(void);

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define FPS 60

void game(void){
	//declare SDL window, event and renderer
	SDL_Window* window = NULL;
	SDL_Event event;
	SDL_Renderer* renderer = NULL;

	//init SDL video and game controller
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init SDL.\n");
		printSDLErrorAndReboot();
	}

	//create window
	window = SDL_CreateWindow("rsbsPLUS-sdl", 
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);

	//throw error if window wasn't created
	if (window == NULL) {
		printf("Window could not be created.");
		SDL_VideoQuit();
		printSDLErrorAndReboot();
	}

	//create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	//init SDL_image with jpg
	if (!(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG)){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init SDL_image.\n");
		printSDLErrorAndReboot();
	}

	SDL_Texture* blauTexture = IMG_LoadTexture(renderer, "./res/blau.bmp");
	SDL_Texture* rotTexture = IMG_LoadTexture(renderer, "./res/rot.bmp");
	SDL_Texture* gruenTexture = IMG_LoadTexture(renderer, "./res/gruen.bmp");
	SDL_Texture* bgTexture = IMG_LoadTexture(renderer,   "./res/bg.bmp");

	//create rects for circles and the screen. used in rendering
	SDL_Rect sRect = {256,176,128,128};
	SDL_Rect screenRect = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};


	//log how many joysticks were found
	printf("%i joysticks were found.\n\n", SDL_NumJoysticks() );

	//create joystick and open joystick 0
	SDL_GameController* joystick;
	SDL_GameControllerEventState(SDL_ENABLE);
	joystick = SDL_GameControllerOpen(0);

	//set current circle to default circle (red)
	SDL_Texture* curCircle = rotTexture;

	//declare game vars
	uint8_t moveDelta = 10;

	//declare variable to stop game loop
	char done = 0;
	while (!done) {
		//event loop
		while (SDL_PollEvent(&event)) {
			//check event type
			switch (event.type) {
				case SDL_QUIT:
					done = 1;
					break;
			}
		}
		const uint8_t* state = SDL_GetKeyboardState(NULL);
		
		//if right is pressed and resulting x is not out of the screen make circle go right 
		if (state[SDL_SCANCODE_RIGHT] || SDL_GameControllerGetButton(joystick, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) && (sRect.x + moveDelta <= SCREEN_WIDTH - 128 ) ) sRect.x += moveDelta;
		
		//if left is set and resulting x is not out of the screen make circle go left
		else if (state[SDL_SCANCODE_LEFT] || SDL_GameControllerGetButton(joystick, SDL_CONTROLLER_BUTTON_DPAD_LEFT) && (sRect.x > 0) ) sRect.x -= moveDelta;

		//if up is pressed set to blue
		if (state[SDL_SCANCODE_UP] || SDL_GameControllerGetButton(joystick, SDL_CONTROLLER_BUTTON_DPAD_UP)) curCircle = blauTexture;
		//otherwise if down is pressed set to green
		else if (state[SDL_SCANCODE_DOWN] || SDL_GameControllerGetButton(joystick, SDL_CONTROLLER_BUTTON_DPAD_DOWN)) curCircle = gruenTexture;
		//otherwise set to red
		else curCircle = rotTexture;

		//render out background to clear screen
		SDL_RenderCopy(renderer, bgTexture,    NULL, &screenRect);
		//render circle
		SDL_RenderCopy(renderer, curCircle,    NULL, &sRect);
		//"blit" render to window
		SDL_RenderPresent(renderer);
		SDL_UpdateWindowSurface(window);
	}

	//stop video system
	SDL_VideoQuit();
	//close joystick stream
	SDL_GameControllerClose(joystick);
}

int main() {
	game();
	return 0;
}

//functions for error reporting
static void printSDLErrorAndReboot(void) {
	printf("SDL_Error: %s\n", SDL_GetError());
	printf("Rebooting in 5 seconds.\n");
	IMG_Quit();
	SDL_Quit();
}

static void printIMGErrorAndReboot(void) {
	printf("SDL_Image Error: %s\n", IMG_GetError());
	printf("Rebooting in 5 seconds.\n");
	IMG_Quit();
	SDL_Quit();
}
