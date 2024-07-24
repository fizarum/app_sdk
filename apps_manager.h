#ifndef APPS_MANAGER_H
#define APPS_MANAGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "app.h"

	typedef struct AppsManager_t AppsManager_t;

	AppsManager_t* AppsManagerCreate(void);

	/**
	* register application in appsManger.
	* Returns true if app is added
	*/
	const bool AppsManagerAddApp(AppsManager_t* manager, App_t* app);
	void AppsManagerStartMenuApp(AppsManager_t* manager, App_t* app);
	void AppsManagerStartLastAddedApp(AppsManager_t* manager);

	void AppsManagerStart(AppsManager_t* manager);

	void AppsManagerUpdate(AppsManager_t* manager);
	void AppsManagerHandleInput(AppsManager_t* manager, const _u16 keyData);
	void AppsManagerPauseActiveApp(AppsManager_t* manager);
	void AppsManagerResumeActiveApp(AppsManager_t* manager);
	void AppsManagerStopActiveApp(AppsManager_t* manager);

#ifdef __cplusplus
}
#endif 



#endif // APPS_MANAGER_H

