#ifndef APP_STATE_H
#define APP_STATE_H

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum AppState_t {
		/**
		* app not yet registered in app manager and
		* can't initialized and started
		*/
		StateCreated,

		/**
		* state when app is initialized, but not opened
		* yet (shown on screen). Actual when system loads
		* all app descriptors/metadata
		*/
		StateInit,

		/**
		* opened and wisible, this state is usually
		* used by loading app resources
		*/
		StateOpen,

		/** when app is ready to interact with user */
		StateRunning,

		/**
		* may be used when system sends some event
		* to app (like broadcast)
		*/
		StateUpdate,

		/**
		* if some alert or system menu shown up. Also can
		* happed before app's stop and further kill
		*/
		StatePaused,

		/**
		* called befor app closing. Its handful to store some
		* app's session data
		*/
		StateStoped,

		/** app is killed, can't be shown on screen */
		StateKilled,
	} AppState_t;

#ifdef __cplusplus
}
#endif

#endif //APP_STATE_H