# app sdk
minimalistic application sdk written in C

## dependencies
This sdk uses [collections](https://github.com/fizarum/collections), so its required to fetch them as well

## description
This is a part of bigger software & hardware project based on this [repo](https://github.com/fizarum/NesCat1.0_ESP32)
The main idea is to make simple, modular sdk to manage apps on embedded device. There are no os/platform 
specific things and any user apps definitions.

There are 2 main components at this moment:
- AppsManager: keeps all applications and allows to manage them
- App: application entity

AppsManager has:
- apps - array of all registered applictions on system
- pausedApps - started earlier, but paused for some reason, organized as stack
- activeApp - currently running app
- menuApp - it's an special app which allows to launch other apps. Should be started as first app 
during cold start

## example
For better understanding how AppsManager works and how to build your own App, check 
[example folder](https://github.com/fizarum/app_sdk/tree/main/example) - it has some simple 
implementation of menu and screenLock apps.

## features to implement:
I have some planned features to to add next:
- Services - its long running task independently from normal App (ideally in separate thread) 
with possibility to bind & unbind to it, get & send data, etc. As an example it can be a service 
which check system health, gather logs, etc.
- Broadcast receiver - its a mechanism (similar to callbacks) which allows to subscribe on various 
predefined system events. For example, app can be subscribed on events for low battery or 
when device go into sleep mode, etc.