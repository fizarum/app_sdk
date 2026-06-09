#include "app.h"

#include <stdlib.h>

typedef struct app_t {
  app_specification_t* specification;
  app_state_t state;
} app_t;

app_t* AppCreate(app_specification_t* specification) {
  app_t* app = (app_t*)malloc(sizeof(app_t));

  if (app == NULL) return NULL;

  app->specification = specification;
  app->state = StateInit;

  return app;
}

void AppDestroy(app_t* app) {
  if (app == NULL) return;
  if (app->state != StateStoped) return;

  free(app);
}

bool AppOnOpen(app_t* app) {
  if (app->state == StateInit || app->state == StateStoped) {
    app->state = StateRunning;
    app_specification_t* specs = app->specification;

    if (specs->onStart != NULL) {
      specs->onStart();
      specs->redrawNeeded = RedrawFull;
      return true;
    } else {
      // failed to start app, return to main menu
      return false;
    }
  }
}

void AppOnUpdate(app_t* app) {
  if (app->state != StateRunning) return;
  app->state = StateUpdate;
  app_specification_t* specs = app->specification;
  if (specs == NULL) {
    return;
  }

  if (specs->onUpdate != NULL) {
    specs->onUpdate();
  }

  if (specs->onRedraw != NULL && specs->redrawNeeded != RedrawNone) {
    specs->onRedraw(app->specification->redrawNeeded);
    specs->redrawNeeded = RedrawNone;
  }
  app->state = StateRunning;
}

void AppOnHandleInput(app_t* app, const void* keyData) {
  if (app->state == StateRunning && app->specification->handleInput != NULL) {
    app->specification->handleInput(keyData);
  }
}

bool AppOnPause(app_t* app) {
  if (app->state == StateRunning || app->state == StateUpdate) {
    app->state = StatePaused;
    if (app->specification->onPause != NULL) {
      app->specification->onPause();
    }
    return true;
  }
  return false;
}

bool AppOnResume(app_t* app) {
  if (app->state == StatePaused) {
    app->state = StateRunning;
    if (app->specification->onResume != NULL) {
      app->specification->onResume();
    }
    app->specification->redrawNeeded = RedrawFull;
    return true;
  }
  return false;
}

void AppOnStop(app_t* app) {
  if (app->state == StateRunning || app->state == StatePaused) {
    app->state = StateStoped;
    if (app->specification->onStop != NULL) {
      app->specification->onStop();
    }
  }
}

const char* AppGetName(const app_t* app) { return app->specification->name; }

_u16 AppGetId(const app_t* app) {
  if (app == NULL || app->specification == NULL) {
    return APP_ID_NA;
  }

  return app->specification->id;
}

app_state_t AppGetState(const app_t* app) { return app->state; }

app_specification_t* AppGetSpecification(const app_t* app) {
  return app != NULL ? app->specification : NULL;
}