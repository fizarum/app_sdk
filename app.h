#ifndef APP_H
#define APP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include "app_state.h"

	typedef uint8_t _u8;
	typedef int8_t _i8;
	typedef uint16_t _u16;

	/** app's specific implementation */
	typedef struct AppSpecification_t {
		const char* name;
		void (*handleInput)(_u16 keyData);
		void (*onInit)(void);
		void (*onStart)(void);
		void (*onPause)(void);
		void (*onResume)(void);
		void (*onUpdate)(void);
		void (*onStop)(void);
	} AppSpecification_t;

	typedef struct App_t App_t;

	App_t* AppCreate(_u16 id, AppSpecification_t* specification);
	void AppDestroy(App_t* app);

	void AppOnInit(App_t* app);
	void AppOnOpen(App_t* app);
	void AppOnUpdate(App_t* app);
	void AppOnHandleInput(App_t* app, const _u16 keyData);
	void AppOnPause(App_t* app);
	void AppOnResume(App_t* app);
	void AppOnStop(App_t* app);
	void AppOnKill(App_t* app);

	const char* AppGetName(const App_t* app);
	const _u16 AppGetId(const App_t* app);
	const AppState_t AppGetState(const App_t* app);

#ifdef __cplusplus
}
#endif

#endif //APP_H