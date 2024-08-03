/**
* example of application build with app sdk
*
*/

#include "screenlock_app.h"
#include <stdio.h>

static void handleKey(const void* keyData) {
	printf("\t[Screenlock] provided keydata: %u\n", *((_u16*)keyData));
}

static void onAppLoading(void) {
	printf("\t[Screenlock] on app loading...\n");
}

static void onAppUpdate(void) {
	printf("\t[Screenlock] on app update...\n");
}

static void onAppStart(void) {
	printf("\t[Screenlock] on app start...\n");
}

static void onAppPause(void) {
	printf("\t[Screenlock] on app pause...\n");
}

static void onAppResume(void) {
	printf("\t[Screenlock] on app resume...\n");
}

static void onAppStop(void) {
	printf("\t[Screenlock] on app stop...\n");
}

static void stub(void) {
	printf("\t[Screenlock] stub called\n");
}

static AppSpecification_t screenLockSpecification = {
		.name = "Screenlock app",
		.handleInput = &handleKey,
		.onInit = &onAppLoading,
		.onStart = &onAppStart,
		.onPause = &onAppPause,
		.onResume = &onAppResume,
		.onUpdate = &onAppUpdate,
		.onStop = &onAppStop,
};

AppSpecification_t* ScreenLockAppSpecification() {
	return &screenLockSpecification;
}
