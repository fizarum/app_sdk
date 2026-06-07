#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum app_state_t {
  /**
   * State when app is initialized, but not opened
   * yet (shown on screen). Actual when system loads
   * all app descriptors/metadata
   */
  StateInit = 0,

  /**
   * Opened and visible, this state is usually
   * used by loading app resources
   */
  StateOpen = 1,

  /** When app is ready to interact with user */
  StateRunning = 2,

  /**
   * May be used when system sends some event
   * to app (like broadcast)
   */
  StateUpdate = 3,

  /**
   * If some alert or system menu shown up. Also can
   * happed before app's stop and further kill
   */
  StatePaused = 4,

  /**
   * Called on app closing. At this state app can't
   * be shown on screen
   */
  StateStoped = 5,
} app_state_t;

#ifdef __cplusplus
}
#endif
