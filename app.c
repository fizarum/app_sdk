#include "app.h"

#include <stdlib.h>

typedef struct App_t {
  AppSpecification_t* specification;

  AppState_t state;
} App_t;

App_t* AppCreate(AppSpecification_t* specification) {
  App_t* app = (App_t*)malloc(sizeof(App_t));

  if (app == NULL) return NULL;

  app->specification = specification;
  app->state = StateInit;

  return app;
}

void AppDestroy(App_t* app) {
  if (app == NULL) return;
  if (app->state != StateStoped) return;

  free(app);
}

void AppOnOpen(App_t* app) {
  if (app->state == StateInit || app->state == StateStoped) {
    app->state = StateRunning;
    app->specification->onStart();
    app->specification->redrawNeeded = RedrawFull;
  }
}

void AppOnUpdate(App_t* app) {
  if (app->state != StateRunning) return;
  app->state = StateUpdate;
  app->specification->onUpdate();

  if (app->specification->redrawNeeded != RedrawNone) {
    app->specification->onRedraw(app->specification->redrawNeeded);
    app->specification->redrawNeeded = RedrawNone;
  }
  app->state = StateRunning;
}

void AppOnHandleInput(App_t* app, const void* keyData) {
  if (app->state == StateRunning) {
    app->specification->handleInput(keyData);
  }
}

bool AppOnPause(App_t* app) {
  if (app->state == StateRunning || app->state == StateUpdate) {
    app->state = StatePaused;
    app->specification->onPause();
    return true;
  }
  return false;
}

bool AppOnResume(App_t* app) {
  if (app->state == StatePaused) {
    app->state = StateRunning;
    app->specification->onResume();
    app->specification->redrawNeeded = RedrawFull;
    return true;
  }
  return false;
}

void AppOnStop(App_t* app) {
  if (app->state == StateRunning || app->state == StatePaused) {
    app->state = StateStoped;
    app->specification->onStop();
  }
}

const char* AppGetName(const App_t* app) { return app->specification->name; }

_u16 AppGetId(const App_t* app) { return app->specification->id; }

AppState_t AppGetState(const App_t* app) { return app->state; }

AppSpecification_t* AppGetSpecification(const App_t* app) {
  return app->specification;
}