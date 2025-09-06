#ifndef APPS_MANAGER_H
#define APPS_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <array.h>

#include "app.h"

#define APP_ID_NA UINT16_MAX

typedef struct AppsManager_t AppsManager_t;

AppsManager_t* AppsManagerCreate(void);

App_t* AppsMangerSetLauncher(AppsManager_t* manager, AppSpecification_t* specs);

/**
 * @brief register application in appsManger.
 * @returns app id if added or APP_ID_NA otherwise
 */
_u16 AppsMangerAddAppSpecs(AppsManager_t* manager, AppSpecification_t* specs);
void AppsManagerStart(AppsManager_t* manager, App_t* app);
void AppsManagerStartLastAddedApp(AppsManager_t* manager);
void AppsManagerStartAppWithId(AppsManager_t* manager, const _u16 appId);

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
