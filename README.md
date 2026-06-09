# app sdk
minimalistic application sdk written in C

## dependencies
This sdk uses [collections](https://github.com/fizarum/collections), so its required to fetch them as well

## description
This is a part of bigger software & hardware project based on this [repo](https://github.com/fizarum/gb)
The main idea is to make simple, modular sdk to manage apps on embedded device. There are no os/platform 
specific things.

There are main components at this moment:
- `apps_manager_t`: manages all applications on device
- `app_t`: application itself, holds a reference to specific app implementation - `app_specification_t`
- `app_specification_t` - implementation of user specific application
- `broadcaster_t`: implements broadcast feature (system events) in system, delivers messages, holds subscibers
- `broadcast_subsriber_t`: listener of broadcast message;
- `broadcast_event_t`: event to send by `broadcaster_t` and received by `broadcast_subsriber_t`;

`apps_manager_t` has:
- `registry` - array of all registered applictions on system
- `running_apps` - currently running apps
- `launcher` - "entry point" for apps manager. This is special `app_t` instance which is used to navigate and launch other apps.

## example
Here is example of apps manager & broadcaster:
```c
broadcaster_init();
apps_manager_init();

// file manager app
apps_manager_register(FileMangerAppSpecification());
// settings app
apps_manager_register(SettingsAppSpecification());
// menu (launcher)
_u16 menu_app_id = apps_manger_set_launcher(MenuAppSpecification());

// subscribing on battery charging events
broadcaster_add_listener(ChargingOn, menu_app_id);
broadcaster_add_listener(ChargingOff, menu_app_id);

apps_manager_start();

//"app loop"
while (1) {
    broadcaster_update();
    apps_manager_update();
    //....

    //if "close" button pressed and we want to close some app

    // stop last added running app
    Array_t* running = apps_manager_get_running_apps();
    app_t* app = ArrayLastValue(running);
    apps_manager_close_app(AppGetId(app));
}
```

```c
// open some app, for example in launcher:
//app_t* app = ....
apps_manager_open_app(AppGetId(app));

// or just:
// _u16 app_id = ...
apps_manager_open_app(app_id);
```


## features to implement:
I have some planned features to to add next:
- Services - its long running task independently from normal App (ideally in separate thread) 
with possibility to bind & unbind to it, get & send data, etc. As an example it can be a service 
which checks system health, gather logs, etc.