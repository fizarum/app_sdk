#include "../apps_manager.h"
#include "../app.h"
#include <stdio.h>

#include "screenlock_app.h"
#include "menu_app.h"
#include <SDL.h>

bool initSDL();
void close();

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;

AppsManager_t* manager;

static void closeAppWithId(const _u16 appId) {
	if (manager == NULL) {
		return;
	}
	printf("requested to stop app: %d\n", appId);

	AppsManagerStopAppWithId(manager, appId);
}

static void test() {
	manager = AppsManagerCreate();

	printf("==> create app\n");

	_u16 menuAppId = AppsManagerNextAppId(manager);
	App_t* menuApp = AppCreate(MenuAppSpecification(menuAppId));

	_u16 screenLockAppId = AppsManagerNextAppId(manager);
	AppSpecification_t* screenLockAppSpecification = ScreenLockAppSpecification(screenLockAppId , &closeAppWithId);
	App_t* screenLockApp = AppCreate(screenLockAppSpecification);

	printf("==> adding app: %s id[%d]\n", AppGetName(menuApp), AppGetId(menuApp));
	printf("==> adding app: %s id[%d]\n", AppGetName(screenLockApp), AppGetId(screenLockApp));

	AppsManagerAddApp(manager, menuApp);
	AppsManagerAddApp(manager, screenLockApp);

	printf("==> starting app menu\n");
	AppsManagerStart(manager, menuApp);
	/*printf("==> starting app\n");
	AppsManagerStartLastAddedApp(manager);

	printf("==> updating user app\n");
	AppsManagerUpdate(manager);

	printf("==> handle input\n");
	AppsManagerHandleInput(manager, 11);

	printf("==> pausing user app\n");
	AppsManagerPauseActiveApp(manager);

	printf("==> resuming user app\n");
	AppsManagerResumeActiveApp(manager);
	printf("==> stoping user app\n");
	AppsManagerStopActiveApp(manager);
	*/
	printf("==> start app by id\n");
	AppsManagerStartAppWithId(manager, screenLockAppId);
}

static _u16 sdlKeyData = 0;

int main() {
	if (!initSDL()) {
		printf("Failed to initialize!\n");
		return 1;
	}

	bool quit = false;
	SDL_Event e;

	test();

	while (!quit) {
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_KEYDOWN) {
				sdlKeyData = e.key.keysym.sym;
				AppsManagerHandleInput(manager, &sdlKeyData);
			}

			//User requests quit
			if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
				quit = true;
			}
		}
	}

	return 0;
}

bool initSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	//Create window
	gWindow = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	gScreenSurface = SDL_GetWindowSurface(gWindow);

	return true;
}

void close() {
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}