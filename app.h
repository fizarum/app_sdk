#ifndef APP_H
#define APP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "app_state.h"
#include "broadcast/broadcast_event.h"

typedef uint8_t _u8;
typedef int8_t _i8;
typedef uint16_t _u16;

typedef void(UserCallback)(const _u16 appId);

typedef enum RedrawType_t {
  RedrawNone,
  RedrawFull,
  RedrawPartial,
  RedrawCustom,
} RedrawType_t;

/** app's specific implementation */
typedef struct AppSpecification_t {
  const char* name;
  const _u16 background;
  _u16 id;

  /**
   * @brief Flag indicating that app should be redrawn
   * (by calling onRedraw()) on next app update. Check RedrawType_t definition
   * for redraw type
   */
  RedrawType_t redrawNeeded;

  void (*onStart)(void);
  void (*onPause)(void);
  void (*onResume)(void);
  void (*onUpdate)(void);
  void (*onRedraw)(RedrawType_t);
  void (*onStop)(void);

  void (*onBroadcastEvent)(BroadcastEvent_t);
  void (*handleInput)(const void* keyData);

} AppSpecification_t;

typedef struct App_t App_t;

App_t* AppCreate(AppSpecification_t* specification);
void AppDestroy(App_t* app);

void AppOnOpen(App_t* app);
void AppOnUpdate(App_t* app);
void AppOnHandleInput(App_t* app, const void* keyData);
bool AppOnPause(App_t* app);
bool AppOnResume(App_t* app);
void AppOnStop(App_t* app);

const char* AppGetName(const App_t* app);
_u16 AppGetId(const App_t* app);
AppState_t AppGetState(const App_t* app);
AppSpecification_t* AppGetSpecification(const App_t* app);

#ifdef __cplusplus
}
#endif

#endif  // APP_H