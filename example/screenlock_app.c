/**
* example of application build with app sdk
*
*/

#include "screenlock_app.h"
#include <stdio.h>

_u16 receivedKeyData = 0;
void (*requestToStop)(const _u16 appId) = NULL;

static void handleKey(const _u16 appId, const void* keyData) {
	receivedKeyData = *((_u16*)keyData);
	if (receivedKeyData == 120) {
		printf("\t closing app\n");
		if (requestToStop != NULL) {
			requestToStop(appId);
		}
	}
}

static void onAppLoading(void) {
	printf("\t[Screenlock] on app loading...\n");
}

static void onAppUpdate(void) {
	printf("\t[Screenlock] on app update...\n");
	printf("\t press 'X' to unlock device\n");
}

static void onAppStart(void) {
	printf("\t press 'X' to unlock device\n");
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

AppSpecification_t* ScreenLockAppSpecification(UserCallback requestToStopCallback) {
	requestToStop = requestToStopCallback;
	return &screenLockSpecification;
}
