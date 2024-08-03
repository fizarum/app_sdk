#include <stdio.h>

#include "menu_app.h"

static void handleKey(const void* keyData) {
	printf("\t[Menu app] provided keydata: %u\n", *((_u16*)keyData));
}

static void onAppLoading(void) {
	printf("\t[Menu app] on app loading...\n");
}

static void onAppUpdate(void) {
	printf("\t[Menu app] on app update...\n");
}

static void onAppPause(void) {
	printf("\t[Menu app] on app pause...\n");
}

static void onAppResume(void) {
	printf("\t[Menu app] on app resume...\n");
}

static void onAppStop(void) {
	printf("\t[Menu app] on app stop...\n");
}

static AppSpecification_t menuSpecification = {
		.name = "Menu app",
		.handleInput = &handleKey,
		.onInit = &onAppLoading,
		.onStart = &onAppResume,
		.onPause = &onAppPause,
		.onResume = &onAppResume,
		.onUpdate = &onAppUpdate,
		.onStop = &onAppStop,
};

AppSpecification_t* MenuAppSpecification() {
	return &menuSpecification;
}