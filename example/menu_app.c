#include "menu_app.h"

#include <stdio.h>

static AppSpecification_t spec = {
    .name = "Menu app",
};

static void handleKey(const void* keyData) {
  printf("\t[Menu app] provided keydata: %u\n", *((_u16*)keyData));
}

static bool onAppLoading(void) {
  printf("\t[Menu app] on app loading...\n");
  return true;
}

static void onAppUpdate(void) {}

static void onBroadcastEvent(BroadcastEvent_t event) {
  printf("\t[Screenlock] on broadcast event %d\n", event.value);
}

static void onAppPause(void) { printf("\t[Menu app] on app pause...\n"); }

static void onAppResume(void) { printf("\t[Menu app] on app resume...\n"); }

static void onAppStop(void) { printf("\t[Menu app] on app stop...\n"); }

AppSpecification_t* MenuAppSpecification(const _u16 id) {
  spec.id = id;
  spec.handleInput = &handleKey;
  spec.onInit = &onAppLoading;
  spec.onStart = &onAppResume;
  spec.onPause = &onAppPause;
  spec.onResume = &onAppResume;
  spec.onUpdate = &onAppUpdate;
  spec.onBroadcastEvent = &onBroadcastEvent;
  spec.onStop = &onAppStop;

  return &spec;
}