#include <stdlib.h>
#include <array.h>
#include <stack.h>

#include "apps_manager.h"

typedef struct AppsManager_t {
	App_t* menuApp;
	App_t* activeApp;
	Array_t* apps;
	Stack_t* pausedApps;
	_u16 nextAppId;
} AppsManager_t;

const _u8 MAX_APPS_COUNT = 20;
const _u8 MAX_ACTIVE_APPS_COUNT = 5;

static bool _FindAppByIdPredicateBody(const void* expected, const void* value);

AppsManager_t* AppsManagerCreate(void) {
	AppsManager_t* appsManager = (AppsManager_t*)malloc(sizeof(AppsManager_t));

	if (appsManager == NULL) return NULL;

	appsManager->nextAppId = 0;
	appsManager->activeApp = NULL;
	appsManager->menuApp = NULL;
	appsManager->apps = ArrayCreate(MAX_APPS_COUNT);
	appsManager->pausedApps = StackCreate(MAX_ACTIVE_APPS_COUNT);

	return appsManager;
}

const bool AppsManagerAddApp(AppsManager_t* manager, App_t* app) {
	if (ArrayIsFull(manager->apps) == true) {
		return false;
	}

	ArrayAdd(manager->apps, app);
	AppOnInit(app);
	return true;
}

void AppsManagerStartMenuApp(AppsManager_t* manager, App_t* app) {
	if (manager->menuApp != NULL && AppGetState(manager->menuApp) == StateRunning) {
		return;
	}

	if (manager->menuApp == NULL) {
		manager->menuApp = app;
		AppOnOpen(app);
	}
}


void AppsManagerStartLastAddedApp(AppsManager_t* manager) {
	Stack_t* pausedApps = manager->pausedApps;
	Array_t* apps = manager->apps;

	// if we ahave already running active app, exit
	if (manager->activeApp != NULL && AppGetState(manager->activeApp) == StateRunning) {
		return;
	}

	// if no apps available to run/resume
	if (StackIsEmpty(pausedApps) == true && ArrayIsEmpty(apps) == true) {
		return;
	}

	//there is some active app, resume it
	if (StackIsEmpty(pausedApps) == false) {
		App_t* app = StackPop(pausedApps);
		manager->activeApp = app;
		AppOnResume(app);
		if (manager->menuApp != NULL) {
			AppOnPause(manager->menuApp);
		}
		return;
	}

	// if regisered app presents, open it
	if (ArrayIsEmpty(apps) == false) {
		App_t* app = ArrayLastValue(apps);
		manager->activeApp = app;
		AppOnOpen(app);
		if (manager->menuApp != NULL) {
			AppOnPause(manager->menuApp);
		}
		return;
	}
}

void AppsManagerStartAppWithId(AppsManager_t* manager, const _u16 appId) {
	Array_t* apps = manager->apps;

	// if we ahave already running active app, exit
	if (manager->activeApp != NULL && AppGetState(manager->activeApp) == StateRunning) {
		return;
	}

	App_t *app = ArrayFind(apps, (void*)appId, &_FindAppByIdPredicateBody);
	if (app != NULL) {
		manager->activeApp = app;
		AppOnOpen(app);
	}
}

const _u16 AppsManagerNextAppId(AppsManager_t* manager) {
	manager->nextAppId++;
	return manager->nextAppId;
}

void AppsManagerStart(AppsManager_t* manager) {
	//...
}

void AppsManagerUpdate(AppsManager_t* manager) {
	App_t* activeApp = manager->activeApp;
	if (activeApp != NULL) {
		AppOnUpdate(activeApp);
	}
	if (manager->menuApp != NULL) {
		AppOnUpdate(manager->menuApp);
	}
}

void AppsManagerHandleInput(AppsManager_t* manager, const void* keyData) {
	App_t* activeApp = manager->activeApp;
	if (activeApp != NULL) {
		AppOnHandleInput(activeApp, keyData);
		return;
	}

	if (manager->menuApp != NULL) {
		AppOnHandleInput(manager->menuApp, keyData);
	}
}

void AppsManagerPauseActiveApp(AppsManager_t* manager) {
	App_t* activeApp = manager->activeApp;
	if (activeApp != NULL) {
		AppOnPause(activeApp);
		StackPush(manager->pausedApps, activeApp);
		manager->activeApp = NULL;

		if (manager->menuApp != NULL) {
			AppOnResume(manager->menuApp);
		}
	}
}

void AppsManagerResumeActiveApp(AppsManager_t* manager) {
	if (manager->activeApp != NULL) {
		return;
	}

	App_t* app = StackPop(manager->pausedApps);
	if (app != NULL) {
		manager->activeApp = app;
		AppOnResume(app);

		if (manager->menuApp != NULL) {
			AppOnPause(manager->menuApp);
		}
	}
}

void AppsManagerStopActiveApp(AppsManager_t* manager) {
	if (manager->activeApp == NULL) {
		return;
	}

	AppOnPause(manager->activeApp);
	AppOnStop(manager->activeApp);
	AppOnKill(manager->activeApp);
	
	manager->activeApp = NULL;

	if (manager->menuApp != NULL) {
		AppOnResume(manager->menuApp);
	}
}

void AppsManagerStopAppWithId(AppsManager_t* manager, const _u16 appId) {
	if (manager->activeApp == NULL) {
		return;
	}

	if (AppGetId(manager->activeApp) == appId) {
		AppsManagerStopActiveApp(manager);
	}
}

//
static bool _FindAppByIdPredicateBody(const void* expected, const void* value) {
	const _u16 appIdToFind = (_u16)expected;
	App_t* app = (App_t*)value;
	_u16 appId = AppGetId(app);
	return appIdToFind == appId;
}