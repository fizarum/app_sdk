#include "apps_manager.h"

#include <stack.h>
#include <stdlib.h>

typedef struct AppsManager_t {
  app_t* menuApp;
  app_t* activeApp;
  Array_t* apps;
  Stack_t* pausedApps;
  _u16 nextAppId;
} AppsManager_t;

const _u8 MAX_APPS_COUNT = 20;
const _u8 MAX_ACTIVE_APPS_COUNT = 5;

static AppsManager_t apps_manager = {};

static bool _FindAppByIdPredicate(const void* expected, const void* value);
static _u16 _AppsManagerNextAppId();

void apps_manager_init() {
  apps_manager.nextAppId = 0;
  apps_manager.activeApp = NULL;
  apps_manager.menuApp = NULL;
  apps_manager.apps = ArrayCreate(MAX_APPS_COUNT);
  apps_manager.pausedApps = StackCreate(MAX_ACTIVE_APPS_COUNT);
}

app_t* apps_manger_set_launcher(app_specification_t* specs) {
  if (ArrayIsFull(apps_manager.apps) == true) {
    return NULL;
  }
  specs->id = _AppsManagerNextAppId();
  app_t* launcher = AppCreate(specs);
  apps_manager.menuApp = launcher;
  AppOnOpen(apps_manager.menuApp);
  return launcher;
}

_u16 apps_manager_add(app_specification_t* specs) {
  if (ArrayIsFull(apps_manager.apps) == true) {
    return false;
  }

  specs->id = _AppsManagerNextAppId();
  app_t* app = AppCreate(specs);

  if (ArrayAdd(apps_manager.apps, app)) {
    return specs->id;
  }
  return APP_ID_NA;
}

void apps_manager_start(app_t* app) {
  if (apps_manager.menuApp != NULL &&
      AppGetState(apps_manager.menuApp) == StateRunning) {
    return;
  }

  if (apps_manager.menuApp == NULL) {
    apps_manager.menuApp = app;
  }

  AppOnOpen(apps_manager.menuApp);
}

void apps_manager_start_last_added_app() {
  Stack_t* pausedApps = apps_manager.pausedApps;
  Array_t* apps = apps_manager.apps;

  // if we ahave already running active app, exit
  if (apps_manager.activeApp != NULL &&
      AppGetState(apps_manager.activeApp) == StateRunning) {
    return;
  }

  // if no apps available to run/resume
  if (StackIsEmpty(pausedApps) == true && ArrayIsEmpty(apps) == true) {
    return;
  }

  // there is some active app, resume it
  if (StackIsEmpty(pausedApps) == false) {
    app_t* app = StackPop(pausedApps);
    AppOnPause(apps_manager.menuApp);
    apps_manager.activeApp = app;
    AppOnResume(app);
    return;
  }

  // if regisered app presents, open it
  if (ArrayIsEmpty(apps) == false) {
    AppOnPause(apps_manager.menuApp);
    app_t* app = ArrayLastValue(apps);
    apps_manager.activeApp = app;
    AppOnOpen(app);
  }
}

void apps_manager_start_with_Id(const _u16 appId) {
  AppOnPause(apps_manager.menuApp);
  Array_t* apps = apps_manager.apps;

  // if we have already running active app, exit
  if (apps_manager.activeApp != NULL &&
      AppGetState(apps_manager.activeApp) == StateRunning) {
    AppOnResume(apps_manager.menuApp);
    return;
  }

  app_t* app = ArrayFind(apps, (void*)appId, &_FindAppByIdPredicate);
  if (app != NULL) {
    apps_manager.activeApp = app;
    bool started = AppOnOpen(apps_manager.activeApp);
    if (started == false) {
      apps_manager.activeApp = NULL;
      AppOnResume(apps_manager.menuApp);
      return;
    }
  } else {
    AppOnResume(apps_manager.menuApp);
  }
}

void apps_manager_update() {
  app_t* activeApp = apps_manager.activeApp;
  if (activeApp != NULL) {
    AppOnUpdate(activeApp);
    return;
  }
  if (apps_manager.menuApp != NULL) {
    AppOnUpdate(apps_manager.menuApp);
  }
}

void apps_manager_handle_input(const void* keyData) {
  app_t* activeApp = apps_manager.activeApp;
  if (activeApp == NULL) {
    activeApp = apps_manager.menuApp;
  }

  AppOnHandleInput(activeApp, keyData);
}

void apps_manager_pause_active_app() {
  app_t* activeApp = apps_manager.activeApp;
  if (activeApp != NULL) {
    AppOnPause(activeApp);
    StackPush(apps_manager.pausedApps, activeApp);
    activeApp = apps_manager.menuApp;

    AppOnResume(activeApp);
  }
}

void apps_manager_resume_app() {
  if (apps_manager.activeApp != NULL) {
    return;
  }

  app_t* app = StackPop(apps_manager.pausedApps);
  if (app != NULL) {
    apps_manager.activeApp = app;
    AppOnResume(app);
    AppOnPause(apps_manager.menuApp);
  }
}

void apps_manager_stop_app() {
  if (apps_manager.activeApp == NULL) {
    return;
  }

  AppOnPause(apps_manager.activeApp);
  AppOnStop(apps_manager.activeApp);

  apps_manager.activeApp = NULL;
  AppOnResume(apps_manager.menuApp);
}

Array_t* apps_manager_get_all() { return apps_manager.apps; }
app_t* apps_manager_get_active_app() { return apps_manager.activeApp; }

//
static bool _FindAppByIdPredicate(const void* expected, const void* value) {
  const _u16 appIdToFind = (_u16)expected;
  app_t* app = (app_t*)value;
  _u16 appId = AppGetId(app);
  return appIdToFind == appId;
}

/**
 * @brief generate id for application
 */
_u16 _AppsManagerNextAppId() {
  apps_manager.nextAppId++;
  return apps_manager.nextAppId;
}
