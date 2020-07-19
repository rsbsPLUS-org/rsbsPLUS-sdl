#ifdef NXDK
//XBOX Defines
#include <hal/debug.h>
#include <hal/video.h>
#include <hal/xbox.h>
#include <windows.h>
#endif
//SDL Defines
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_gamecontroller.h>

static void printSDLErrorAndReboot(void);

static void printIMGErrorAndReboot(void);

#ifdef NXDK
const extern int SCREEN_WIDTH;
const extern int SCREEN_HEIGHT;
#else
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#endif

void game(void){
	SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
	SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");

	SDL_Window* window = NULL;
	SDL_Event event;
	SDL_Surface* screen = NULL;
	SDL_Renderer* renderer = NULL;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init SDL.\n");
		printSDLErrorAndReboot();
	}

	window = SDL_CreateWindow("rsbs-xboxen", 
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
			SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);

	if (window == NULL) {
#ifdef NXDK
		debugPrint("Window could not be created.\n");
#else
		printf("Window could not be created.");
#endif
		SDL_VideoQuit();
		printSDLErrorAndReboot();
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	if (!(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG)){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init SDL_image.\n");
		printSDLErrorAndReboot();
	}

	screen = SDL_GetWindowSurface(window);
	if (!screen) {
		SDL_VideoQuit();
		printSDLErrorAndReboot();
	}

	SDL_Texture* blauTexture = IMG_LoadTexture(renderer, "D:\\res\\blau.bmp");
	SDL_Texture* roteTexture = IMG_LoadTexture(renderer, "D:\\res\\rote.bmp");
	SDL_Texture* grunTexture = IMG_LoadTexture(renderer, "D:\\res\\grun.bmp");

	SDL_Texture* bgTexture = IMG_LoadTexture(renderer,   "D:\\res\\bg.bmp");

	SDL_Rect sRect = {256,176,128,128};
	SDL_Rect screenRect = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};


#ifdef NXDK
	debugPrint("%i joysticks were found.\n\n", SDL_NumJoysticks() );
#else
	printf("%i joysticks were found.\n\n", SDL_NumJoysticks() );
#endif

	SDL_GameController* joystick;
	SDL_GameControllerEventState(SDL_ENABLE);
	joystick = SDL_GameControllerOpen(0);

	SDL_Texture* curCircle = roteTexture;

	uint8_t moveDelta = 10;
	uint8_t sel = 10;
	uint8_t left = 0;
	uint8_t right = 0;

	char done = 0;
	while (!done) {
		XVideoWaitForVBlank();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					done = 1;
					break;
				case SDL_CONTROLLERBUTTONDOWN:
					switch (event.cbutton.button) {
						case SDL_CONTROLLER_BUTTON_DPAD_UP:
							sel = 1;
							break;
						case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
							sel = 2;
							break;
						case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
							left = 1;
							right = 0;
							break;
						case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
							right = 1;
							left = 0;
							break;
					}
					break;
				case SDL_CONTROLLERBUTTONUP:
					switch (event.cbutton.button) {
						case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
						case SDL_CONTROLLER_BUTTON_DPAD_UP:
							sel = 0;
							break;
						case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
							left = 0;
							break;
						case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
							right = 0;
							break;
					}
					break;
			}
			switch (sel) {
				case 0:
					curCircle = roteTexture;
					break;
				case 1:
					curCircle = blauTexture;
					break;
				case 2:
					curCircle = grunTexture;
					break;
			}
		}
		if (right && (sRect.x + moveDelta <= SCREEN_WIDTH - 128 ) ) sRect.x += moveDelta;
		if (left && (sRect.x > 0) ) sRect.x -= moveDelta;
		SDL_RenderCopy(renderer, bgTexture,    NULL, &screenRect);
		SDL_RenderCopy(renderer, curCircle,    NULL, &sRect);
		SDL_RenderPresent(renderer);
		SDL_UpdateWindowSurface(window);
	}

	SDL_VideoQuit();
	SDL_GameControllerClose(joystick);
}

int main() {
#ifdef NXDK
	XVideoSetMode(640,480,32,REFRESH_DEFAULT);
#endif
	game();
	return 0;
}

#ifdef NXDK
static void printSDLErrorAndReboot(void) {
	debugPrint("SDL_Error: %s\n", SDL_GetError());
	debugPrint("Rebooting in 5 seconds.\n");
	Sleep(5000);
	XReboot();
}

static void printIMGErrorAndReboot(void) {
	debugPrint("SDL_Image Error: %s\n", IMG_GetError());
	debugPrint("Rebooting in 5 seconds.\n");
	Sleep(5000);
	XReboot();
}

#else
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


#endif
