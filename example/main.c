#include "../apps_manager.h"
#include "../app.h"
#include <stdio.h>

#include "screenlock_app.h"
#include "menu_app.h"

int main() {
	AppsManager_t* manager = AppsManagerCreate();
	AppsManagerStart(manager);

	printf("==> create app\n");
	App_t* menuApp = AppCreate(1, MenuAppSpecification());
	App_t* screenLockApp = AppCreate(2, ScreenLockAppSpecification());

	printf("==> adding app: %s\n", AppGetName(menuApp));
	printf("==> adding app: %s\n", AppGetName(screenLockApp));
	
	AppsManagerAddApp(manager, menuApp);
	AppsManagerAddApp(manager, screenLockApp);
	
	printf("==> starting app menu\n");
	AppsManagerStartMenuApp(manager, menuApp);
	printf("==> starting app\n");
	AppsManagerStartLastAddedApp(manager);

	printf("==> updating user app\n");
	AppsManagerUpdate(manager);

	printf("==> handle input\n");
	AppsManagerHandleInput(manager, 11);

	printf("==> pausing user app\n");
	AppsManagerPauseActiveApp(manager);

	printf("==> resuming user app\n");
	AppsManagerResumeActiveApp(manager);
	printf("==> stoping user app\n");
	AppsManagerStopActiveApp(manager);

	printf("end!\n");
	while (true)
	{
		//waiting..
	}
}