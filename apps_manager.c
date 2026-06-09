#include "apps_manager.h"

#include <stack.h>
#include <stdlib.h>

typedef struct apps_manager_t {
  app_t* launcher;

  /**
   * @brief running and active applications, usually there is only one active
   * app here
   */
  Array_t* running_apps;

  /**
   * @brief place where all applications are registered
   */
  Array_t* registry;
} apps_manager_t;

const _u8 MAX_APPS_COUNT = 20;
const _u8 MAX_ACTIVE_APPS_COUNT = 5;

static apps_manager_t apps_manager = {};
static _u16 next_id;

static app_t* _create_app(app_specification_t* specs);
static app_t* _find_by_id(const _u16 app_id);

void apps_manager_init() {
  next_id = 0;
  apps_manager.launcher = NULL;
  apps_manager.running_apps = ArrayCreate(MAX_ACTIVE_APPS_COUNT);
  apps_manager.registry = ArrayCreate(MAX_APPS_COUNT);
}

_u16 apps_manger_set_launcher(app_specification_t* specs) {
  app_t* app = _create_app(specs);
  apps_manager.launcher = app;
  AppOnOpen(app);
  return AppGetId(app);
}

void apps_manager_start() {
  assert(apps_manager.launcher != NULL);

  // if already running
  if (AppGetState(apps_manager.launcher) == StateRunning) {
    return;
  }

  AppOnOpen(apps_manager.launcher);
}

void apps_manager_update() {
  _u16 len = ArraySize(apps_manager.running_apps);
  for (_u16 index = 0; index < len; index++) {
    app_t* app = ArrayValueAt(apps_manager.running_apps, index);
    AppOnUpdate(app);
  }

  if (apps_manager.launcher != NULL) {
    AppOnUpdate(apps_manager.launcher);
  }
}

void apps_manager_handle_input(const void* key_data) {
  if (ArrayIsEmpty(apps_manager.running_apps)) {
    AppOnHandleInput(apps_manager.launcher, key_data);
    return;
  }

  _u16 len = ArraySize(apps_manager.running_apps);
  for (_u16 index = 0; index < len; index++) {
    app_t* app = ArrayValueAt(apps_manager.running_apps, index);
    AppOnHandleInput(app, key_data);
  }
}

_u16 apps_manager_register(app_specification_t* specs) {
  if (ArrayIsFull(apps_manager.registry) == true) {
    return APP_ID_NA;
  }

  app_t* app = _create_app(specs);

  if (ArrayAdd(apps_manager.registry, app)) {
    return AppGetId(app);
  }
  return APP_ID_NA;
}

bool apps_manager_open_app(const _u16 app_id) {
  app_t* app = _find_by_id(app_id);
  if (app == NULL) {
    return false;
  }

  app_state_t launcher_state = AppGetState(apps_manager.launcher);
  if (launcher_state == StateRunning || launcher_state == StateUpdate) {
    AppOnPause(apps_manager.launcher);
  }

  if (ArrayIsFull(apps_manager.running_apps)) {
    return false;
  }

  bool is_ok = ArrayAdd(apps_manager.running_apps, app);
  if (is_ok) {
    is_ok = AppOnOpen(app);
    return true;
  }

  AppOnResume(apps_manager.launcher);
  return false;
}

bool apps_manager_close_app(const _u16 app_id) {
  app_t* app = _find_by_id(app_id);
  if (app == NULL) {
    return false;
  }

  if (AppOnPause(app)) {
    AppOnStop(app);
    bool is_ok = ArrayRemove(apps_manager.running_apps, app);
    if (ArrayIsEmpty(apps_manager.running_apps)) {
      AppOnResume(apps_manager.launcher);
    }
    return is_ok;
  }

  return false;
}

Array_t* apps_manager_get_all() { return apps_manager.registry; }
Array_t* apps_manager_get_running_apps() { return apps_manager.running_apps; }

app_t* apps_manager_get_app(const _u16 app_id) {
  if (app_id == AppGetSpecification(apps_manager.launcher)->id) {
    return apps_manager.launcher;
  }

  return _find_by_id(app_id);
}

app_t* _create_app(app_specification_t* specs) {
  specs->id = ++next_id;
  return AppCreate(specs);
}

app_t* _find_by_id(const _u16 app_id) {
  if (app_id == APP_ID_NA) {
    return NULL;
  }

  _u16 len = ArraySize(apps_manager.registry);

  for (_u16 index = 0; index < len; index++) {
    app_t* app = ArrayValueAt(apps_manager.registry, index);
    if (app != NULL && AppGetSpecification(app)->id == app_id) {
      return app;
    }
  }
  return NULL;
}