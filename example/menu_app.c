#include <stdio.h>

#include "menu_app.h"

static AppSpecification_t spec = {
		.name = "Menu app",
};

static void handleKey(const _u16 appId, const void* keyData) {
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

AppSpecification_t* MenuAppSpecification(const _u16 id) {
	spec.id = id;
	spec.handleInput = &handleKey;
	spec.onInit = &onAppLoading;
	spec.onStart = &onAppResume;
	spec.onPause = &onAppPause;
	spec.onResume = &onAppResume;
	spec.onUpdate = &onAppUpdate;
	spec.onStop = &onAppStop;

	return &spec;
}