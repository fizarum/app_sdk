#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

#include "app_state.h"
#include "broadcast/broadcast_event.h"

typedef void(UserCallback)(const _u16 appId);

typedef enum redraw_type_t {
  RedrawNone,
  RedrawFull,
  RedrawPartial,
  RedrawCustom,
} redraw_type_t;

/** app's specific implementation */
typedef struct app_specification_t {
  const char* name;
  _u16 id;

  /**
   * @brief Flag indicating that app should be redrawn
   * (by calling onRedraw()) on next app update. Check redraw_type_t definition
   * for redraw type
   */
  redraw_type_t redrawNeeded;

  void (*onStart)(void);
  void (*onPause)(void);
  void (*onResume)(void);
  void (*onUpdate)(void);
  void (*onRedraw)(redraw_type_t);
  void (*onStop)(void);

  void (*onBroadcastEvent)(broadcast_event_t);
  void (*handleInput)(const void* keyData);

} app_specification_t;

typedef struct app_t app_t;

app_t* AppCreate(app_specification_t* specification);
void AppDestroy(app_t* app);

bool AppOnOpen(app_t* app);
void AppOnUpdate(app_t* app);
void AppOnHandleInput(app_t* app, const void* keyData);
bool AppOnPause(app_t* app);
bool AppOnResume(app_t* app);
void AppOnStop(app_t* app);

const char* AppGetName(const app_t* app);
_u16 AppGetId(const app_t* app);
app_state_t AppGetState(const app_t* app);
app_specification_t* AppGetSpecification(const app_t* app);

#ifdef __cplusplus
}
#endif
