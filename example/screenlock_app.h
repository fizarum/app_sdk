#ifndef SCREENLOCK_APP_H
#define SCREENLOCK_APP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../app.h"

	AppSpecification_t* ScreenLockAppSpecification(UserCallback requestToStopCallback);

#ifdef __cplusplus
}
#endif

#endif //SCREENLOCK_APP_H