#ifndef APPS_MANAGER_H
#define APPS_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <array.h>

#include "app.h"

typedef struct AppsManager_t AppsManager_t;

AppsManager_t* AppsManagerCreate(void);

/**
 * @brief register application in appsManger.
 * @returns true if app is added
 */
bool AppsManagerAddApp(AppsManager_t* manager, App_t* app);
void AppsManagerStart(AppsManager_t* manager, App_t* app);
void AppsManagerStartLastAddedApp(AppsManager_t* manager);
void AppsManagerStartAppWithId(AppsManager_t* manager, const _u16 appId);

/**
 * @brief generate id for application
 */
_u16 AppsManagerNextAppId(AppsManager_t* manager);

void AppsManagerUpdate(AppsManager_t* manager);
void AppsManagerHandleInput(AppsManager_t* manager, const void* keyData);
void AppsManagerPauseActiveApp(AppsManager_t* manager);
void AppsManagerResumeActiveApp(AppsManager_t* manager);
void AppsManagerStopActiveApp(AppsManager_t* manager);
void AppsManagerStopAppWithId(AppsManager_t* manager, const _u16 appId);

Array_t* AppsManagerGetAllApps(const AppsManager_t* manager);
App_t* AppsManagerGetActiveApp(const AppsManager_t* manager);

#ifdef __cplusplus
}
#endif

#endif  // APPS_MANAGER_H
