#include <SDL.h>
#include <stdio.h>

#include "../app.h"
#include "../apps_manager.h"
#include "../broadcast/broadcast_manager.h"
#include "menu_app.h"
#include "screenlock_app.h"

#define KEYCODE_TRIGGER_BROADCAST 98
#define KEYCODE_TRIGGER_MENU 120

bool initSDL();
void close();

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;

AppsManager_t* manager;
BroadcastManager_t* broadcastManager;

BroadcastEvent_t event;

static void test() {
  manager = AppsManagerCreate();
  broadcastManager = BroadcastManagerCreate();

  printf("==> create app\n");

  _u16 menuAppId = AppsManagerNextAppId(manager);
  App_t* menuApp = AppCreate(MenuAppSpecification(menuAppId));

  _u16 screenLockAppId = AppsManagerNextAppId(manager);
  AppSpecification_t* screenLockAppSpecification =
      ScreenLockAppSpecification(screenLockAppId);
  App_t* screenLockApp = AppCreate(screenLockAppSpecification);

  printf("==> adding app: %s id[%d]\n", AppGetName(menuApp), AppGetId(menuApp));
  printf("==> adding app: %s id[%d]\n", AppGetName(screenLockApp),
         AppGetId(screenLockApp));

  AppsManagerAddApp(manager, menuApp);
  AppsManagerAddApp(manager, screenLockApp);

  printf("==> starting app menu\n");
  AppsManagerStart(manager, menuApp);

  printf("==> start app by id\n");
  AppsManagerStartAppWithId(manager, screenLockAppId);

  // broadcast manager part
  bool registered = BroadcastManager_Subscribe(
      broadcastManager, EventTypeChangeBatteryLevel, screenLockApp);
  if (registered == true) {
    printf("broadcast listener with id[%d] registered\n",
           AppGetId(screenLockApp));
  }
}

static void sendTestBroadcastEvent() {
  event.type = EventTypeChangeBatteryLevel;
  event.payload = 98;
  printf("sending broadcast event: %d\n", event.value);
  BroadcastManager_SendEvent(broadcastManager, event);
}

int main() {
  if (!initSDL()) {
    printf("Failed to initialize!\n");
    return 1;
  }

  bool quit = false;
  SDL_Event e;
  static _u16 sdlKeyData = 0;

  test();
  event.value = 0;

  while (!quit) {
    // Handle events on queue
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_KEYDOWN) {
        sdlKeyData = e.key.keysym.sym;

        switch (sdlKeyData) {
          case KEYCODE_TRIGGER_BROADCAST: {
            sendTestBroadcastEvent();
            break;
          }

          case KEYCODE_TRIGGER_MENU: {
            printf("pressed menu button\n");
            App_t* activeApp = AppsManagerGetActiveApp(manager);
            BroadcastManager_RemoveListener(broadcastManager, activeApp);
            AppsManagerStopActiveApp(manager);
            break;
          }
          default:
            AppsManagerHandleInput(manager, &sdlKeyData);
            break;
        }
      }

      // User requests quit
      if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
        quit = true;
      }
    }
    AppsManagerUpdate(manager);
    BroadcastManager_Update(broadcastManager);
  }

  return 0;
}

bool initSDL() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return false;
  }

  // Create window
  gWindow =
      SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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